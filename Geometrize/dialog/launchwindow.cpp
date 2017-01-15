#include "launchwindow.h"
#include "ui_launchwindow.h"

#include <QAction>
#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QMap>
#include <QMenu>

#include "chaiscript/chaiscript.hpp"

#include "constants.h"
#include "common/sharedapp.h"
#include "common/uiactions.h"
#include "network/downloader.h"
#include "network/networkactions.h"
#include "formatsupport.h"
#include "dialog/recentitemwidget.h"
#include "recentitems.h"
#include "script/chaiscriptcreator.h"
#include "util.h"
#include "serialization/serializationutil.h"

namespace geometrize
{

namespace dialog
{

class LaunchWindow::LaunchWindowImpl
{
public:
    LaunchWindowImpl(LaunchWindow* pQ, Ui::LaunchWindow* pUi) : q{pQ}, ui{pUi}, m_engine{script::createChaiScript()}
    {
        ui->setupUi(q);
        ui->consoleWidget->setVisible(false);
        ui->consoleWidget->setEngine(m_engine.get());

        const QString moreResourcesLabel{tr("Get more online")};
        ui->moreResourcesLink->setText(R"(<a href=")" + constants::MORE_RESOURCES_URL + R"(" style="text-decoration:none;">)" + moreResourcesLabel + R"(</a>)");

        const QString tutorialsLabel{tr("Video tutorials")};
        ui->tutorialsLink->setText(R"(<a href=")" + constants::VIDEO_TUTORIAL_URL + R"(" style="text-decoration:none;">)" + tutorialsLabel + R"(</a>)");

        ui->recentsList->setRecentItems(&common::app::SharedApp::get().getRecentFiles());

        connect(ui->recentsList, &RecentJobsList::itemClicked, [this](QListWidgetItem* item) {
            qDebug() << "Item activated " << item->text();

            // TODO deal with bad paths, use data not text
            const QStringList files{item->text()};
            openJobs(files);
        });

        loadConsoleHistory();
    }
    LaunchWindowImpl operator=(const LaunchWindowImpl&) = delete;
    LaunchWindowImpl(const LaunchWindowImpl&) = delete;
    ~LaunchWindowImpl() = default;

    void openJobs(const QStringList& urls)
    {
        common::ui::openJobs(urls);
    }

    void setConsoleVisibility(const bool visible)
    {
        ui->consoleWidget->setVisible(visible);
    }

    void loadConsoleHistory()
    {
        std::vector<std::string> history{util::readStringVector(util::getAppDataLocation().append("/").append(geometrize::dialog::ScriptConsole::launchConsoleHistoryFilename))};
        q->ui->consoleWidget->setHistory(history);
    }

    void saveConsoleHistory()
    {
        const std::vector<std::string> history{q->ui->consoleWidget->getHistory()};
        util::writeStringVector(history, util::getAppDataLocation().append("/").append(geometrize::dialog::ScriptConsole::launchConsoleHistoryFilename));
    }

private:
    LaunchWindow* q;
    Ui::LaunchWindow* ui;
    std::unique_ptr<chaiscript::ChaiScript> m_engine;
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
    d->saveConsoleHistory();
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

void LaunchWindow::on_openLinkButton_clicked()
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

void LaunchWindow::on_actionScript_Console_toggled(const bool checked)
{
    d->setConsoleVisibility(checked);
}

}

}
