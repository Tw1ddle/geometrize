#include "imagejobwindow.h"
#include "ui_imagejobwindow.h"

#include <cassert>

#include <QMessageBox>
#include <QPixmap>
#include <QTimer>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/runner/imagerunneroptions.h"

#include "common/uiactions.h"
#include "common/util.h"
#include "dialog/imagejobgraphicsview.h"
#include "dialog/imagejobpixmapscene.h"
#include "dialog/imagejobscriptingpanel.h"
#include "dialog/imagejobsvgscene.h"
#include "dialog/imagejobtargetimagewidget.h"
#include "dialog/scripteditorwidget.h"
#include "image/imageloader.h"
#include "job/imagejob.h"

namespace geometrize
{

namespace dialog
{

// Utility function for destroying an image job set on a window.
// This is a special case because we may need to defer job deletion until the job is finished working.
void destroyJob(job::ImageJob* job)
{
    if(job == nullptr) {
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
        q{pQ},
        m_currentImageView{nullptr},
        m_svgImageView{nullptr},
        m_job{nullptr},
        m_jobWillStepConnection{},
        m_jobDidStepConnection{},
        m_running{false},
        m_timeRunning{0.0f},
        m_timeRunningResolutionMs{100.0f}
    {
        q->setAttribute(Qt::WA_DeleteOnClose);
        ui->setupUi(q);

        q->tabifyDockWidget(ui->runnerSettingsDock, ui->exporterDock);
        q->tabifyDockWidget(ui->runnerSettingsDock, ui->targetImageSettingsDock);
        ui->runnerSettingsDock->raise(); // Make sure runner settings dock is selected

        ui->consoleWidget->setVisible(false); // Make sure console widget is hidden by default

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

        // Used to track how long the job has been in the running state
        connect(&m_timeRunningTimer, &QTimer::timeout, [this]() {
            if(m_running) {
                m_timeRunning += m_timeRunningResolutionMs;
                updateStats();
            }
        });
        m_timeRunningTimer.start(m_timeRunningResolutionMs);

        connect(ui->imageJobTargetImageWidget, &ImageJobTargetImageWidget::targetImageOpacityChanged, [this](const unsigned int value) {
            const float opacity{value * (1.0f / 255.0f)};
            m_currentImageScene.setTargetPixmapOpacity(opacity);
            m_currentSvgScene.setTargetPixmapOpacity(opacity);
        });
        const float initialTargetImageOpacity{10};
        ui->imageJobTargetImageWidget->setTargetImageOpacity(initialTargetImageOpacity);

        connect(ui->imageJobTargetImageWidget, &ImageJobTargetImageWidget::targetImageSelected, [this](const QImage& image) {
            assert(!image.isNull());

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

            ui->imageJobTargetImageWidget->setTargetImage(image);
        });

        connect(ui->imageJobTargetImageWidget, &ImageJobTargetImageWidget::targetImageSet, [this](const QImage& image) {
            // TODO throw away current job, clone and continue? or possibly defer until next step *if* already running
        });

        connect(ui->imageJobRunnerWidget, &ImageJobRunnerWidget::runStopButtonClicked, [this]() {
            toggleRunning();
        });
        connect(ui->imageJobRunnerWidget, &ImageJobRunnerWidget::stepButtonClicked, [this]() {
            stepModel();
        });
        connect(ui->imageJobRunnerWidget, &ImageJobRunnerWidget::clearButtonClicked, [this]() {
            clearModel();
        });

        connect(q, &ImageJobWindow::willSetImageJob, [this](job::ImageJob* lastJob, job::ImageJob*) {
            // NOTE we disconnect and destroy the last image job, which will soon be replaced by the next image job
            // This means that any job set on the window is destroyed after it is replaced
            disconnectJob();
            destroyJob(lastJob);

            m_shapes.clear();
        });

        connect(q, &ImageJobWindow::didSetImageJob, [this](job::ImageJob*, job::ImageJob* currentJob) {
            ui->imageJobExportWidget->setImageJob(currentJob, &m_shapes);
            ui->imageJobRunnerWidget->setImageJob(currentJob);

            m_jobWillStepConnection = connect(currentJob, &job::ImageJob::signal_modelWillStep, [this]() {
                ui->statsDockContents->setCurrentStatus(geometrize::dialog::ImageJobStatsWidget::RUNNING);
            });

            m_jobDidStepConnection = connect(currentJob, &job::ImageJob::signal_modelDidStep, [this](std::vector<geometrize::ShapeResult> shapes) {
                std::copy(shapes.begin(), shapes.end(), std::back_inserter(m_shapes));

                updateCurrentGraphics(shapes);
                updateStats();

                if(m_running) {
                    stepModel();
                }
            });

            q->setWindowTitle(tr("Geometrize - Image - %1").arg(QString::fromStdString(currentJob->getDisplayName())));

            setupOverlayImages();
            updateCurrentGraphics(m_shapes);
            currentJob->drawBackgroundRectangle();

            ui->consoleWidget->setEngine(currentJob->getEngine());
            ui->imageJobRunnerWidget->syncUserInterface();
            ui->imageJobTargetImageWidget->setTargetImage(image::createImage(currentJob->getTarget()));

            m_timeRunning = 0.0f;
        });

        connect(q, &ImageJobWindow::didLoadSettingsTemplate, [this]() {
            ui->imageJobRunnerWidget->syncUserInterface();
        });
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
        q->willSetImageJob(lastJob, nextJob);
        m_job = job;
        q->didSetImageJob(lastJob, nextJob);
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
        ui->consoleWidget->setVisible(visible);
    }

    void setPixmapViewVisibility(const bool visible)
    {
        m_currentImageView->setVisible(visible);
    }

    void setVectorViewVisibility(const bool visible)
    {
        m_svgImageView->setVisible(visible);
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

    void toggleRunning()
    {
        m_running = !m_running;

        if(!m_running) {
            ui->imageJobRunnerWidget->setRunStopButtonText(tr("Start"));
        } else {
            stepModel();
            ui->imageJobRunnerWidget->setRunStopButtonText(tr("Stop"));
        }
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

    void updateStats()
    {
        if(!m_running) {
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

    std::unique_ptr<Ui::ImageJobWindow> ui;
    ImageJobWindow* q;

    job::ImageJob* m_job;
    QMetaObject::Connection m_jobWillStepConnection;
    QMetaObject::Connection m_jobDidStepConnection;
    std::vector<geometrize::ShapeResult> m_shapes;

    ImageJobPixmapScene m_currentImageScene;
    ImageJobSvgScene m_currentSvgScene;
    geometrize::dialog::ImageJobGraphicsView* m_currentImageView;
    geometrize::dialog::ImageJobGraphicsView* m_svgImageView;



    bool m_running; ///> Whether the model is running (automatically)
    QTimer m_timeRunningTimer; ///> Timer used to keep track of how long the image job has been in the "running" state
    float m_timeRunning; ///> Total time that the image job has been in the "running" state
    const float m_timeRunningResolutionMs; ///> Resolution of the time running timer
};

ImageJobWindow::ImageJobWindow() :
    QMainWindow(nullptr),
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

void ImageJobWindow::on_actionScript_Console_toggled(const bool checked)
{
    d->setConsoleVisibility(checked);
}

void ImageJobWindow::on_actionPixmap_Results_View_toggled(const bool checked)
{
    d->setPixmapViewVisibility(checked);
}

void ImageJobWindow::on_actionVector_Results_View_toggled(const bool checked)
{
    d->setVectorViewVisibility(checked);
}

}

}
