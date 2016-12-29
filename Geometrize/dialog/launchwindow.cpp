#include "launchwindow.h"
#include "ui_launchwindow.h"

#include <QAction>
#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QMap>
#include <QMenu>

#include "network/downloader.h"
#include "network/networkactions.h"
#include "formatsupport.h"
#include "dialog/itembutton.h"
#include "recentitems.h"
#include "sharedapp.h"

namespace geometrize
{

namespace dialog
{

class LaunchWindow::LaunchWindowImpl
{
public:
    LaunchWindowImpl(LaunchWindow* pQ, Ui::LaunchWindow* pUi) : q{pQ}, ui{pUi}
    {
        ui->setupUi(q);

        ui->recentsList->setRecentItems(&app::SharedApp::get().getRecentFiles());

        connect(ui->recentsList, &RecentJobsList::itemActivated, [this](QListWidgetItem* item) {
            qDebug() << "Item activated " << item->text();

            // TODO deal with bad paths, use data not text
            const QList<QUrl> files{item->text()};
            openJobs(files);
        });

        connect(ui->recentsList, &RecentJobsList::signal_contextMenuRequested, [](QListWidgetItem* item, QPoint position) {
            // TODO

            QMenu itemContextMenu;
            QAction openAction(tr("Open"));
            itemContextMenu.addAction(&openAction);
            QAction copyToClipboard(tr("Copy path to clipboard"));
            itemContextMenu.addAction(&copyToClipboard);
            QAction removalAction(tr("Remove from list"));
            //connect(&removalAction, &QAction::triggered, [this, &item]() {
                //ui->recentsList->getRecentItems()->remove(item->text()); // TODO
            //});
            itemContextMenu.addAction(&removalAction);

            itemContextMenu.exec(position);
        });
    }
    LaunchWindowImpl operator=(const LaunchWindowImpl&) = delete;
    LaunchWindowImpl(const LaunchWindowImpl&) = delete;
    ~LaunchWindowImpl() = default;

    void openJobs(const QList<QUrl>& urls)
    {
        app::openJobs(urls);
    }

private slots:
    void on_openWebpageButton_clicked();

private:
    LaunchWindow* q;
    Ui::LaunchWindow* ui;
};

LaunchWindow::LaunchWindow(QWidget* parent) :
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
    d->openJobs(geometrize::format::getUrls(event->mimeData()));
}

void LaunchWindow::closeEvent(QCloseEvent* event)
{
    const int dialogResult{app::openQuitDialog(this)}; // TODO unsaved changes check
    if(dialogResult == QDialog::Accepted) {
        event->accept();
    } else {
        event->ignore();
    }
}

void LaunchWindow::on_actionPreferences_triggered()
{
    app::openPreferences(this);
}

void LaunchWindow::on_actionClear_Recents_triggered()
{
    app::SharedApp::get().getRecentFiles().clear();
}

void LaunchWindow::on_actionExit_triggered()
{
    // TODO
    const int dialogResult{app::openQuitDialog(this)};
    switch(dialogResult) {
        case QDialog::Accepted:
            // TODO save any outstanding stuff(?) separate method needed
            QApplication::quit();
    }
}

void LaunchWindow::on_openImageButton_clicked()
{
    const QString imagePath{app::getImagePath(this)};
    if(imagePath.length() == 0) {
        return;
    }
    app::createImageJobAndUpdateRecents(this, imagePath);
}

void LaunchWindow::on_openWebpageButton_clicked()
{
    const QUrl url{app::openGetUrlDialog(this)};

    if(url.isValid()) {
        app::createImageJobAndUpdateRecents(this, url);
    }
}

void LaunchWindow::on_actionTutorials_triggered()
{
    app::openOnlineTutorials();
}

void LaunchWindow::on_actionSupport_triggered()
{
    app::openTechnicalSupport();
}

void LaunchWindow::on_actionAbout_triggered()
{
    app::openAboutPage(this);
}

}

}
