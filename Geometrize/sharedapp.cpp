#include "sharedapp.h"

#include <assert.h>
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
#include "dialog/openurldialog.h"
#include "dialog/preferencestabdialog.h"
#include "dialog/quitdialog.h"
#include "imagejobcontext.h"
#include "network/completionhandlers.h"
#include "network/networkactions.h"
#include "recentitems.h"

namespace geometrize
{

namespace app
{

void openAboutPage(QWidget* parent)
{
    dialog::AboutDialog dialog(parent);
    dialog.exec();
}

void openPreferences(QWidget* parent)
{
    dialog::PreferencesTabDialog preferencesTabDialog(parent);
    preferencesTabDialog.exec();
}

int openQuitDialog(QWidget* parent)
{
    dialog::QuitDialog dialog(parent);
    dialog.exec();
    return dialog.result();
}

QUrl openGetUrlDialog(QWidget* parent)
{
    dialog::OpenUrlDialog dialog(parent);
    dialog.show();
    return dialog.getUrl();
}

void openTechnicalSupport()
{
    QDesktopServices::openUrl(QUrl(geometrize::constants::TECHNICAL_SUPPORT_URL));
}

void openOnlineTutorials()
{
    QDesktopServices::openUrl(QUrl(geometrize::constants::VIDEO_TUTORIAL_URL));
}

ImageJobContext* createImageJob(QWidget* parent, const QString& displayName, const QPixmap& pixmap)
{
    dialog::ImageJobWindow* imageJobWindow = new dialog::ImageJobWindow(parent);
    imageJobWindow->show(); // TODO cleanup
    imageJobWindow->setWindowTitle(displayName);

    // TODO QImage instead?
    //BitmapData data(pixmap.data_ptr().);

    ImageJobContext* context = new ImageJobContext(displayName, BitmapData(10, 10, rgba{0, 0, 0, 0})); // TODO
    return context;
}

ImageJobContext* createImageJobAndUpdateRecents(QWidget* parent, const QUrl& url)
{
    QPixmap pixmap(url.toString());
    if(pixmap.isNull()) {
        // TODO error message and exit out
        return nullptr;
    }
    app::SharedApp::get().getRecentFiles().add(url.toString());
    return app::createImageJob(parent, url.toString(), pixmap);
}

void openJobs(const QList<QUrl>& urls)
{
    if(urls.empty()) {
        return;
    }

    for(const QUrl& url : urls) {
        if(url.isLocalFile()) {
            createImageJob(nullptr, url.toString(), url.toLocalFile());
        } else if(url.toString().endsWith(".png")) { // TODO need list of supported formats
            network::downloadImage(url, network::completionhandlers::onImageDownloadComplete);
        } else {
            network::downloadWebpage(url, network::completionhandlers::onWebpageDownloadComplete);
        }
    }
}

QImage imageForImagePath(const QString& imagePath)
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

QString getImagePath(QWidget* parent)
{
    return QFileDialog::getOpenFileName(parent, QWidget::tr("Open Image"), "", QWidget::tr("Image Files (*.jpg *.jpeg *.png *.bmp)", ""));
}

void saveImage(QWidget* parent)
{
    // TODO save svg
    QString imagePath{QFileDialog::getSaveFileName(parent, QWidget::tr("Save Image"), "", QWidget::tr("JPEG Image (*.jpg *.jpeg);;PNG Image(*.png);;BMP Image(*.bmp)"))};
    if(imagePath.length() == 0) {
        return;
    }

    // TODO save the file
}

QPixmap openPixmap(QWidget* parent, const QString& imagePath)
{
    const auto openPix = [&](QWidget* parent, const QString& imagePath) {
        QImage image(imagePath);
        if(!isImageValid(image)) {
            QMessageBox::critical(parent, QWidget::tr("Image Loading Error"), QWidget::tr("The image file could not be loaded."));
            return QPixmap();
        }

        image = image.convertToFormat(QImage::Format_RGBA8888);

        const QPixmap pixmap{QPixmap::fromImage(image)};
        if(!isPixmapValid(pixmap)) {
            QMessageBox::critical(parent, QWidget::tr("Image Loading Error"), QWidget::tr("Failed to convert image file."));
            return QPixmap();
        }

        return pixmap;

        // TODO open svg, need to specify size after the fact
        // TODO separate this logic from the error boxes and this code path with signals
        // TODO create a new image job context, add a new tab for the image, add the image background initially
    };

    const QPixmap pixmap{openPix(parent, imagePath)};

    assert(!pixmap.isNull());
    if(!pixmap.isNull()) {
        SharedApp::get().getRecentFiles().add(imagePath);
    }

    return pixmap;
}

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

    RecentItems& getRecentFiles()
    {
        return m_recentFiles;
    }

private:
    RecentItems m_recentFiles;
};

SharedApp::SharedApp() : d{std::make_unique<geometrize::app::SharedApp::SharedAppImpl>()} {}
SharedApp::~SharedApp() {}

SharedApp& SharedApp::get()
{
    static SharedApp _instance;
    return _instance;
}

RecentItems& SharedApp::getRecentFiles()
{
    return d->getRecentFiles();
}

}

}
