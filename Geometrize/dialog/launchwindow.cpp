#include "launchwindow.h"
#include "ui_launchwindow.h"

#include <QAction>
#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QMap>
#include <QMenu>

#include "constants.h"
#include "common/sharedapp.h"
#include "common/uiactions.h"
#include "network/downloader.h"
#include "network/networkactions.h"
#include "formatsupport.h"
#include "dialog/itembutton.h"
#include "recentitems.h"

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

        const QString moreResourcesLabel{tr("Get more templates online")};
        ui->moreResourcesLink->setText(R"(<a href=")" + constants::MORE_RESOURCES_URL + R"(" style="text-decoration:none;">)" + moreResourcesLabel + R"(</a>)");

        const QString tutorialsLabel{tr("Video tutorials")};
        ui->tutorialsLink->setText(R"(<a href=")" + constants::VIDEO_TUTORIAL_URL + R"(" style="text-decoration:none;">)" + tutorialsLabel + R"(</a>)");

        ui->recentsList->setRecentItems(&common::app::SharedApp::get().getRecentFiles());

        connect(ui->recentsList, &RecentJobsList::itemActivated, [this](QListWidgetItem* item) {
            qDebug() << "Item activated " << item->text();

            // TODO deal with bad paths, use data not text
            const QStringList files{item->text()};
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

    void openJobs(const QStringList& urls)
    {
        common::ui::openJobs(urls);
    }

private:
    LaunchWindow* q;
    Ui::LaunchWindow* ui;
};

LaunchWindow::LaunchWindow() :
    QMainWindow(nullptr),
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
    // TODO
    //d->openJobs(geometrize::format::getUrls(event->mimeData()));
}

void LaunchWindow::closeEvent(QCloseEvent* event)
{
    const int dialogResult{common::ui::openQuitDialog(this)}; // TODO unsaved changes check
    if(dialogResult == QDialog::Accepted) {
        event->accept();
    } else {
        event->ignore();
    }
}

void LaunchWindow::on_actionPreferences_triggered()
{
    common::ui::openPreferences(this);
}

void LaunchWindow::on_actionClear_Recents_triggered()
{
    common::app::SharedApp::get().getRecentFiles().clear();
}

void LaunchWindow::on_actionExit_triggered()
{
    // TODO
    const int dialogResult{common::ui::openQuitDialog(this)};
    switch(dialogResult) {
        case QDialog::Accepted:
            // TODO save any outstanding stuff(?) separate method needed
            QApplication::quit();
    }
}

void LaunchWindow::on_openImageButton_clicked()
{
    // TODO support other file types?
    common::ui::createImageJobAndUpdateRecents(this, common::ui::getImagePath(this));
}

void LaunchWindow::on_openWebpageButton_clicked()
{
    const QUrl url{common::ui::openGetUrlDialog(this)};
    if(url.isValid()) {
        common::ui::openJobs({ url.toString() });
    }
}

void LaunchWindow::on_runScriptButton_clicked()
{
    common::ui::openGetScriptDialog(this);

    // TODO get path and args and run
    //app::runScript(scriptPath, arguments);
}

void LaunchWindow::on_actionTutorials_triggered()
{
    common::ui::openOnlineTutorials();
}

void LaunchWindow::on_actionSupport_triggered()
{
    common::ui::openTechnicalSupport();
}

void LaunchWindow::on_actionAbout_triggered()
{
    common::ui::openAboutPage(this);
}

}

}
