#include "launchwindow.h"
#include "ui_launchwindow.h"

#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QMap>

#include "network/downloader.h"
#include "formatsupport.h"
#include "dialog/itembutton.h"
#include "recentitems.h"
#include "sharedapp.h"

namespace geometrize
{

namespace dialog
{

void createImageJob(QWidget* parent, const QUrl& imagePath)
{
    QPixmap pixmap(imagePath.toString());
    if(pixmap.isNull()) {
        // TODO error message and exit out
        return;
    }

    SharedApp::get().getRecentFiles().add(imagePath.toString());
    SharedApp::get().createImageJob(parent, imagePath.toString(), pixmap);
}

class LaunchWindow::LaunchWindowImpl
{
public:
    LaunchWindowImpl(LaunchWindow* pQ, Ui::LaunchWindow* pUi) : q{pQ}, ui{pUi}
    {
        ui->setupUi(q);

        connect(ui->recentsList, &RecentJobsList::itemActivated, [this](QListWidgetItem* item) {
            qDebug() << "Item activated " << item->text();

            // TODO deal with bad paths, use data not text
            const QList<QUrl> files{item->text()};
            openJobs(files);
        });

        ui->recentsList->setRecentItems(&SharedApp::get().getRecentFiles());
    }
    LaunchWindowImpl operator=(const LaunchWindowImpl&) = delete;
    LaunchWindowImpl(const LaunchWindowImpl&) = delete;
    ~LaunchWindowImpl() = default;

    static void downloadImage(QUrl url)
    {
        new network::Downloader(url, &LaunchWindowImpl::onImageDownloadComplete);
    }

    static void downloadWebpage(QUrl url)
    {
        new network::Downloader(url, &LaunchWindowImpl::onWebpageDownloadComplete);
    }

    static void onImageDownloadComplete(network::Downloader* self, QNetworkReply::NetworkError error)
    {
        qDebug() << "FINISHED DOWNLOADING WITH ERROR" << error; // TODO error checks

        QPixmap image;
        image.loadFromData(self->downloadedData());
        SharedApp::get().getRecentFiles().add(self->getUrl().toString());
        SharedApp::get().createImageJob(nullptr, self->getUrl().toString(), image);

        delete self;
    }

    static void onWebpageDownloadComplete(network::Downloader* self, QNetworkReply::NetworkError error)
    {
        qDebug() << "FINISHED DOWNLOADING WITH ERROR" << error; // TODO error checks

        const QString document(self->downloadedData());

        QRegExp imageTagRegex("\\<img[^\\>]*src\\s*=\\s*\"([^\"]*)\"[^\\>]*\\>", Qt::CaseInsensitive);
        imageTagRegex.setMinimal(true);

        QStringList imageMatches;
        QStringList urlMatches;

        int offset{0};
        while((offset = imageTagRegex.indexIn(document, offset)) != -1) {
            offset += imageTagRegex.matchedLength();
            imageMatches.append(imageTagRegex.cap(0)); // Should hold complete img tag
            urlMatches.append(imageTagRegex.cap(1)); // Should hold only src property
        }

        qDebug() << imageMatches;
        qDebug() << urlMatches;

        QList<QUrl> imageUrls;
        for(const QString& url : urlMatches) {
            imageUrls.push_back(QUrl(url));
        }

        const QString baseUrl{self->getUrl().adjusted(QUrl::RemoveFilename | QUrl::StripTrailingSlash).toString()};
        for(QUrl& url : imageUrls) {
            if(url.isRelative()) {
                url = QUrl(baseUrl + url.toString());
            }
            downloadImage(url);
        }

        delete self;
    }

    void openJobs(const QList<QUrl>& urls)
    {
        if(urls.empty()) {
            // TODO validate and show unsupported message?
            return;
        }

        if(!urls.empty()) {
            for(const QUrl& url : urls) {
                if(url.isLocalFile()) {
                    createImageJob(nullptr, url);
                } else if(url.toString().endsWith(".png")) { // TODO need list of supported formats
                    downloadImage(url);
                } else {
                    downloadWebpage(url);
                }
            }
            return;
        }
    }

private:
    LaunchWindow* q;
    Ui::LaunchWindow* ui;
};

LaunchWindow::LaunchWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LaunchWindow),
    d{std::make_unique<LaunchWindow::LaunchWindowImpl>(this, ui)}
{

}

LaunchWindow::~LaunchWindow()
{
    delete ui;
}

void LaunchWindow::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}

void LaunchWindow::dropEvent(QDropEvent* event)
{
    d->openJobs(geometrize::file::getUrls(event->mimeData()));
}

void LaunchWindow::closeEvent(QCloseEvent* event)
{
    const int dialogResult{SharedApp::get().openQuitDialog(this)}; // TODO unsaved changes check
    if(dialogResult == QDialog::Accepted) {
        event->accept();
    } else {
        event->ignore();
    }
}

void LaunchWindow::on_actionPreferences_triggered()
{
    SharedApp::get().openPreferences(this);
}

void LaunchWindow::on_actionClear_Recents_triggered()
{
    SharedApp::get().getRecentFiles().clear();
}

void LaunchWindow::on_actionExit_triggered()
{
    // TODO
    const int dialogResult{SharedApp::get().openQuitDialog(this)};
    switch(dialogResult) {
        case QDialog::Accepted:
            // TODO save any outstanding stuff(?) separate method needed
            QApplication::quit();
    }
}

void LaunchWindow::on_openImageButton_clicked()
{
    const QString imagePath{SharedApp::get().getImagePath(this)};
    if(imagePath.length() == 0) {
        return;
    }
    createImageJob(this, imagePath);
}

void LaunchWindow::on_actionTutorials_triggered()
{
    SharedApp::get().openOnlineTutorials();
}

void LaunchWindow::on_actionSupport_triggered()
{
    SharedApp::get().openTechnicalSupport();
}

void LaunchWindow::on_actionAbout_triggered()
{
    SharedApp::get().openAboutPage(this);
}

}

}
