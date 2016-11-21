#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QPixmap>

#include "constants.h"
#include "forms/aboutdialog.h"
#include "forms/quitdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QImage imageForImagePath(const QString imagePath)
{
    return QImage(imagePath);
}

bool isImageValid(const QImage& image)
{
    return !image.isNull() && image.width() > 0 && image.height() > 0;
}

bool isPixmapValid(const QPixmap& pixmap)
{
    return !pixmap.isNull() && pixmap.width() > 0 && pixmap.height() > 0;
}

void MainWindow::on_imageJob_updated()
{
    qDebug("Image job updated");

    // TODO get the image job
    // TODO draw the new shapes onto the graphics view?
    // TODO check if the job should stop?
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionExit_triggered()
{
    exitApplication();
}

int MainWindow::exitApplication()
{
    QuitDialog dialog(this);
    dialog.exec();

    const int dialogResult = dialog.result();
    switch(dialogResult) {
        case QDialog::Accepted:
            // TODO save any outstanding stuff
            QApplication::quit();
    }

    return dialogResult;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    const int dialogResult = exitApplication();

    // TODO use the proper events
    if(dialogResult == QDialog::Accepted) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::on_actionTechnical_Support_triggered()
{
    QDesktopServices::openUrl(QUrl(TECHNICAL_SUPPORT_URL));
}

void MainWindow::on_actionOnline_Tutorials_triggered()
{
    QDesktopServices::openUrl(QUrl(VIDEO_TUTORIAL_URL));
}

void MainWindow::on_actionOpen_New_Image_triggered()
{
    QString imagePath{QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.jpg *.jpeg *.png *.bmp)"))};
    if(imagePath.length() == 0) {
        return; // User did not select an item
    }

    // TODO open svg, need to specify size after the fact
    // TODO separate this logic from the error boxes and this code path with signals

    QImage image{imageForImagePath(imagePath)};
    if(!isImageValid(image)) {
        QMessageBox::critical(this, tr("Image Loading Error"), tr("The image file could not be loaded."));
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(image);
    if(!isPixmapValid(pixmap)) {
        QMessageBox::critical(this, tr("Image Loading Error"), tr("Failed to convert image file."));
        return;
    }

    // TODO setup scene earlier
    if(ui->graphicsView->scene() == nullptr) {
        ui->graphicsView->setScene(new QGraphicsScene());
    }
    QGraphicsPixmapItem* pixmapItem{ui->graphicsView->scene()->addPixmap(pixmap)};
    pixmapItem->setPos(QPointF(150, 150)); // TODO center in scene
}

void MainWindow::on_actionSave_Geometrized_Image_triggered()
{
    // TODO save svg
    QString imagePath{QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("JPEG Image (*.jpg *.jpeg);;PNG Image(*.png);;BMP Image(*.bmp)"))};
    if(imagePath.length() == 0) {
        return;
    }

    // TODO save the file
}
