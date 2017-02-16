#include "launchwindow.h"
#include "ui_launchwindow.h"

#include <QCloseEvent>
#include <QDialog>

#include "chaiscript/chaiscript.hpp"

#include "constants.h"
#include "common/sharedapp.h"
#include "common/uiactions.h"
#include "dialog/recentitemwidget.h"
#include "script/chaiscriptcreator.h"
#include "job/imagejob.h"
#include "job/jobutil.h"
#include "image/imageloader.h"
#include "util.h"
#include "serialization/serializationutil.h"

namespace geometrize
{

namespace dialog
{

class LaunchWindow::LaunchWindowImpl
{
public:
    LaunchWindowImpl(LaunchWindow* pQ) : ui{std::make_unique<Ui::LaunchWindow>()}, q{pQ}, m_engine{script::createChaiScript()}
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
            // TODO deal with bad paths, use data not text
            const QStringList files{item->text()};
            openJobs(files, false);
        });

        loadConsoleHistory();

        connect(ui->templateGrid, &dialog::TemplateGrid::signal_templateLoaded, [this](const QString& templateFolder, const bool success) {
            ui->templatesSearchEdit->addToCompletionList(QString::fromStdString(util::getTemplateManifest(templateFolder.toStdString()).getName()));
        });

        connect(ui->templatesSearchEdit, &dialog::SearchBox::textChanged, [this](const QString& text) {
            ui->templateGrid->setItemFilter(text);
        });

        ui->templateGrid->loadTemplates();

        setupLogoImageJob();
    }
    LaunchWindowImpl operator=(const LaunchWindowImpl&) = delete;
    LaunchWindowImpl(const LaunchWindowImpl&) = delete;
    ~LaunchWindowImpl() = default;

    static std::vector<LaunchWindow*> getTopLevelLaunchWindows()
    {
        std::vector<LaunchWindow*> windows;

        QWidgetList topLevelWidgets{QApplication::topLevelWidgets()};
        for(QWidget* w : topLevelWidgets) {
            if(LaunchWindow* lw = dynamic_cast<LaunchWindow*>(w)) {
                windows.push_back(lw);
            }
        }

        return windows;
    }

    void openJobs(const QStringList& urls, const bool addToRecents)
    {
        util::openJobs(urls, addToRecents);
    }

    void setConsoleVisibility(const bool visible)
    {
        ui->consoleWidget->setVisible(visible);
    }

    void loadConsoleHistory()
    {
        // TODO possibly add helper function to make this instead of appending slash?
        std::vector<std::string> history{util::readStringVector(util::getAppDataLocation().append("/").append(geometrize::dialog::ScriptConsole::launchConsoleHistoryFilename))};
        ui->consoleWidget->setHistory(history);
    }

    void saveConsoleHistory()
    {
        const std::vector<std::string> history{ui->consoleWidget->getHistory()};
        util::writeStringVector(history, util::getAppDataLocation().append("/").append(geometrize::dialog::ScriptConsole::launchConsoleHistoryFilename));
    }

private:
    void setupLogoImageJob()
    {
        // TODO ensure format is RGBA8888
        const QPixmap* logo{ui->logoLabel->pixmap()};
        geometrize::Bitmap logoBitmap{image::createBitmap(logo->toImage())};
        m_logoJob = std::make_unique<job::ImageJob>("Logo Image Job", "Logo (Resource File)", logoBitmap);
        ui->logoLabel->setPixmap(image::createPixmap(m_logoJob->getBitmap()));

        connect(m_logoJob.get(), &job::ImageJob::signal_modelDidStep, [this](std::vector<geometrize::ShapeResult> results) {
            const QPixmap pixmap{image::createPixmap(m_logoJob->getBitmap())};
            ui->logoLabel->setPixmap(pixmap);

            // TODO set tooltip for number of steps and time taken
            // TODO disconnect when finished and destroy job?
            // TODO alpha background is currently black
        });

        for(int i = 0; i < maxLogoJobSteps; i++) {
            m_logoJob->stepModel();
        }
    }

    std::unique_ptr<Ui::LaunchWindow> ui;
    LaunchWindow* q;
    std::unique_ptr<chaiscript::ChaiScript> m_engine;

    std::unique_ptr<job::ImageJob> m_logoJob;
    const int maxLogoJobSteps{100};
};

LaunchWindow::LaunchWindow() :
    QMainWindow(nullptr),
    d{std::make_unique<LaunchWindow::LaunchWindowImpl>(this)}
{
}

LaunchWindow::~LaunchWindow()
{
}

std::vector<LaunchWindow*> LaunchWindow::getTopLevelLaunchWindows()
{
    return LaunchWindowImpl::getTopLevelLaunchWindows();
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
    close();
}

void LaunchWindow::on_openImageButton_clicked()
{
    const QString imagePath{common::ui::openImagePathPickerDialog(this)};
    if(imagePath.isEmpty()) {
        return;
    }

    d->openJobs({ imagePath }, true);
}

void LaunchWindow::on_openLinkButton_clicked()
{
    const QUrl url{common::ui::openGetUrlDialog(this)};
    if(url.isValid()) {
        geometrize::util::openJobs({ url.toString() }, true);
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
