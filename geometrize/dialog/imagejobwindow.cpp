#include "imagejobwindow.h"
#include "ui_imagejobwindow.h"

#include <cassert>
#include <functional>
#include <vector>

#include <QMarginsF>
#include <QMessageBox>
#include <QPixmap>
#include <QRectF>
#include <QTimer>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/runner/imagerunneroptions.h"

#include "common/uiactions.h"
#include "common/util.h"
#include "dialog/imagejobgraphicsview.h"
#include "dialog/imagejobpixmapscene.h"
#include "dialog/imagejobscriptingpanel.h"
#include "dialog/imagejobsvgscene.h"
#include "dialog/imagejobimagewidget.h"
#include "dialog/scripteditorwidget.h"
#include "image/imageloader.h"
#include "job/imagejob.h"
#include "localization/strings.h"
#include "preferences/globalpreferences.h"

namespace geometrize
{

namespace dialog
{

// Utility function for destroying an image job set on a window.
// This is a special case because we may need to defer job deletion until the job is finished working.
void destroyJob(job::ImageJob* job)
{
    if(job == nullptr) {
        assert(0 && "Attempted to destroy an image job that was already null");
        return;
    }

    if(job->isStepping()) {
        // Wait until the job finishes stepping before disposing of it
        // Otherwise it will probably crash as the Geometrize library will be working with deleted data
        job->connect(job, &job::ImageJob::signal_modelDidStep, [job](std::vector<geometrize::ShapeResult>) {
            job->deleteLater();
        });
    } else {
        delete job;
        job = nullptr;
    }
}

class ImageJobWindow::ImageJobWindowImpl
{
public:
    ImageJobWindowImpl(ImageJobWindow* pQ) :
        ui{std::make_unique<Ui::ImageJobWindow>()},
        q{pQ}
    {
        q->setAttribute(Qt::WA_DeleteOnClose);
        ui->setupUi(q);
        q->setWindowTitle(geometrize::strings::Strings::getApplicationName());

        // Set up the dock widgets
        q->tabifyDockWidget(ui->runnerSettingsDock, ui->exporterDock);
        ui->runnerSettingsDock->raise(); // Make sure runner settings dock is selected

        ui->consoleWidget->setVisible(false); // Make sure console widget is hidden by default

        // Set up the image job geometrization views
        m_currentImageView = new geometrize::dialog::ImageJobGraphicsView(ui->imageViewContainer);
        m_currentImageView->setScene(&m_currentImageScene);
        m_currentImageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_currentImageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->imageViewContainer->layout()->addWidget(m_currentImageView);

        m_svgImageView = new geometrize::dialog::ImageJobGraphicsView(ui->imageViewContainer);
        m_svgImageView->setScene(&m_currentSvgScene);
        m_svgImageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_svgImageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->imageViewContainer->layout()->addWidget(m_svgImageView);

        m_currentImageView->setVisible(false); // Make sure the image view is hidden by default (we prefer the SVG view)
        m_svgImageView->setVisible(true);

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

        // Handle request to set the image job on the job window
        connect(q, &ImageJobWindow::willSwitchImageJob, [this](job::ImageJob* lastJob, job::ImageJob*) {
            // NOTE we disconnect and destroy the last image job, which will soon be replaced by the next image job
            // This means that any job set on the window is destroyed after it is replaced
            disconnectJob();

            if(lastJob) {
                destroyJob(lastJob);
            }

            m_shapes.clear();
        });
        connect(q, &ImageJobWindow::didSwitchImageJob, [this](job::ImageJob*, job::ImageJob* currentJob) {
            ui->imageJobExportWidget->setImageJob(currentJob, &m_shapes);
            ui->imageJobRunnerWidget->setImageJob(currentJob);

            m_jobWillStepConnection = connect(currentJob, &job::ImageJob::signal_modelWillStep, [this]() {
                ui->statsDockContents->setCurrentStatus(geometrize::dialog::ImageJobStatsWidget::RUNNING);
            });

            m_jobDidStepConnection = connect(currentJob, &job::ImageJob::signal_modelDidStep, [this](std::vector<geometrize::ShapeResult> shapes) {
                processPostStepCbs();

                updateCurrentGraphics(shapes);
                // If the first shape added background rectangle then fit the scenes to it
                if(m_shapes.size() == 0) {
                    fitScenesInViews();
                }
                std::copy(shapes.begin(), shapes.end(), std::back_inserter(m_shapes));

                updateStats();

                if(isRunning()) {
                    stepModel();
                }
            });

            q->setWindowTitle(tr("Geometrize - Image - %1").arg(QString::fromStdString(currentJob->getDisplayName())));

            setupOverlayImages();
            currentJob->drawBackgroundRectangle();

            ui->consoleWidget->setEngine(currentJob->getEngine());
            ui->imageJobRunnerWidget->syncUserInterface();

            ui->imageJobImageWidget->setTargetImage(image::createImage(currentJob->getTarget()));

            m_timeRunning = 0.0f;
        });

        // Handle requested target image overlay opacity changes
        connect(ui->imageJobImageWidget, &ImageJobImageWidget::targetImageOpacityChanged, [this](const unsigned int value) {
            const float opacity{value * (1.0f / 255.0f)};
            m_currentImageScene.setTargetPixmapOpacity(opacity);
            m_currentSvgScene.setTargetPixmapOpacity(opacity);
        });

        // Handle a request to change the target image
        connect(ui->imageJobImageWidget, &ImageJobImageWidget::targetImageSelected, [this](const QImage& image) {
            assert(!image.isNull());

            // Validate the target image size
            const geometrize::Bitmap& target{m_job->getTarget()};
            const auto targetWidth{target.getWidth()};
            const auto targetHeight{target.getHeight()};
            if(targetWidth != image.width() || targetHeight != image.height()) {
                const QString selectedImageSize(tr("%1x%2").arg(image.width()).arg(image.height()));
                const QString targetImageSize(tr("%1x%2").arg(targetWidth).arg(targetHeight));
                QMessageBox::warning(
                            q,
                            tr("Failed to set target image"),
                            tr("Selected image must have the same dimensions as the current target image. Size was %1, but should have been %2").arg(selectedImageSize).arg(targetImageSize));

                return;
            }

            setTargetImage(image);
        });

        // Handle a request to change the target image that has passed size checks and validation
        connect(ui->imageJobImageWidget, &ImageJobImageWidget::targetImageSet, [this](const QImage& image) {
            // If the job is running then defer the target image change to the next step, else do it immediately
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

        // Handle a new set of image job-specific settings being loaded
        connect(q, &ImageJobWindow::didLoadSettingsTemplate, [this]() {
            ui->imageJobRunnerWidget->syncUserInterface();
        });

        // Handle runner button presses
        connect(ui->imageJobRunnerWidget, &ImageJobRunnerWidget::runStopButtonClicked, [this]() {
            setRunning(!isRunning());

            // Toggle running button text and request another image job step if running started
            if(!isRunning()) {
                ui->imageJobRunnerWidget->setRunStopButtonText(tr("Start"));
            } else {
                stepModel();
                ui->imageJobRunnerWidget->setRunStopButtonText(tr("Stop"));
            }
        });
        connect(ui->imageJobRunnerWidget, &ImageJobRunnerWidget::stepButtonClicked, [this]() {
            stepModel();
        });
        connect(ui->imageJobRunnerWidget, &ImageJobRunnerWidget::clearButtonClicked, [this]() {
            clearModel();
        });

        // Track how long the job has been in the running state
        connect(&m_timeRunningTimer, &QTimer::timeout, [this]() {
            if(isRunning()) {
                m_timeRunning += m_timeRunningResolutionMs;
                updateStats();
            }
        });

        // Start the timer used to track how long the image job has been in the running state
        m_timeRunningTimer.start(m_timeRunningResolutionMs);

        // Set initial target image opacity
        const float initialTargetImageOpacity{10};
        ui->imageJobImageWidget->setTargetImageOpacity(initialTargetImageOpacity);

        // Set initial view visibility
        const geometrize::preferences::GlobalPreferences& prefs{geometrize::preferences::getGlobalPreferences()};
        setConsoleVisibility(prefs.shouldShowImageJobConsoleByDefault());
        setPixmapViewVisibility(prefs.shouldShowImageJobPixmapViewByDefault());
        setVectorViewVisibility(prefs.shouldShowImageJobVectorViewByDefault());
        if(prefs.shouldShowImageJobScriptEditorByDefault()) {
            revealScriptEditor();
        }
    }
    ImageJobWindowImpl operator=(const ImageJobWindowImpl&) = delete;
    ImageJobWindowImpl(const ImageJobWindowImpl&) = delete;
    ~ImageJobWindowImpl()
    {
        disconnectJob();
        destroyJob(m_job);
    }

    void close()
    {
        q->close();
    }

    void setImageJob(job::ImageJob* job)
    {
        job::ImageJob* lastJob{m_job};
        job::ImageJob* nextJob{job};
        q->willSwitchImageJob(lastJob, nextJob);
        m_job = nextJob;
        q->didSwitchImageJob(lastJob, nextJob);
    }

    void revealLaunchWindow()
    {
        if(common::ui::isLaunchWindowOpen()) {
            common::ui::bringLaunchWindowToFront();
        } else {
            common::ui::openLaunchWindow();
        }
    }

    void revealScriptEditor()
    {
        auto existingPanel = q->findChild<geometrize::dialog::ImageJobScriptingPanel*>();
        if(existingPanel) {
            existingPanel->setWindowState(existingPanel->windowState() & ~Qt::WindowMinimized);
            QApplication::setActiveWindow(existingPanel);
            existingPanel->raise();
            return;
        }
        auto scriptingPanel = new geometrize::dialog::ImageJobScriptingPanel(q);
        scriptingPanel->show();
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

        m_currentImageView->setVisible(visible);
        if(visible) {
            fitImageSceneInView();
        }
    }

    void setVectorViewVisibility(const bool visible)
    {
        if(ui->actionVector_Results_View->isChecked() != visible) {
            ui->actionVector_Results_View->setChecked(visible);
        }

        m_svgImageView->setVisible(visible);
        if(visible) {
            fitVectorSceneInView();
        }
    }

    void loadSettingsTemplate()
    {
        const QString path{common::ui::openLoadImageJobSettingsDialog(q)};
        if(path.isEmpty()) {
            return;
        }
        m_job->getPreferences().load(path.toStdString());

        emit q->didLoadSettingsTemplate();
    }

    void saveSettingsTemplate() const
    {
        const QString path{common::ui::openSaveImageJobSettingsDialog(q)};
        if(path.isEmpty()) {
            return;
        }
        m_job->getPreferences().save(path.toStdString());

        emit q->didSaveSettingsTemplate();
    }

private:
    void updateCurrentGraphics(const std::vector<geometrize::ShapeResult>& shapes)
    {
        const QPixmap pixmap{image::createPixmap(m_job->getCurrent())};
        m_currentImageScene.setWorkingPixmap(pixmap);
        m_currentSvgScene.drawSvg(shapes, pixmap.size().width(), pixmap.size().height());
    }

    bool isRunning() const
    {
        return m_running; // TODO this is buggy, need to synch up whether the job is running or not in a better way
    }

    void setRunning(const bool running)
    {
        m_running = running;
    }

    void stepModel()
    {
        m_job->stepModel();
    }

    void clearModel()
    {
        auto job = new geometrize::job::ImageJob(m_job->getDisplayName(), m_job->getTarget());
        job->setPreferences(m_job->getPreferences());
        setImageJob(job);
    }

    void switchTargetImage(Bitmap& bitmap)
    {
        Bitmap& targetBitmap{m_job->getTarget()};
        targetBitmap = bitmap;
    }

    void switchCurrentImage(Bitmap& bitmap)
    {
        Bitmap& currentBitmap{m_job->getCurrent()};
        currentBitmap = bitmap;
    }

    void updateStats()
    {
        ui->statsDockContents->setJobId(m_job->getJobId());
        ui->statsDockContents->setImageDimensions(m_job->getWidth(), m_job->getHeight());

        if(!isRunning()) {
            ui->statsDockContents->setCurrentStatus(geometrize::dialog::ImageJobStatsWidget::STOPPED);
        }

        ui->statsDockContents->setShapeCount(m_shapes.size());

        if(!m_shapes.empty()) {
            ui->statsDockContents->setSimilarity(m_shapes.back().score * 100.0f);
        }

        ui->statsDockContents->setTimeRunning(m_timeRunning);
    }

    void disconnectJob()
    {
        if(m_jobWillStepConnection) {
            disconnect(m_jobWillStepConnection);
        }
        if(m_jobDidStepConnection) {
            disconnect(m_jobDidStepConnection);
        }
    }

    void setupOverlayImages()
    {
        const QPixmap target{image::createPixmap(m_job->getTarget())};
        m_currentImageScene.setTargetPixmap(target);
        m_currentSvgScene.setTargetPixmap(target);
    }

    void fitImageSceneInView()
    {
        const float margin{m_defaultViewMargins};
        const QRectF imageViewRect{m_currentImageScene.itemsBoundingRect().marginsAdded(QMarginsF(margin, margin, margin, margin))};
        m_currentImageView->fitInView(imageViewRect, Qt::KeepAspectRatio);
    }

    void fitVectorSceneInView()
    {
        const float margin{m_defaultViewMargins};
        const QRectF svgRect{m_currentSvgScene.itemsBoundingRect().marginsAdded(QMarginsF(margin, margin, margin, margin))};
        m_svgImageView->fitInView(svgRect, Qt::KeepAspectRatio);
    }

    void fitScenesInViews()
    {
        fitImageSceneInView();
        fitVectorSceneInView();
    }

    void setTargetImage(const QImage& image)
    {
        ui->imageJobImageWidget->setTargetImage(image);
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

    std::unique_ptr<Ui::ImageJobWindow> ui{nullptr};
    ImageJobWindow* q{nullptr};

    job::ImageJob* m_job{nullptr}; ///> The image job currently set and manipulated via this window
    QMetaObject::Connection m_jobWillStepConnection{}; ///> Connection for the window to do work just prior the image job starts a step
    QMetaObject::Connection m_jobDidStepConnection{}; ///> Connection for the window to do work just after the image job finishes a step
    std::vector<std::function<void()>> m_onPostStepCbs; ///> One-shot callbacks triggered when the image job finishes a step

    std::vector<geometrize::ShapeResult> m_shapes; ///> The shapes and score results created by the image job

    ImageJobPixmapScene m_currentImageScene; ///> The scene containing the raster/pixel-based representation of the shapes
    ImageJobSvgScene m_currentSvgScene; ///> The scene containing the vector-based representation of the shapes
    const float m_defaultViewMargins{20.0f}; ///> Margins around the graphics shown in the views
    geometrize::dialog::ImageJobGraphicsView* m_currentImageView{nullptr}; ///> The view that holds the raster/pixel-based scene
    geometrize::dialog::ImageJobGraphicsView* m_svgImageView{nullptr}; ///> The view that holds the vector-based scene

    bool m_running{false}; ///> Whether the model is running (automatically)
    QTimer m_timeRunningTimer; ///> Timer used to keep track of how long the image job has been in the "running" state
    float m_timeRunning{0.0f}; ///> Total time that the image job has been in the "running" state
    const float m_timeRunningResolutionMs{100.0f}; ///> Resolution of the time running timer
};

ImageJobWindow::ImageJobWindow() :
    QMainWindow{nullptr},
    d{std::make_unique<ImageJobWindow::ImageJobWindowImpl>(this)}
{
}

ImageJobWindow::~ImageJobWindow()
{
}

void ImageJobWindow::setImageJob(job::ImageJob* job)
{
    d->setImageJob(job);
}

void ImageJobWindow::on_actionExit_triggered()
{
    d->close();
}

void ImageJobWindow::on_actionLoad_Settings_Template_triggered()
{
    d->loadSettingsTemplate();
}

void ImageJobWindow::on_actionSave_Settings_Template_triggered()
{
    d->saveSettingsTemplate();
}

void ImageJobWindow::on_actionReveal_Launch_Window_triggered()
{
    d->revealLaunchWindow();
}

void ImageJobWindow::on_actionReveal_Script_Editor_triggered()
{
    d->revealScriptEditor();
}

}

}
