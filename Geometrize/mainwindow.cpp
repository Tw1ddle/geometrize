#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QPixmap>

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

void MainWindow::on_actionOpen_Image_triggered()
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

void MainWindow::on_actionSave_Image_triggered()
{
    // TODO save svg
    QString imagePath{QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("JPEG Image (*.jpg *.jpeg);;PNG Image(*.png);;BMP Image(*.bmp)"))};
    if(imagePath.length() == 0) {
        return;
    }

    // TODO save the file
}
