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

void createImageJob(QWidget* parent, const QString& imagePath)
{
    QPixmap pixmap(imagePath);
    if(pixmap.isNull()) {
        // TODO error message and exit out
        return;
    }

    SharedApp::get().getRecentFiles().add(imagePath);
    SharedApp::get().createImageJob(parent, pixmap);
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
            createImageJob(q, item->text());
        });

        ui->recentsList->setRecentItems(&SharedApp::get().getRecentFiles());
    }
    LaunchWindowImpl operator=(const LaunchWindowImpl&) = delete;
    LaunchWindowImpl(const LaunchWindowImpl&) = delete;
    ~LaunchWindowImpl() = default;

    void downloadImage(QUrl url)
    {
        new network::Downloader(url, &LaunchWindowImpl::onImageDownloadComplete);
    }

    static void onImageDownloadComplete(network::Downloader* self, QNetworkReply::NetworkError error) {
        QPixmap image;
        image.loadFromData(self->downloadedData());
        SharedApp::get().getRecentFiles().add(self->getUrl().toString());
        SharedApp::get().createImageJob(nullptr, image);
        qDebug() << "FINISHED DOWNLOADING WITH ERROR" << error;

        // TODO remove from list so it self-destructs
        //m_imageDownloaders
        delete self;
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
    const QStringList files{geometrize::file::getLocalFiles(event->mimeData())};
    const QList<QUrl> urls{geometrize::file::getRemoteUrls(event->mimeData())};
    if(files.empty() && urls.empty()) {
        // TODO validate and show unsupported message?
        return;
    }

    if(!files.empty()) {
        for(const QString& file : files) {
            createImageJob(nullptr, file);
        }
        return;
    }

    if(!urls.empty()) {
        for(const QUrl& url : urls) {
            if(url.toString().endsWith(".png")) { // TODO need list of supported formats
                d->downloadImage(url);
            }
        }
        return;
    }
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
