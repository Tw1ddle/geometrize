#include "imagejobwindow.h"
#include "ui_imagejobwindow.h"

#include <QCloseEvent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QPixmap>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/core.h"

#include "common/uiactions.h"
#include "constants.h"
#include "dialog/aboutdialog.h"
#include "dialog/collapsiblesection.h"
#include "dialog/globalpreferencestabdialog.h"
#include "dialog/quitdialog.h"
#include "image/imageloader.h"
#include "job/imagejob.h"

namespace geometrize
{

namespace dialog
{

class ImageJobWindow::ImageJobWindowImpl
{
public:
    ImageJobWindowImpl(ImageJobWindow* pQ) : ui{std::make_unique<Ui::ImageJobWindow>()}, q{pQ}, m_job{nullptr}
    {
        ui->setupUi(q);

        ui->imageView->setScene(&m_scene);
        m_scene.addItem(&m_currentPixmapItem);
        m_scene.addItem(&m_targetPixmapItem);

        connect(ui->targetOpacitySlider, &QSlider::valueChanged, [this](int value) {
            m_targetPixmapItem.setOpacity(value * 0.01f);
        });

        const float startingTargetOpacity{10};
        ui->targetOpacitySlider->setValue(startingTargetOpacity);
    }
    ImageJobWindowImpl operator=(const ImageJobWindowImpl&) = delete;
    ImageJobWindowImpl(const ImageJobWindowImpl&) = delete;
    ~ImageJobWindowImpl() = default;

    void close()
    {
        q->close();
    }

    void setImageJob(job::ImageJob* job)
    {
        m_job = job;

        setupOverlayImages();
        updateWorkingImage();

        setDisplayName(QString::fromStdString(m_job->getDisplayName()));

        // TODO disconnect when setting new job
        connect(job, &job::ImageJob::signal_modelDidStep, [this](std::vector<geometrize::ShapeResult> shapes) {
            updateWorkingImage();
        });
    }

    void toggleModelRunning()
    {
    }

    void stepModel()
    {
        m_job->stepModel();
    }

    void revealLaunchWindow()
    {
        if(common::ui::isLaunchWindowOpen()) {
            common::ui::bringLaunchWindowToFront();
        } else {
            common::ui::openLaunchWindow();
        }
    }

    void resetJob()
    {

    }

    void loadSettingsTemplate()
    {
        const QString path{common::ui::openLoadImageJobSettingsDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        m_job->getPreferences().load(path.toStdString());
    }

    void saveSettingsTemplate()
    {
        const QString path{common::ui::openSaveImageJobSettingsDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        m_job->getPreferences().save(path.toStdString());
    }

private:
    void setDisplayName(const QString& displayName)
    {
        q->setWindowTitle(tr("Geometrize - Image - %1").arg(displayName));
    }

    void updateWorkingImage()
    {
        const QPixmap pixmap{image::createPixmap(m_job->getCurrent())};
        m_currentPixmapItem.setPixmap(pixmap);
    }

    void setupOverlayImages()
    {
        const QPixmap target{image::createPixmap(m_job->getTarget())};
        m_targetPixmapItem.setPixmap(target);
    }

    job::ImageJob* m_job;
    ImageJobWindow* q;
    QGraphicsScene m_scene;
    std::unique_ptr<Ui::ImageJobWindow> ui;

    QGraphicsPixmapItem m_targetPixmapItem;
    QGraphicsPixmapItem m_currentPixmapItem;
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

void ImageJobWindow::on_runStopButton_clicked()
{
    d->toggleModelRunning();
}

void ImageJobWindow::on_stepButton_clicked()
{
    d->stepModel();
}

void ImageJobWindow::on_resetButton_clicked()
{
    d->resetJob();
}

}

}
