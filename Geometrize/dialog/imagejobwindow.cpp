#include "imagejobwindow.h"
#include "ui_imagejobwindow.h"

#include <QCloseEvent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QPixmap>

#include "geometrize/bitmap/bitmapdata.h"

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
    ImageJobWindowImpl(ImageJobWindow* pQ) : q{pQ}, m_job{nullptr}, ui{std::make_unique<Ui::ImageJobWindow>()}
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

        const QPixmap pixmap{image::createPixmap(m_job->getBitmapData())};
        m_scene.addPixmap(pixmap); // TODO use a single pixmap?
    }

private:
    job::ImageJob* m_job;
    ImageJobWindow* q;
    std::unique_ptr<Ui::ImageJobWindow> ui;
    QGraphicsScene m_scene;
};

ImageJobWindow::ImageJobWindow(QWidget* parent) :
    QMainWindow(parent),
    d{std::make_unique<ImageJobWindowImpl>(this)}
{
}

ImageJobWindow::~ImageJobWindow()
{
}

void ImageJobWindow::setImageJob(job::ImageJob* job)
{
    d->setImageJob(job);
}

void ImageJobWindow::on_actionAbout_triggered()
{
    common::ui::openAboutPage(this);
}

void ImageJobWindow::on_actionExit_triggered()
{
    d->closeWindow();
}

void ImageJobWindow::closeEvent(QCloseEvent* event)
{
    const int dialogResult{common::ui::openQuitDialog(this)};
    if(dialogResult == QDialog::Accepted) {
        return;
    }

    QMainWindow::closeEvent(event);
}

void ImageJobWindow::on_actionTechnical_Support_triggered()
{
    common::ui::openTechnicalSupport();
}

void ImageJobWindow::on_actionOnline_Tutorials_triggered()
{
    common::ui::openOnlineTutorials();
}

void ImageJobWindow::on_actionOpenPreferences_triggered()
{
    common::ui::openPreferences(this);
}

void ImageJobWindow::on_runStopButton_clicked()
{
    // TODO check settings are valid
}

}

}
