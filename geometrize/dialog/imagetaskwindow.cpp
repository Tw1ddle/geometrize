#include "imagetaskwindow.h"
#include "ui_imagetaskwindow.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

#include <QEvent>
#include <QLocale>
#include <QMessageBox>
#include <QPixmap>
#include <QRectF>
#include <QTimer>

#include "chaiscript/chaiscript.hpp" // TODO remove

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/runner/imagerunneroptions.h"

#include "common/uiactions.h"
#include "common/util.h"
#include "dialog/imagetaskimagewidget.h"
#include "dialog/scripteditorwidget.h"
#include "image/imageloader.h"
#include "localization/strings.h"
#include "preferences/globalpreferences.h"
#include "script/geometrizerengine.h"
#include "scene/imagetaskgraphicsview.h"
#include "scene/imagetaskscenemanager.h"
#include "scene/tools/areaofinfluenceshapes.h"
#include "task/imagetask.h"
#include "task/shapecollection.h"
#include "version/versioninfo.h"

#if defined DATASLINGER_INCLUDED
#include "dataslinger/imageslinger.h"
#include "geometrize/exporter/svgexporter.h"
#endif

namespace
{

// Utility function for destroying an image task set on a window.
// This has a special case because we may need to defer task deletion until the task is finished working.
void destroyTask(geometrize::task::ImageTask* task)
{
    if(task == nullptr) {
        assert(0 && "Attempted to destroy an image task that was already null");
        return;
    }

    if(task->isStepping()) {
        // Wait until the task finishes stepping before disposing of it
        // Otherwise it will probably crash as the Geometrize library will be working with deleted data
        task->connect(task, &geometrize::task::ImageTask::signal_modelDidStep, [task](std::vector<geometrize::ShapeResult>) {
            task->disconnect(nullptr, nullptr, nullptr, nullptr);
            task->deleteLater();
        });
    } else {
        task->disconnect(nullptr, nullptr, nullptr, nullptr);
        task->deleteLater();
    }
}

}

namespace geometrize
{

namespace dialog
{

class ImageTaskWindow::ImageTaskWindowImpl
{
public:
    ImageTaskWindowImpl(ImageTaskWindow* pQ) :
        ui{std::make_unique<Ui::ImageTaskWindow>()},
        q{pQ}
    {
        ui->setupUi(q);
        populateUi();
        q->setAttribute(Qt::WA_DeleteOnClose);

        // Set up the dock widgets
        q->tabifyDockWidget(ui->runnerSettingsDock, ui->scriptsDock);
        q->tabifyDockWidget(ui->runnerSettingsDock, ui->exporterDock);

        ui->runnerSettingsDock->raise(); // Make sure runner settings dock is selected

        ui->consoleWidget->setVisible(false); // Make sure console widget is hidden by default

        // Set up the image task geometrization views
        m_pixmapView = new geometrize::scene::ImageTaskGraphicsView(ui->imageViewContainer);
        m_pixmapView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_pixmapView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->imageViewContainer->layout()->addWidget(m_pixmapView);

        m_svgView = new geometrize::scene::ImageTaskGraphicsView(ui->imageViewContainer);
        m_svgView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_svgView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->imageViewContainer->layout()->addWidget(m_svgView);

        m_sceneManager.setViews(*m_pixmapView, *m_svgView);

        // Set initial view visibility
        const geometrize::preferences::GlobalPreferences& prefs{geometrize::preferences::getGlobalPreferences()};
        setConsoleVisibility(prefs.shouldShowImageTaskConsoleByDefault());
        setPixmapViewVisibility(prefs.shouldShowImageTaskPixmapViewByDefault());
        setVectorViewVisibility(prefs.shouldShowImageTaskVectorViewByDefault());

        // Handle clicks on checkable title bar items
        connect(ui->actionScript_Console, &QAction::toggled, [this](const bool checked) {
            setConsoleVisibility(checked);
        });
        connect(ui->actionPixmap_Results_View, &QAction::toggled, [this](const bool checked) {
            setPixmapViewVisibility(checked);
        });
        connect(ui->actionVector_Results_View, &QAction::toggled, [this](const bool checked) {
            setVectorViewVisibility(checked);
        });

        // Handle request to set the image task on the task window
        connect(q, &ImageTaskWindow::willSwitchImageTask, [this](task::ImageTask*, task::ImageTask*) {
            // Disconnect the last image task, which will soon be replaced by the next image task
            disconnectTask();

            m_shapes.clear();
        });
        connect(q, &ImageTaskWindow::didSwitchImageTask, [this](task::ImageTask* lastTask, task::ImageTask* currentTask) {
            ui->imageTaskExportWidget->setImageTask(currentTask, &m_shapes.getShapeVector());
            ui->imageTaskRunnerWidget->setImageTask(currentTask);

            if(dialog::ImageTaskPrePostScriptsWidget* scriptingPanel = getScriptingPanel()) {
                scriptingPanel->setImageTask(currentTask);
            }

            m_taskPreferencesSetConnection = connect(currentTask, &task::ImageTask::signal_preferencesSet, [this]() {
                ui->imageTaskRunnerWidget->syncUserInterface();
            });

            m_taskWillStepConnection = connect(currentTask, &task::ImageTask::signal_modelWillStep, [this]() {
                ui->statsDockContents->setCurrentStatus(geometrize::dialog::ImageTaskStatsWidget::RUNNING);

                // Apply the latest scripts and engine state prior to stepping
                auto& geometrizer = m_task->getGeometrizer();

                chaiscript::ChaiScript* engine = geometrizer.getEngine();

                const bool scriptModeEnabled = m_task->getPreferences().isScriptModeEnabled();
                if(scriptModeEnabled) {
                    geometrizer.setupScripts(m_task->getPreferences().getScripts());
                }

                engine->set_global(chaiscript::var(static_cast<int>(m_task->getWidth())), "xBound");
                engine->set_global(chaiscript::var(static_cast<int>(m_task->getHeight())), "yBound");
                engine->set_global(chaiscript::var(m_areaOfInfluenceShapes.getCurrentShape()), "aoi");
                engine->set_global(chaiscript::var(m_shapes.getShapeVector().size()), "currentShapeCount");

                std::vector<std::pair<std::int32_t, std::int32_t>> aoiPixels = m_areaOfInfluenceShapes.getPixels(m_task->getWidth(), m_task->getHeight());
                if(aoiPixels.empty()) {
                    aoiPixels.push_back(std::make_pair(m_task->getWidth() / 2, m_task->getHeight() / 2));
                }
                engine->set_global(chaiscript::var(aoiPixels), "aoiPixels");
            });

            m_taskDidStepConnection = connect(currentTask, &task::ImageTask::signal_modelDidStep, [this](std::vector<geometrize::ShapeResult> shapes) {
                processPostStepCbs();

                // If the first shape added background rectangle then fit the scenes to it
                if(m_shapes.empty()) {
                    m_sceneManager.fitScenesInViews(*m_pixmapView, *m_svgView);
                }
                m_shapes.appendShapes(shapes);

                updateStats();

                if(shouldKeepStepping()) {
                    stepModel();
                }
            });

            q->setWindowTitle(geometrize::strings::Strings::getApplicationName()
                              .append(" ")
                              .append(geometrize::version::getApplicationVersionString())
                              .append(" ")
                              .append(QString::fromStdString(currentTask->getDisplayName())));

            const QPixmap target{image::createPixmap(m_task->getTarget())};
            m_sceneManager.setTargetPixmap(target);

            currentTask->drawBackgroundRectangle();

            ui->consoleWidget->setEngine(currentTask->getGeometrizer().getEngine());
            ui->imageTaskRunnerWidget->syncUserInterface();

            ui->imageTaskImageWidget->setTargetImage(image::createImage(currentTask->getTarget()));

            m_timeRunning = 0.0f;

            #if defined DATASLINGER_INCLUDED
            // Bind keyboard shortcuts, setup UI for sending images out over the network etc
            geometrize::installImageSlingerUserInterface(q);
            #endif

            // As a final step, dispose of the old image task, if there was one
            if(lastTask != nullptr) {
                destroyTask(lastTask);
            }
        });

        // Handle requested target image overlay opacity changes
        connect(ui->imageTaskImageWidget, &ImageTaskImageWidget::targetImageOpacityChanged, [this](const unsigned int value) {
            const float opacity{value * (1.0f / 255.0f)};
            m_sceneManager.setTargetPixmapOpacity(opacity);
        });

        // Handle a request to change the target image
        connect(ui->imageTaskImageWidget, &ImageTaskImageWidget::targetImageSelected, [this](const QImage& image) {
            assert(!image.isNull());

            // Validate the target image size
            const geometrize::Bitmap& target{m_task->getTarget()};
            const int targetWidth{static_cast<int>(target.getWidth())};
            const int targetHeight{static_cast<int>(target.getHeight())};
            if(targetWidth != image.width() || targetHeight != image.height()) {
                const QString selectedImageSize(tr("%1x%2", "Dimensions of an image e.g. width-x-height, 1024x800").arg(QLocale().toString(image.width())).arg(QLocale().toString(image.height())));
                const QString targetImageSize(tr("%1x%2", "Dimensions of an image e.g. width-x-height, 1024x800").arg(QLocale().toString(targetWidth)).arg(QLocale().toString(targetHeight)));
                QMessageBox::warning(
                            q,
                            tr("Image has incorrect dimensions", "Title of an error dialog shown when the user selects an image that was the wrong resolution/size"),
                            tr("Selected image must have the same dimensions as the current target image. Size was %1, but should have been %2",
                               "Error message shown when the user selects an image that was the wrong resolution/size").arg(selectedImageSize).arg(targetImageSize));

                return;
            }

            setTargetImage(image);
        });

        // Handle a request to change the target image that has passed size checks and validation
        connect(ui->imageTaskImageWidget, &ImageTaskImageWidget::targetImageSet, [this](const QImage& image) {
            // If the task is running then defer the target image change to the next step, else do it immediately
            if(isRunning()) {
                const QImage imageCopy{image.copy()};
                addPostStepCb([this, imageCopy]() {
                    Bitmap target = geometrize::image::createBitmap(imageCopy);
                    switchTargetImage(target);
                });
            } else {
                Bitmap target = geometrize::image::createBitmap(image);
                switchTargetImage(target);
            }
        });

        // Handle runner button presses
        connect(ui->imageTaskRunnerWidget, &ImageTaskRunnerWidget::runStopButtonClicked, [this]() {
            setShouldKeepStepping(!shouldKeepStepping());

            // Request another image task step if user clicked start here
            if(shouldKeepStepping()) {
                stepModel();
            }
        });
        connect(ui->imageTaskRunnerWidget, &ImageTaskRunnerWidget::stepButtonClicked, [this]() {
            stepModel();
        });
        connect(ui->imageTaskRunnerWidget, &ImageTaskRunnerWidget::clearButtonClicked, [this]() {
            clearModel();
        });

        connect(q, &ImageTaskWindow::didLoadSettingsTemplate, [this]() {
            ui->imageTaskRunnerWidget->syncUserInterface();

            if(dialog::ImageTaskPrePostScriptsWidget* scriptingPanel = getScriptingPanel()) {
                 scriptingPanel->syncUserInterface();
            }
        });

        // Track how long the task has been in the running state
        connect(&m_timeRunningTimer, &QTimer::timeout, [this]() {
            if(isRunning()) {
                m_timeRunning += m_timeRunningResolutionMs;
                updateStats();
            }
        });

        // When number of shapes changes check the scripts
        connect(&m_shapes, &geometrize::task::ShapeCollection::signal_sizeChanged, [this](const std::size_t size) {
            if(!ui->scriptsWidget->stopConditionsMet(size)) {
                return;
            }

            setShouldKeepStepping(false);
            geometrize::dialog::showImageTaskStopConditionMetMessage(q);
        });

        // Update the graphical image views when shapes are added
        connect(&m_shapes, &geometrize::task::ShapeCollection::signal_appendedShapes, [this](const std::vector<geometrize::ShapeResult>& shapes) {
            const QPixmap pixmap{image::createPixmap(m_task->getCurrent())};
            m_sceneManager.updateScenes(pixmap, shapes);
        });

        #if defined DATASLINGER_INCLUDED
        // Send the newly added SVG shape data out to listening clients
        connect(&m_shapes, &geometrize::task::ShapeCollection::signal_appendedShapes, [](const std::vector<geometrize::ShapeResult>& shapes) {

            // Exporting rotated ellipses as polygons, since OpenFL's SVG library can't handle regular rotated SVG ellipse or paths
            geometrize::exporter::SVGExportOptions options;
            options.rotatedEllipseExportMode = geometrize::exporter::RotatedEllipseSVGExportMode::POLYGON;

            for(const auto& result : shapes) {
                geometrize::sendSvgShapeData(geometrize::exporter::getSingleShapeSVGData(result.color, *result.shape, options));
            }
        });
        #endif

        // Update the graphical image views when the number of shapes changes (e.g. when cleared)
        connect(&m_shapes, &geometrize::task::ShapeCollection::signal_sizeChanged, [this](const std::size_t size) {
            if(size == 0) {
                m_sceneManager.reset();
            }
        });

        // Connect controls/manipulation in the scenes to the actual area of influence shapes
        m_areaOfInfluenceShapes.connectToSceneManager(m_sceneManager);

        // Update the scene whenever the current area of influence shape is modified
        connect(&m_areaOfInfluenceShapes, &geometrize::scene::tools::AreaOfInfluenceShapes::signal_didModifyShape, [this](const geometrize::Shape& shape) {
            m_sceneManager.setAreaOfInfluenceShape(shape);
        });
        m_sceneManager.setAreaOfInfluenceShape(*m_areaOfInfluenceShapes.getCurrentShape().get());

        // Set initial target image opacity
        const float initialTargetImageOpacity{10};
        ui->imageTaskImageWidget->setTargetImageOpacity(static_cast<unsigned int>(initialTargetImageOpacity));

        // Start the timer used to track how long the image task has been in the running state
        m_timeRunningTimer.start(static_cast<int>(m_timeRunningResolutionMs));
    }
    ImageTaskWindowImpl& operator=(const ImageTaskWindowImpl&) = delete;
    ImageTaskWindowImpl(const ImageTaskWindowImpl&) = delete;
    ~ImageTaskWindowImpl()
    {
        disconnectTask();
        destroyTask(m_task);
    }

    void close()
    {
        q->close();
    }

    task::ImageTask* getImageTask()
    {
        return m_task;
    }

    void setImageTask(task::ImageTask* task)
    {
        task::ImageTask* lastTask{m_task};
        task::ImageTask* nextTask{task};
        q->willSwitchImageTask(lastTask, nextTask);
        m_task = nextTask;
        q->didSwitchImageTask(lastTask, nextTask);
    }

    void revealLaunchWindow()
    {
        if(common::ui::isLaunchWindowOpen()) {
            common::ui::bringLaunchWindowToFront();
        } else {
            common::ui::openLaunchWindow();
        }
    }

    void setConsoleVisibility(const bool visible)
    {
        if(ui->actionScript_Console->isChecked() != visible) {
            ui->actionScript_Console->setChecked(visible);
        }
        ui->consoleWidget->setVisible(visible);
    }

    void setPixmapViewVisibility(const bool visible)
    {
        if(ui->actionPixmap_Results_View->isChecked() != visible) {
            ui->actionPixmap_Results_View->setChecked(visible);
        }

        m_pixmapView->setVisible(visible);
        if(visible) {
            m_sceneManager.fitPixmapSceneInView(*m_pixmapView);
        }
    }

    void setVectorViewVisibility(const bool visible)
    {
        if(ui->actionVector_Results_View->isChecked() != visible) {
            ui->actionVector_Results_View->setChecked(visible);
        }

        m_svgView->setVisible(visible);
        if(visible) {
            m_sceneManager.fitVectorSceneInView(*m_svgView);
        }
    }

    void loadSettingsTemplate()
    {
        const QString path{common::ui::openLoadImageTaskSettingsDialog(q)};
        loadSettingsTemplate(path.toStdString());
    }

    void loadSettingsTemplate(const std::string& path)
    {
        if(path.empty()) {
            return;
        }
        m_task->getPreferences().load(path);
        emit q->didLoadSettingsTemplate();
    }

    void saveSettingsTemplate() const
    {
        const QString path{common::ui::openSaveImageTaskSettingsDialog(q)};
        if(path.isEmpty()) {
            return;
        }
        m_task->getPreferences().save(path.toStdString());

        emit q->didSaveSettingsTemplate();
    }

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    void populateUi()
    {
        updateStartStopButtonText();
        q->setWindowTitle(geometrize::strings::Strings::getApplicationName());
    }

    geometrize::dialog::ImageTaskPrePostScriptsWidget* getScriptingPanel()
    {
        return ui->scriptsDock->findChild<geometrize::dialog::ImageTaskPrePostScriptsWidget*>();
    }

    bool isRunning() const
    {
        if(!m_task) {
            return false;
        }
        return m_task->isStepping();
    }

    bool shouldKeepStepping() const
    {
        return m_shouldKeepStepping;
    }

    void setShouldKeepStepping(const bool stepping)
    {
        m_shouldKeepStepping = stepping;
        updateStartStopButtonText();
    }

    void updateStartStopButtonText()
    {
        if(!shouldKeepStepping()) {
            ui->imageTaskRunnerWidget->setRunStopButtonText(tr("Start", "Text on a button that the user presses to make the app start/begin transforming an image into shapes"));
        } else {
            ui->imageTaskRunnerWidget->setRunStopButtonText(tr("Stop", "Text on a button that the user presses to make the app stop/pause transforming an image into shapes"));
        }
    }

    void stepModel()
    {
        m_task->stepModel();
    }

    void clearModel()
    {
        auto task = new geometrize::task::ImageTask(m_task->getDisplayName(), m_task->getTarget());
        task->setPreferences(m_task->getPreferences());
        setImageTask(task);
    }

    void switchTargetImage(Bitmap& bitmap)
    {
        Bitmap& targetBitmap{m_task->getTarget()};
        targetBitmap = bitmap;
    }

    void switchCurrentImage(Bitmap& bitmap)
    {
        Bitmap& currentBitmap{m_task->getCurrent()};
        currentBitmap = bitmap;
    }

    void updateStats()
    {
        ui->statsDockContents->setTaskId(m_task->getTaskId());
        ui->statsDockContents->setImageDimensions(m_task->getWidth(), m_task->getHeight());

        // Note this is "stopped" when !isRunning alone, but this prevents flashing while
        // the user has it continually adding shapes
        if(!isRunning() && !shouldKeepStepping()) {
            ui->statsDockContents->setCurrentStatus(geometrize::dialog::ImageTaskStatsWidget::STOPPED);
        }

        ui->statsDockContents->setShapeCount(m_shapes.size());

        if(!m_shapes.empty()) {
            ui->statsDockContents->setSimilarity(m_shapes.back().score * 100.0f);
        }

        ui->statsDockContents->setTimeRunning(static_cast<int>(m_timeRunning / 1000.0f));
    }

    void disconnectTask()
    {
        if(m_taskPreferencesSetConnection) {
            disconnect(m_taskPreferencesSetConnection);
        }
        if(m_taskWillStepConnection) {
            disconnect(m_taskWillStepConnection);
        }
        if(m_taskDidStepConnection) {
            disconnect(m_taskDidStepConnection);
        }
    }

    void setTargetImage(const QImage& image)
    {
        ui->imageTaskImageWidget->setTargetImage(image);
    }

    void addPostStepCb(const std::function<void()>& f)
    {
        m_onPostStepCbs.push_back(f);
    }

    void clearPostStepCbs()
    {
        m_onPostStepCbs.clear();
    }

    void processPostStepCbs()
    {
        for(const auto& f : m_onPostStepCbs) {
            f();
        }
        clearPostStepCbs();
    }

    std::unique_ptr<Ui::ImageTaskWindow> ui{nullptr};
    ImageTaskWindow* q{nullptr};

    task::ImageTask* m_task{nullptr}; ///> The image task currently set and manipulated via this window
    QMetaObject::Connection m_taskPreferencesSetConnection{}; ///> Connection for telling the dialog when the image task preferences are set
    QMetaObject::Connection m_taskWillStepConnection{}; ///> Connection for the window to do work just prior the image task starts a step
    QMetaObject::Connection m_taskDidStepConnection{}; ///> Connection for the window to do work just after the image task finishes a step
    std::vector<std::function<void()>> m_onPostStepCbs; ///> One-shot callbacks triggered when the image task finishes a step

    geometrize::task::ShapeCollection m_shapes; ///> Collection of shapes added so far

    geometrize::scene::tools::AreaOfInfluenceShapes m_areaOfInfluenceShapes; ///> Shapes that can be used to control where shapes are allowed to be spawned/mutated (when a corresponding scripting mode is enabled)

    geometrize::scene::ImageTaskSceneManager m_sceneManager; ///> Manager for scenes containing the pixmap/vector-based representations of the shapes etc
    geometrize::scene::ImageTaskGraphicsView* m_pixmapView{nullptr}; ///> The view that holds the raster/pixel-based scene
    geometrize::scene::ImageTaskGraphicsView* m_svgView{nullptr}; ///> The view that holds the vector-based scene

    bool m_shouldKeepStepping{false}; ///> Whether to continually step i.e. whether to start another step after stepping once
    QTimer m_timeRunningTimer; ///> Timer used to keep track of how long the image task has been in the "running" state
    float m_timeRunning{0.0f}; ///> Total time that the image task has been in the "running" state
    const float m_timeRunningResolutionMs{100.0f}; ///> Resolution of the time running timer
};

ImageTaskWindow::ImageTaskWindow() :
    QMainWindow{nullptr},
    d{std::make_unique<ImageTaskWindow::ImageTaskWindowImpl>(this)}
{
}

ImageTaskWindow::~ImageTaskWindow()
{
}

task::ImageTask* ImageTaskWindow::getImageTask()
{
    return d->getImageTask();
}

void ImageTaskWindow::setImageTask(task::ImageTask* task)
{
    d->setImageTask(task);
}

void ImageTaskWindow::on_actionExit_triggered()
{
    d->close();
}

void ImageTaskWindow::on_actionLoad_Settings_Template_triggered()
{
    d->loadSettingsTemplate();
}

void ImageTaskWindow::on_actionSave_Settings_Template_triggered()
{
    d->saveSettingsTemplate();
}

void ImageTaskWindow::on_actionReveal_Launch_Window_triggered()
{
    d->revealLaunchWindow();
}

void ImageTaskWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QMainWindow::changeEvent(event);
}

}

}
