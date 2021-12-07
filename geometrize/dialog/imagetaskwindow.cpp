#include "imagetaskwindow.h"
#include "ui_imagetaskwindow.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <string>
#include <vector>

#include <QByteArray>
#include <QCryptographicHash>
#include <QEvent>
#include <QKeySequence>
#include <QLocale>
#include <QMessageBox>
#include <QPixmap>
#include <QRectF>
#include <QTimer>

#include "chaiscript/chaiscript.hpp"

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/commonutil.h"
#include "geometrize/runner/imagerunneroptions.h"

#include "common/uiactions.h"
#include "common/util.h"
#include "dialog/imagetaskimagewidget.h"
#include "dialog/scripteditorwidget.h"
#include "image/imageloader.h"
#include "localization/strings.h"
#include "preferences/globalpreferences.h"
#include "script/geometrizerengine.h"
#include "script/command.h"
#include "script/commandhandler.h"
#include "scene/imagetaskgraphicsview.h"
#include "scene/imagetaskscenemanager.h"
#include "task/imagetask.h"
#include "task/shapecollection.h"
#include "version/versioninfo.h"
#include "tabletproximityeventfilter.h"

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
            task->disconnect();
            task->deleteLater();
        });
    } else {
        task->disconnect();
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
        ui->imageViewContainer->layout()->addWidget(m_pixmapView);

        m_svgView = new geometrize::scene::ImageTaskGraphicsView(ui->imageViewContainer);
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
        connect(q, &ImageTaskWindow::willSwitchImageTask, [this](task::ImageTask* lastTask, task::ImageTask* nextTask) {
            // Disconnect the last image task, which will soon be replaced by the next image task
            disconnectTask();

            m_shapes.clear();
            ui->imageTaskRunnerWidget->setImageTask(nextTask);
            ui->scriptsWidget->setImageTask(nextTask);
            ui->imageTaskExportWidget->setImageTask(nextTask, &m_shapes.getShapeVector());

            const auto taskIdForSerialization = [](const task::ImageTask& task) {
                std::string s = std::to_string(task.getWidth()) + "_" + std::to_string(task.getHeight()) + "_";

                QCryptographicHash hash(QCryptographicHash::Md5);
                hash.addData(reinterpret_cast<const char*>(task.getTarget().getDataRef().data()), task.getTarget().getDataRef().size());
                const QByteArray result = hash.result().toHex();
                const std::string hashStr = std::string(result.begin(), result.end());
                return s + hashStr;
            };

            // Autosave the preferences for the task being switched away from, if there was one
            if(lastTask != nullptr && geometrize::preferences::getGlobalPreferences().shouldAutoSaveImageTaskSettings()) {
                const std::string path{geometrize::preferences::getImageTaskPreferencesAutoSavePath(taskIdForSerialization(*lastTask))};
                if(path.empty()) {
                    assert(0 && "Auto save path for image task file must not be empty");
                }
                saveSettingsTemplate(lastTask, path);
            }

            // Autoload the preferences for the task being switched to, if there is one
            if(nextTask != nullptr && geometrize::preferences::getGlobalPreferences().shouldAutoLoadImageTaskSettings()) {
                const std::string path{geometrize::preferences::getImageTaskPreferencesAutoSavePath(taskIdForSerialization(*nextTask))};
                if(path.empty()) {
                    assert(0 && "Auto load path for image task file must not be empty");
                }
                loadSettingsTemplate(nextTask, path);
            }
        });
        connect(q, &ImageTaskWindow::didSwitchImageTask, [this](task::ImageTask* lastTask, task::ImageTask* currentTask) {
            if(currentTask == nullptr) {
                m_taskPreferencesSetConnection = QMetaObject::Connection{};
                m_taskWillStepConnection = QMetaObject::Connection{};
                return; // Don't try to connect if we're setting the current task to nothing/nullptr
            }

            m_taskPreferencesSetConnection = connect(currentTask, &task::ImageTask::signal_preferencesSet, [this]() {
                ui->imageTaskRunnerWidget->syncUserInterface();
                ui->scriptsWidget->syncUserInterface();
            });

            m_taskWillStepConnection = connect(currentTask, &task::ImageTask::signal_modelWillStep, [this]() {
                ui->statsDockContents->setCurrentStatus(geometrize::dialog::ImageTaskStatsWidget::RUNNING);

                // Apply the latest scripts and engine state prior to stepping
                auto& geometrizer = m_task->getGeometrizer();

                m_task->getPreferences().setScripts(ui->scriptsWidget->getScripts());

                chaiscript::ChaiScript* engine = geometrizer.getEngine();
                engine->set_global(chaiscript::var(m_shapes.getShapeVector().size()), "currentShapeCount");

                ui->scriptsWidget->evaluateBeforeStepScripts();
            });

            m_taskDidStepConnection = connect(currentTask, &task::ImageTask::signal_modelDidStep, [this](std::vector<geometrize::ShapeResult> shapes) {
                processPostStepCbs();

                // If the first shape added background rectangle then fit the scenes to it
                if(m_shapes.empty()) {
                    m_sceneManager.fitScenesInViews(*m_pixmapView, *m_svgView);
                }
                m_shapes.appendShapes(shapes);

                updateStats();

                ui->scriptsWidget->evaluateAfterStepScripts();

                if(shouldKeepStepping()) {
                    stepModel();
                }
            });

            const QString windowTitle = [currentTask]() {
                QString title = geometrize::strings::Strings::getApplicationName()
                        .append(" ").append(geometrize::version::getApplicationVersionString());
                if(currentTask != nullptr) {
                    title.append(" ").append(QString::fromStdString(currentTask->getDisplayName()));
                }
                return title;
            }();
            q->setWindowTitle(windowTitle);

            if(currentTask != nullptr) {
                const QPixmap target{image::createPixmap(m_task->getTarget())};
                m_sceneManager.setTargetPixmap(target);
            } else {
                m_sceneManager.setTargetPixmap(QPixmap());
            }

            if(currentTask != nullptr) {
                const auto getStartingColor = [this]() {
                    const geometrize::preferences::GlobalPreferences& prefs{geometrize::preferences::getGlobalPreferences()};
                    if(prefs.shouldUseCustomImageTaskBackgroundOverrideColor()) {
                        const auto color = prefs.getCustomImageTaskBackgroundOverrideColor();
                        return geometrize::rgba{ static_cast<std::uint8_t>(color[0]), static_cast<std::uint8_t>(color[1]), static_cast<std::uint8_t>(color[2]), static_cast<std::uint8_t>(color[3]) };
                    }
                    return geometrize::commonutil::getAverageImageColor(m_task->getTarget());
                };
                currentTask->drawBackgroundRectangle(getStartingColor());
            }

            if(currentTask != nullptr) {
                ui->consoleWidget->setEngine(currentTask->getGeometrizer().getEngine());
            } else {
                ui->consoleWidget->setEngine(nullptr);
            }

            ui->imageTaskRunnerWidget->syncUserInterface();
            ui->scriptsWidget->syncUserInterface();

            if(currentTask != nullptr) {
                ui->imageTaskImageWidget->setTargetImage(image::createImage(currentTask->getTarget()));
            } else {
                ui->imageTaskImageWidget->setTargetImage(QImage());
            }

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
            ui->scriptsWidget->syncUserInterface();
        });

        // Track how long the task has been in the running state
        connect(&m_timeRunningTimer, &QTimer::timeout, [this]() {
            if(isRunning()) {
                m_timeRunning += m_timeRunningTimerResolutionMs;
                updateStats();
            }
        });

        connect(&m_scriptEngineUpdateTimer, &QTimer::timeout, [this]() {
            ui->scriptsWidget->evaluateOnTimedUpdateEventScripts();
        });

        // Before shapes are added, evaluate he pre-add shape callbacks
        connect(&m_shapes, &geometrize::task::ShapeCollection::signal_beforeAppendShapes, [this](const std::vector<geometrize::ShapeResult>&) {
            ui->scriptsWidget->evaluateBeforeAddShapeScripts();
        });

        // After shapes are added, evaluate the post-add shape callbacks and stop conditions
        connect(&m_shapes, &geometrize::task::ShapeCollection::signal_afterAppendShapes, [this](const std::vector<geometrize::ShapeResult>&) {
            chaiscript::ChaiScript* engine = m_task->getGeometrizer().getEngine();
            engine->set_global(chaiscript::var(m_shapes.back().shape->clone()), "lastAddedShape");
            engine->set_global(chaiscript::var(m_shapes.back().score), "lastAddedShapeScore");
            engine->set_global(chaiscript::var(m_shapes.back().color), "lastAddedShapeColor");

            ui->scriptsWidget->evaluateAfterAddShapeScripts();

            if(!ui->scriptsWidget->evaluateStopConditionScripts()) {
                return;
            }
            setShouldKeepStepping(false);
            ui->scriptsWidget->evaluateOnStopConditionMetScripts();
            //geometrize::dialog::showImageTaskStopConditionMetMessage(q); // Letting the scripts handle notifying the user
        });

        // Update the graphical image views after shapes are added
        connect(&m_shapes, &geometrize::task::ShapeCollection::signal_afterAppendShapes, [this](const std::vector<geometrize::ShapeResult>& shapes) {
            const QPixmap pixmap{image::createPixmap(m_task->getCurrent())};
            m_sceneManager.updateScenes(pixmap, shapes);
        });

        #if defined DATASLINGER_INCLUDED
        // Send the newly added SVG shape data out to listening clients
        connect(&m_shapes, &geometrize::task::ShapeCollection::signal_afterAppendShapes, [](const std::vector<geometrize::ShapeResult>& shapes) {

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

        // Pass the latest mouse event info to the current image task's script engine
        connect(&m_sceneManager, &geometrize::scene::ImageTaskSceneManager::signal_onTargetImageHoverMoveEvent, [this](double lastX, double lastY, double x, double y, bool /*ctrlModifier*/) {
            chaiscript::ChaiScript* engine = m_task->getGeometrizer().getEngine();
            engine->set_global(chaiscript::var(lastX), "targetImageLastMouseX");
            engine->set_global(chaiscript::var(lastY), "targetImageLastMouseY");
            engine->set_global(chaiscript::var(x), "targetImageMouseX");
            engine->set_global(chaiscript::var(y), "targetImageMouseY");

            ui->scriptsWidget->evaluateOnMouseMoveEventScripts();
        });
        connect(&m_sceneManager, &geometrize::scene::ImageTaskSceneManager::signal_onTargetImageMousePressEvent, [this](double x, double y, bool /*ctrlModifier*/) {
            chaiscript::ChaiScript* engine = m_task->getGeometrizer().getEngine();
            engine->set_global(chaiscript::var(x), "targetImageLastMouseX");
            engine->set_global(chaiscript::var(y), "targetImageLastMouseY");
            engine->set_global(chaiscript::var(x), "targetImageMouseX");
            engine->set_global(chaiscript::var(y), "targetImageMouseY");

            ui->scriptsWidget->evaluateOnMouseDownEventScripts();
        });
        connect(&m_sceneManager, &geometrize::scene::ImageTaskSceneManager::signal_onTargetImageMouseMoveEvent, [this](double lastX, double lastY, double x, double y, bool /*ctrlModifier*/) {
            // NOTE not triggered currently, hover move events are used instead
            chaiscript::ChaiScript* engine = m_task->getGeometrizer().getEngine();
            engine->set_global(chaiscript::var(lastX), "targetImageLastMouseX");
            engine->set_global(chaiscript::var(lastY), "targetImageLastMouseY");
            engine->set_global(chaiscript::var(x), "targetImageMouseX");
            engine->set_global(chaiscript::var(y), "targetImageMouseY");

            ui->scriptsWidget->evaluateOnMouseMoveEventScripts();
        });
        connect(&m_sceneManager, &geometrize::scene::ImageTaskSceneManager::signal_onTargetImageMouseReleaseEvent, [this](double x, double y, bool /*ctrlModifier*/) {
            chaiscript::ChaiScript* engine = m_task->getGeometrizer().getEngine();
            engine->set_global(chaiscript::var(x), "targetImageLastMouseX");
            engine->set_global(chaiscript::var(y), "targetImageLastMouseY");
            engine->set_global(chaiscript::var(x), "targetImageMouseX");
            engine->set_global(chaiscript::var(y), "targetImageMouseY");

            ui->scriptsWidget->evaluateOnMouseUpEventScripts();
        });
        connect(&m_sceneManager, &geometrize::scene::ImageTaskSceneManager::signal_onTargetImageWheelEvent, [this](double x, double y, int amount, bool /*ctrlModifier*/) {
            chaiscript::ChaiScript* engine = m_task->getGeometrizer().getEngine();
            engine->set_global(chaiscript::var(x), "targetImageLastMouseX");
            engine->set_global(chaiscript::var(y), "targetImageLastMouseY");
            engine->set_global(chaiscript::var(x), "targetImageMouseX");
            engine->set_global(chaiscript::var(y), "targetImageMouseY");
            engine->set_global(chaiscript::var(amount), "targetImageWheelMoveAmount");

            ui->scriptsWidget->evaluateOnMouseWheelEventScripts();
        });

        // Pass the latest key event info to the current image task's script engine
        connect(&m_sceneManager, &geometrize::scene::ImageTaskSceneManager::signal_onTargetImageKeyPressEvent, [this](int key, bool ctrlModifier) {
            chaiscript::ChaiScript* engine = m_task->getGeometrizer().getEngine();
            const std::string keyString = QKeySequence(key).toString().toStdString();

            // Update vars for pressed keys in the engine (cleared on key release)
            engine->set_global(chaiscript::var(keyString), "targetImageLastKeyDown");
            engine->set_global(chaiscript::var(true), "targetImageKeyDown_" + keyString);
            engine->set_global(chaiscript::var(ctrlModifier), "targetImageControlModifierDown");

            // Last pressed key in the engine, not cleared on key release
            engine->set_global(chaiscript::var(keyString), "targetImageLastKeyDownPersistent");

            ui->scriptsWidget->evaluateOnKeyDownEventScripts();
        });
        connect(&m_sceneManager, &geometrize::scene::ImageTaskSceneManager::signal_onTargetImageKeyReleaseEvent, [this](int key, bool ctrlModifier) {
            chaiscript::ChaiScript* engine = m_task->getGeometrizer().getEngine();
            const std::string keyString = QKeySequence(key).toString().toStdString();

            // Update vars for pressed keys in the engine
            engine->set_global(chaiscript::var(""), "targetImageLastKeyDown");
            engine->set_global(chaiscript::var(false), "targetImageKeyDown_" + keyString);
            engine->set_global(chaiscript::var(ctrlModifier), "targetImageControlModifierDown");

            ui->scriptsWidget->evaluateOnKeyUpEventScripts();
        });

        // Pass the latest tablet event info to the current image task's script engine
        connect(&m_sceneManager, &geometrize::scene::ImageTaskSceneManager::signal_onTargetImageTabletEvent, [this](const geometrize::scene::CustomTabletEvent& event) {
            if(!m_task) {
                return;
            }

            const geometrize::scene::TabletEventData data = event.getData();
            chaiscript::ChaiScript* engine = m_task->getGeometrizer().getEngine();
            engine->set_global(chaiscript::var(data), "lastTabletEvent");

            ui->scriptsWidget->evaluateOnPenInputEventScripts();

            try {
                std::shared_ptr<geometrize::Shape> aoiShape = engine->eval<std::shared_ptr<geometrize::Shape>>("aoi");
                engine->set_global(chaiscript::var(aoiShape->clone()), "aoi"); // Work around fail to assign shape to shape in the script
                m_sceneManager.setAreaOfInfluenceShape(*aoiShape);
            } catch(...) {
                // The pen input scripts should setup an area of influence shape
            }
        });

        // Connect the global pen input proximity event filter signals
        // Note these are separate from the other tablet events, since they are application-wide
        // and proximity events don't seem to include useful data e.g. position etc
        auto& sharedTabletProximityEventFilter = geometrize::getSharedTabletProximityEventFilterInstance();
        connect(&sharedTabletProximityEventFilter, &geometrize::TabletProximityEventFilter::signal_onTabletEnterProximity, q, [this]() {
            ui->scriptsWidget->evaluateOnPenProximityEnterEventScripts();
        });
        connect(&sharedTabletProximityEventFilter, &geometrize::TabletProximityEventFilter::signal_onTabletLeaveProximity, q, [this]() {
            ui->scriptsWidget->evaluateOnPenProximityExitEventScripts();
        });

        /*
        // TODO do stuff with the area of influence shape, or tell the script engine, when input happens
        connect(&m_sceneManager, &geometrize::scene::ImageTaskSceneManager::signal_onAreaOfInfluenceShapeHoverMoveEvent, [this](const int lastX, const int lastY, const int x, const int y, const bool ctrlModifier) {
            if(!ctrlModifier) {
                return;
            }
            //translateShape(x - lastX, y - lastY);
        });
        connect(&m_sceneManager, &geometrize::scene::ImageTaskSceneManager::signal_onAreaOfInfluenceShapeMouseWheelEvent, [this](const int, const int, const double amount, const bool ctrlModifier) {
            if(!ctrlModifier) {
                return;
            }
            //scaleShape(amount > 0 ? 1.03f : 0.97f);
        });
        connect(&m_sceneManager, &geometrize::scene::ImageTaskSceneManager::signal_onAreaOfInfluenceShapeKeyPressEvent, [this](const int key, const bool) {
            if(key == Qt::Key_R) { // Rotate
                //rotateShape(3);
            }
            if(key == Qt::Key_Q) { // Scale down
                //scaleShape(0.97f);
            }
            if(key == Qt::Key_A) { // Scale up
                //scaleShape(1.03f);
            }
        });
        */

        // Set initial target image opacity
        const float initialTargetImageOpacity{0};
        ui->imageTaskImageWidget->setTargetImageOpacity(static_cast<unsigned int>(initialTargetImageOpacity));

        // Start the timer used to track how long the image task has been in the running state
        m_timeRunningTimer.start(static_cast<int>(m_timeRunningTimerResolutionMs));

        // Start the timer used to regularly update the associated image task's script engine (if any)
        m_scriptEngineUpdateTimer.start(static_cast<int>(m_scriptEngineUpdateTimerResolution));
    }
    ImageTaskWindowImpl& operator=(const ImageTaskWindowImpl&) = delete;
    ImageTaskWindowImpl(const ImageTaskWindowImpl&) = delete;
    ~ImageTaskWindowImpl()
    {
        // Sets the task in the UI etc to nothing (potentially autosaving stuff etc), then dispose of the task
        task::ImageTask* lastTask = getImageTask();
        setImageTask(nullptr);

        if(lastTask != nullptr) {
            destroyTask(lastTask);
        }
    }

    static std::vector<ImageTaskWindow*> getExistingImageTaskWindows()
    {
        std::vector<ImageTaskWindow*> windows;
        const QWidgetList allWidgets = QApplication::allWidgets();
        for (QWidget* widget : allWidgets) {
            if(ImageTaskWindow* w = dynamic_cast<ImageTaskWindow*>(widget)) {
                windows.push_back(w);
            }
        }
        return windows;
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
        if(path.isEmpty()) {
            return;
        }
        loadSettingsTemplate(m_task, path.toStdString());
    }

    void loadSettingsTemplate(task::ImageTask* task, const std::string& path)
    {
        if(task == nullptr) {
            return; // Can't load settings for a non-existent task
        }
        if(path.empty()) {
            assert(0 && "Auto load path for image task file must not be empty");
        }
        task->getPreferences().load(path);
        emit q->didLoadSettingsTemplate();
    }

    void saveSettingsTemplate() const
    {
        const QString path{common::ui::openSaveImageTaskSettingsDialog(q)};
        if(path.isEmpty()) {
            return;
        }
        saveSettingsTemplate(m_task, path.toStdString());
    }

    void saveSettingsTemplate(task::ImageTask* task, const std::string& path) const
    {
        if(task == nullptr) {
            return; // Can't save settings for a non-existent task
        }
        if(path.empty()) {
            assert(0 && "Auto save path for image task file must not be empty");
        }
        task->getPreferences().save(path);
        emit q->didSaveSettingsTemplate();
    }

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

    void handleCommand(const geometrize::script::Command& command)
    {
        const QString s = QString::fromStdString(command.getCommand());

        if(s == "start") {
            setShouldKeepStepping(true);
            stepModel();
        }
        if(s == "stop") {
            setShouldKeepStepping(false);
        }

        if(s == "click_start_stop_button") {
            ui->imageTaskRunnerWidget->runStopButtonClicked();
        }
        if(s == "click_step_button") {
            ui->imageTaskRunnerWidget->stepButtonClicked();
        }
        if(s == "click_clear_button") {
            ui->imageTaskRunnerWidget->clearButtonClicked();
        }

        if(s == "toggle_script_console") {
            ui->actionScript_Console->toggle();
        }
        if(s == "show_script_console") {
            ui->actionScript_Console->setChecked(true);
        }
        if(s == "hide_script_console") {
            ui->actionScript_Console->setChecked(false);
        }

        if(s == "toggle_pixmap_view") {
            ui->actionPixmap_Results_View->toggle();
        }
        if(s == "show_pixmap_view") {
            ui->actionPixmap_Results_View->setChecked(true);
        }
        if(s == "hide_pixmap_view") {
            ui->actionPixmap_Results_View->setChecked(false);
        }

        if(s == "toggle_vector_view") {
            ui->actionVector_Results_View->toggle();
        }
        if(s == "show_vector_view") {
            ui->actionVector_Results_View->setChecked(true);
        }
        if(s == "hide_vector_view") {
            ui->actionVector_Results_View->setChecked(false);
        }
    }

    void setCommandHandlerName(const std::string& name)
    {
        q->setObjectName(QString::fromStdString(name));
    }

    std::string getCommandHandlerName() const
    {
        return q->objectName().toStdString();
    }

private:
    void populateUi()
    {
        updateStartStopButtonText();
        q->setWindowTitle(geometrize::strings::Strings::getApplicationName());
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
        auto task = new geometrize::task::ImageTask(m_task->getDisplayName(), m_task->getTargetMutable());
        task->setPreferences(m_task->getPreferences());
        setImageTask(task);
    }

    void switchTargetImage(Bitmap& bitmap)
    {
        Bitmap& targetBitmap{m_task->getTargetMutable()};
        targetBitmap = bitmap;
    }

    void switchCurrentImage(Bitmap& bitmap)
    {
        Bitmap& currentBitmap{m_task->getCurrentMutable()};
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

    geometrize::scene::ImageTaskSceneManager m_sceneManager; ///> Manager for scenes containing the pixmap/vector-based representations of the shapes etc
    geometrize::scene::ImageTaskGraphicsView* m_pixmapView{nullptr}; ///> The view that holds the raster/pixel-based scene
    geometrize::scene::ImageTaskGraphicsView* m_svgView{nullptr}; ///> The view that holds the vector-based scene

    bool m_shouldKeepStepping{false}; ///> Whether to continually step i.e. whether to start another step after stepping once
    QTimer m_timeRunningTimer; ///> Timer used to keep track of how long the image task has been in the "running" state
    float m_timeRunning{0.0f}; ///> Total time that the image task has been in the "running" state
    const float m_timeRunningTimerResolutionMs{100.0f}; ///> Resolution of the time running timer in milliseconds

    QTimer m_scriptEngineUpdateTimer; ///> Timer used to call an update function on the script engine of the associated image task
    float m_scriptEngineUpdateTimerResolution{100.0f}; ///> Resolution of the script update timer in milliseconds
};

ImageTaskWindow::ImageTaskWindow() :
    QMainWindow{nullptr},
    d{std::make_unique<ImageTaskWindow::ImageTaskWindowImpl>(this)}
{
}

ImageTaskWindow::~ImageTaskWindow()
{
}

std::vector<ImageTaskWindow*> ImageTaskWindow::getExistingImageTaskWindows()
{
    return ImageTaskWindowImpl::getExistingImageTaskWindows();
}

void ImageTaskWindow::handleCommand(const geometrize::script::Command& command)
{
    d->handleCommand(command);
}

std::string ImageTaskWindow::getCommandHandlerName() const
{
    return d->getCommandHandlerName();
}

void ImageTaskWindow::setCommandHandlerName(const std::string& name)
{
    d->setCommandHandlerName(name);
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
