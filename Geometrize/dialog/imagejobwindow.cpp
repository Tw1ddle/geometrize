#include "imagejobwindow.h"
#include "ui_imagejobwindow.h"

#include <QCloseEvent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QPixmap>

#include "constants.h"
#include "dialog/aboutdialog.h"
#include "dialog/preferencestabdialog.h"
#include "dialog/quitdialog.h"
#include "sharedapp.h"

namespace geometrize
{

namespace dialog
{

class ImageJobWindow::ImageJobWindowImpl
{
public:
    ImageJobWindowImpl(ImageJobWindow* pQ, Ui::ImageJobWindow* pUi) : q{pQ}, ui{pUi}
    {
        ui->setupUi(q);
    }
    ImageJobWindowImpl operator=(const ImageJobWindowImpl&) = delete;
    ImageJobWindowImpl(const ImageJobWindowImpl&) = delete;
    ~ImageJobWindowImpl() = default;

private:
    ImageJobWindow* q;
    Ui::ImageJobWindow* ui;
};

ImageJobWindow::ImageJobWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageJobWindow),
    d{std::make_unique<ImageJobWindowImpl>(this, ui)}
{
    // TODO set title to name of image loaded?
}

ImageJobWindow::~ImageJobWindow()
{
    delete ui;
}

void ImageJobWindow::on_actionAbout_triggered()
{
    SharedApp::get().openAboutPage(this);
}

void ImageJobWindow::on_actionExit_triggered()
{
    closeWindow();
}

int ImageJobWindow::closeWindow()
{
    const int dialogResult{SharedApp::get().openQuitDialog(this)};
    switch(dialogResult) {
        case QDialog::Accepted:
            // TODO save any outstanding stuff(?) separate method needed
            close();
    }
    return dialogResult;
}

void ImageJobWindow::closeEvent(QCloseEvent* event)
{
    const int dialogResult{SharedApp::get().openQuitDialog(this)};
    if(dialogResult == QDialog::Accepted) {
        return;
    }

    QMainWindow::closeEvent(event);
}

void ImageJobWindow::on_actionTechnical_Support_triggered()
{
    SharedApp::openTechnicalSupport();
}

void ImageJobWindow::on_actionOnline_Tutorials_triggered()
{
    SharedApp::openOnlineTutorials();
}

void ImageJobWindow::on_actionOpenPreferences_triggered()
{
    SharedApp::openPreferences(this);
}

void ImageJobWindow::on_imageJob_updated()
{
    qDebug("Image job updated");

    // TODO get the image job
    // TODO draw the new shapes onto the graphics view?
    // TODO check if the job should stop?
}

void ImageJobWindow::on_runStopButton_clicked()
{
    // TODO check settings are valid
}

}

}
