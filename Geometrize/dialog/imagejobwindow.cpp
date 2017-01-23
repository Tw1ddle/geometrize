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
    ImageJobWindowImpl(ImageJobWindow* pQ, Ui::ImageJobWindow* pUi) : q{pQ}, m_job{nullptr}, ui{pUi}
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

    void setDisplayName(const QString& displayName)
    {
        q->setWindowTitle(tr("Geometrize - Image - %1").arg(displayName));
    }

    void runStopModel()
    {

    }

    void stepModel()
    {

    }

private:
    job::ImageJob* m_job;
    ImageJobWindow* q;
    Ui::ImageJobWindow* ui;
    QGraphicsScene m_scene;
};

ImageJobWindow::ImageJobWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::ImageJobWindow),
    d{std::make_unique<ImageJobWindowImpl>(this, ui)}
{
}

ImageJobWindow::~ImageJobWindow()
{
    delete ui;
}

void ImageJobWindow::setImageJob(job::ImageJob* job)
{
    d->setImageJob(job);
}

void ImageJobWindow::setDisplayName(const QString& displayName)
{
    d->setDisplayName(displayName);
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

void ImageJobWindow::on_runStopButton_clicked()
{
    d->runStopModel();
}

void ImageJobWindow::on_stepButton_clicked()
{
    d->stepModel();
}

void ImageJobWindow::on_actionSave_Settings_Template_triggered()
{

}

void ImageJobWindow::on_actionReveal_Launch_Window_triggered()
{

}

}

}
