#include "uiactions.h"

#include <assert.h>

#include <QDesktopServices>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QPixmap>
#include <QUrl>

#include "lib/geometrizer/geometrizer/bitmap/bitmapdata.h"
#include "lib/geometrizer/geometrizer/bitmap/rgba.h"

#include "common/sharedapp.h"
#include "constants.h"
#include "dialog/aboutdialog.h"
#include "dialog/imagejobwindow.h"
#include "dialog/launchwindow.h"
#include "dialog/openurldialog.h"
#include "dialog/preferencestabdialog.h"
#include "dialog/quitdialog.h"
#include "dialog/runscriptdialog.h"
#include "imagejobcontext.h"
#include "network/completionhandlers.h"
#include "network/networkactions.h"
#include "recentitems.h"

namespace geometrize
{

namespace common
{

namespace ui
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
    dialog.exec();
    return dialog.getUrl();
}

QPair<QString, geometrize::script::ScriptOptions> openGetScriptDialog(QWidget* parent)
{
    dialog::RunScriptDialog dialog(parent);
    dialog.exec();
    return QPair<QString, geometrize::script::ScriptOptions>(dialog.getScriptFilepath(), dialog.getScriptOptions());
}

QString openSelectScriptDialog(QWidget* parent)
{
    return QFileDialog::getOpenFileName(parent, QWidget::tr("Select Script"), "", QWidget::tr("ChaiScript Files (*.chai)", ""));
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
    // TODO error message and exit out

    if(url.isEmpty() || !url.isValid()) {
        return nullptr;
    }

    const QPixmap pixmap(url.toString());
    if(pixmap.isNull()) {
        return nullptr;
    }
    app::SharedApp::get().getRecentFiles().add(url.toString());
    return ui::createImageJob(parent, url.toString(), pixmap);
}

void openJobs(const QStringList& urls)
{
    if(urls.empty()) {
        return;
    }

    for(const QString& s : urls) {
        const QUrl url{QUrl::fromUserInput(s)};
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
        app::SharedApp::get().getRecentFiles().add(imagePath);
    }

    return pixmap;
}

}

}

}
