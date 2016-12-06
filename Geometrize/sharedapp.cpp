#include "sharedapp.h"

#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QImage>
#include <QMessageBox>
#include <QPixmap>
#include <QtGlobal>
#include <QUrl>

#include "lib/geometrizer/geometrizer/bitmap/bitmapdata.h"
#include "lib/geometrizer/geometrizer/bitmap/rgba.h"

#include "constants.h"
#include "dialog/aboutdialog.h"
#include "dialog/imagejobwindow.h"
#include "dialog/launchwindow.h"
#include "dialog/preferencestabdialog.h"
#include "dialog/quitdialog.h"
#include "imagejobcontext.h"

namespace geometrize
{

/**
 * @brief The SharedAppImpl class contains the concrete implementation of the SharedApp class.
 */
class SharedAppImpl : public QObject
{
public:
    SharedAppImpl() = default;
    SharedAppImpl& operator=(const SharedAppImpl&) = delete;
    SharedAppImpl(const SharedAppImpl&) = delete;
    ~SharedAppImpl() = default;

    static void openAboutPage(QWidget* parent)
    {
        dialog::AboutDialog dialog(parent);
        dialog.exec();
    }

    static void openPreferences(QWidget* parent)
    {
        dialog::PreferencesTabDialog preferencesTabDialog(parent);
        preferencesTabDialog.exec();
    }

    static int openQuitDialog(QWidget* parent)
    {
        dialog::QuitDialog dialog(parent);
        dialog.exec();
        return dialog.result();
    }

    static ImageJobContext* createImageJob(QWidget* parent)
    {
        dialog::ImageJobWindow* imageJobWindow = new dialog::ImageJobWindow(parent);
        imageJobWindow->show(); // TODO cleanup
        return new ImageJobContext(BitmapData(10, 10, rgba{0, 0, 0, 0})); // TODO
    }

    static void openTechnicalSupport()
    {
        QDesktopServices::openUrl(QUrl(geometrize::constants::TECHNICAL_SUPPORT_URL));
    }

    static void openOnlineTutorials()
    {
        QDesktopServices::openUrl(QUrl(geometrize::constants::VIDEO_TUTORIAL_URL));
    }

    static void openImage(QGraphicsScene* scene, QWidget* parent)
    {
        QString imagePath{QFileDialog::getOpenFileName(parent, tr("Open Image"), "", tr("Image Files (*.jpg *.jpeg *.png *.bmp)"))};
        if(imagePath.length() == 0) {
            return; // User did not select an item
        }

        // TODO open svg, need to specify size after the fact
        // TODO separate this logic from the error boxes and this code path with signals

        QImage image{imageForImagePath(imagePath)};
        if(!isImageValid(image)) {
            QMessageBox::critical(parent, tr("Image Loading Error"), tr("The image file could not be loaded."));
            return;
        }
        image = image.convertToFormat(QImage::Format_RGBA8888);

        QPixmap pixmap = QPixmap::fromImage(image);
        if(!isPixmapValid(pixmap)) {
            QMessageBox::critical(parent, tr("Image Loading Error"), tr("Failed to convert image file."));
            return;
        }

        QGraphicsPixmapItem* pixmapItem{scene->addPixmap(pixmap)};
        pixmapItem->setPos(QPointF(150, 150)); // TODO center in scene?

        // TODO create a new image job context, add a new tab for the image, add the image background initially
    }

    static void saveImage(QWidget* parent)
    {
        // TODO save svg
        QString imagePath{QFileDialog::getSaveFileName(parent, tr("Save Image"), "", tr("JPEG Image (*.jpg *.jpeg);;PNG Image(*.png);;BMP Image(*.bmp)"))};
        if(imagePath.length() == 0) {
            return;
        }

        // TODO save the file
    }

private:
    static QImage imageForImagePath(const QString imagePath)
    {
        return QImage(imagePath);
    }

    static bool isImageValid(const QImage& image)
    {
        return !image.isNull() && image.width() > 0 && image.height() > 0;
    }

    static bool isPixmapValid(const QPixmap& pixmap)
    {
        return !pixmap.isNull() && pixmap.width() > 0 && pixmap.height() > 0;
    }
};

Q_GLOBAL_STATIC(SharedApp, app)
SharedApp::SharedApp() : d{std::make_unique<SharedAppImpl>()} {}
SharedApp::~SharedApp() {}

void SharedApp::createImageJob(QWidget* parent) const
{
    SharedAppImpl::createImageJob(parent);
}

void SharedApp::openAboutPage(QWidget* parent) const
{
    SharedAppImpl::openAboutPage(parent);
}

void SharedApp::openPreferences(QWidget* parent) const
{
    SharedAppImpl::openPreferences(parent);
}

int SharedApp::openQuitDialog(QWidget* parent) const
{
    return SharedAppImpl::openQuitDialog(parent);
}

void SharedApp::openTechnicalSupport() const
{
    SharedAppImpl::openTechnicalSupport();
}

void SharedApp::openOnlineTutorials() const
{
    SharedAppImpl::openOnlineTutorials();
}

void SharedApp::openImage(QGraphicsScene* scene, QWidget* parent) const
{
    SharedAppImpl::openImage(scene, parent);
}

void SharedApp::saveImage(QWidget* parent) const
{
    SharedAppImpl::saveImage(parent);
}

}
