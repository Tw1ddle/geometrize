#include "sharedapp.h"

#include <QApplication>
#include <QDesktopServices>
#include <QException>
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
#include "recentitems.h"

namespace geometrize
{

/**
 * @brief The SharedAppImpl class contains the concrete implementation of the SharedApp class.
 */
class SharedApp::SharedAppImpl : public QObject
{
public:
    SharedAppImpl() : m_recentFiles{RecentItems::RECENT_FILES_SETTINGS_GROUP}
    {
    }

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

    static void openTechnicalSupport()
    {
        QDesktopServices::openUrl(QUrl(geometrize::constants::TECHNICAL_SUPPORT_URL));
    }

    static void openOnlineTutorials()
    {
        QDesktopServices::openUrl(QUrl(geometrize::constants::VIDEO_TUTORIAL_URL));
    }

    static ImageJobContext* createImageJob(QWidget* parent, const QPixmap& pixmap)
    {
        dialog::ImageJobWindow* imageJobWindow = new dialog::ImageJobWindow(parent);
        imageJobWindow->show(); // TODO cleanup

        // TODO QImage instead?
        //BitmapData data(pixmap.data_ptr().);

        return new ImageJobContext(BitmapData(10, 10, rgba{0, 0, 0, 0})); // TODO
    }

    static QString getImagePath(QWidget* parent)
    {
        return QFileDialog::getOpenFileName(parent, tr("Open Image"), "", tr("Image Files (*.jpg *.jpeg *.png *.bmp)"));
    }

    static QPixmap openPixmap(QWidget* parent, const QString& imagePath)
    {
        QImage image(imagePath);
        if(!isImageValid(image)) {
            QMessageBox::critical(parent, tr("Image Loading Error"), tr("The image file could not be loaded."));
            return QPixmap();
        }

        image = image.convertToFormat(QImage::Format_RGBA8888);

        const QPixmap pixmap{QPixmap::fromImage(image)};
        if(!isPixmapValid(pixmap)) {
            QMessageBox::critical(parent, tr("Image Loading Error"), tr("Failed to convert image file."));
            return QPixmap();
        }

        return pixmap;

        // TODO open svg, need to specify size after the fact
        // TODO separate this logic from the error boxes and this code path with signals
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

    RecentItems& getRecentFiles()
    {
        return m_recentFiles;
    }

private:
    static QImage imageForImagePath(const QString& imagePath)
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

    RecentItems m_recentFiles;
};

SharedApp::SharedApp() : d{std::make_unique<geometrize::SharedApp::SharedAppImpl>()} {}
SharedApp::~SharedApp() {}

SharedApp& SharedApp::get() {
    static SharedApp _instance;
    return _instance;
}

void SharedApp::createImageJob(QWidget* parent, const QPixmap& pixmap) const
{
    SharedAppImpl::createImageJob(parent, pixmap);
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

QPixmap SharedApp::openPixmap(QWidget* parent, const QString& imagePath) const
{
    const QPixmap pixmap{SharedAppImpl::openPixmap(parent, imagePath)};

    if(!pixmap.isNull()) {
        d->getRecentFiles().add(imagePath); // TODO connect?

        emit signal_imageOpened(imagePath);
    }

    return pixmap;
}

void SharedApp::saveImage(QWidget* parent) const
{
    SharedAppImpl::saveImage(parent);
}

QString SharedApp::getImagePath(QWidget *parent) const
{
    return SharedAppImpl::getImagePath(parent);
}

RecentItems& SharedApp::getRecentFiles()
{
    return d->getRecentFiles();
}

}
