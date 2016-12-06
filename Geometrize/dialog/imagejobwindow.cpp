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

ImageJobWindow::ImageJobWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageJobWindow)
{
    ui->setupUi(this);
}

ImageJobWindow::~ImageJobWindow()
{
    delete ui;
}

void ImageJobWindow::on_actionAbout_triggered()
{
    SharedApp().openAboutPage(this);
}

void ImageJobWindow::on_actionExit_triggered()
{
    exitApplication();
}

int ImageJobWindow::exitApplication()
{
    const int dialogResult{SharedApp().openQuitDialog(this)};
    switch(dialogResult) {
        case QDialog::Accepted:
            // TODO save any outstanding stuff(?) separate method needed
            QApplication::quit();
    }
    return dialogResult;
}

void ImageJobWindow::closeEvent(QCloseEvent* event)
{
    const int dialogResult{SharedApp().openQuitDialog(this)};
    if(dialogResult == QDialog::Accepted) {
        event->accept();
    } else {
        event->ignore();
    }
}

void ImageJobWindow::on_actionTechnical_Support_triggered()
{
    SharedApp().openTechnicalSupport();
}

void ImageJobWindow::on_actionOnline_Tutorials_triggered()
{
    SharedApp().openOnlineTutorials();
}

void ImageJobWindow::on_actionOpen_New_Image_triggered()
{
    SharedApp().openImage(ui->imageView->scene(), this);
}

void ImageJobWindow::on_actionSave_Geometrized_Image_triggered()
{
    // TODO save svg?
    QString imagePath{QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("JPEG Image (*.jpg *.jpeg);;PNG Image(*.png);;BMP Image(*.bmp)"))};
    if(imagePath.length() == 0) {
        return;
    }

    SharedApp().saveImage(this); // TODO actually save it
}

void ImageJobWindow::on_actionOpenPreferences_triggered()
{
    SharedApp().openPreferences(this);
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
