#include "imagejobwindow.h"
#include "ui_imagejobwindow.h"

#include <QCloseEvent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QPixmap>

#include "geometrize/core.h"

#include "common/uiactions.h"
#include "constants.h"
#include "dialog/aboutdialog.h"
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
    }
    ImageJobWindowImpl operator=(const ImageJobWindowImpl&) = delete;
    ImageJobWindowImpl(const ImageJobWindowImpl&) = delete;
    ~ImageJobWindowImpl() = default;

    int closeWindow()
    {
        const int dialogResult{common::ui::openQuitDialog(q)};
        switch(dialogResult) {
            case QDialog::Accepted:
                // TODO save any outstanding stuff(?) separate method needed
                q->close();
        }
        return dialogResult;
    }

    void setImageJob(job::ImageJob* job)
    {
        m_job = job;

        // TODO reset ui?

        updateWorkingImage();

        setDisplayName(QString::fromStdString(m_job->getDisplayName()));

        // TODO disconnect when setting new job
        connect(job, &job::ImageJob::signal_modelDidStep, [this](const std::vector<geometrize::ShapeResult>& shapes) {
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

    }

    void saveSettingsTemplate()
    {

    }

private:
    void setDisplayName(const QString& displayName)
    {
        q->setWindowTitle(tr("Geometrize - Image - %1").arg(displayName));
    }

    void updateWorkingImage()
    {
        // TODO replace item
        m_scene.clear();
        const QPixmap pixmap{image::createPixmap(m_job->getBitmap())};
        m_scene.addPixmap(pixmap); // TODO use a single pixmap?
    }

    job::ImageJob* m_job;
    ImageJobWindow* q;
    QGraphicsScene m_scene;
    std::unique_ptr<Ui::ImageJobWindow> ui;
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

void ImageJobWindow::closeEvent(QCloseEvent* event)
{
    const int dialogResult{common::ui::openQuitDialog(this)};
    if(dialogResult == QDialog::Accepted) {
        return;
    }

    QMainWindow::closeEvent(event);
}

void ImageJobWindow::on_actionExit_triggered()
{
    d->closeWindow();
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
