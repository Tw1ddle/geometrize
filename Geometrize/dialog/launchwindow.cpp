#include "launchwindow.h"
#include "ui_launchwindow.h"

#include <QCloseEvent>
#include <QDialog>

#include "chaiscript/chaiscript.hpp"

#include "constants.h"
#include "common/sharedapp.h"
#include "common/uiactions.h"
#include "dialog/recentitemwidget.h"
#include "job/imagejob.h"
#include "job/jobutil.h"
#include "image/imageloader.h"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"
#include "serialization/serializationutil.h"
#include "formatsupport.h"
#include "recentitems.h"
#include "util.h"

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

        loadConsoleHistory();

        connect(ui->templateGrid, &dialog::TemplateGrid::signal_templateLoaded, [this](const QString& templateFolder, const bool /*success*/) {
            ui->templatesSearchEdit->addToCompletionList(QString::fromStdString(util::getTemplateManifest(templateFolder.toStdString()).getName()));
        });

        connect(ui->templatesSearchEdit, &dialog::CompletionBox::textChanged, [this](const QString& text) {
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

    void setConsoleVisibility(const bool visible)
    {
        ui->consoleWidget->setVisible(visible);
    }

    void loadConsoleHistory()
    {
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
        QImage image(":/logos/logo_small.png");
        image = image.convertToFormat(QImage::Format_RGBA8888); // Note doing this to guarantee format is RGBA8888

        geometrize::Bitmap logoBitmap{image::createBitmap(image)};
        geometrize::Bitmap initialBitmap{logoBitmap.getWidth(), logoBitmap.getHeight(), geometrize::rgba{0, 0, 0, 0}};
        m_logoJob = std::make_unique<job::ImageJob>("Logo Image Job", "Logo (Resource File)", logoBitmap, initialBitmap);
        m_logoJob->getPreferences().setShapeTypes(geometrize::ShapeTypes::RECTANGLE);
        m_logoJob->getPreferences().setShapeAlpha(255U);

        ui->logoLabel->setPixmap(image::createPixmap(m_logoJob->getCurrent()));

        connect(m_logoJob.get(), &job::ImageJob::signal_modelDidStep, [this](std::vector<geometrize::ShapeResult> results) {
            const QPixmap pixmap{image::createPixmap(m_logoJob->getCurrent())};
            ui->logoLabel->setPixmap(pixmap);

            m_logoJobShapeCount += results.size();
            m_logoJobStepCount += 1;
            const QString logoToolTip{tr("Logo: %1 shapes added in %2 steps").arg(QString::number(m_logoJobShapeCount)).arg(m_logoJobStepCount)};
            ui->logoLabel->setToolTip(logoToolTip);

            m_logoJobSteps++;
            if(m_logoJobSteps < m_maxLogoJobSteps) {
                m_logoJob->stepModel();
            }
        });
        m_logoJob->stepModel();
    }

    std::unique_ptr<Ui::LaunchWindow> ui;
    LaunchWindow* q;
    std::unique_ptr<chaiscript::ChaiScript> m_engine;

    std::unique_ptr<job::ImageJob> m_logoJob;
    std::size_t m_logoJobShapeCount{0};
    std::size_t m_logoJobStepCount{0};
    const std::size_t m_maxLogoJobSteps{300};
    std::atomic_int m_logoJobSteps{0};
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
    const QList<QUrl> urls{geometrize::format::getUrls(event->mimeData())};
    QStringList scripts;
    QStringList jobs;
    for(const QUrl& url : urls) {
        const QString urlString{url.toString()};

        if(urlString.endsWith(".chai") && url.isLocalFile()) {
            scripts.push_back(url.toLocalFile());
        } else {
            jobs.push_back(urlString);
        }
    }

    if(!scripts.empty()) {
        for(const QString& scriptPath : scripts) {
            geometrize::script::runScript(util::readFileAsString(scriptPath.toStdString()));
        }
    }

    if(!jobs.empty()) {
        util::openJobs(jobs, true);
    }
}

void LaunchWindow::closeEvent(QCloseEvent* /*event*/)
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

    geometrize::util::openJobs({ imagePath }, true);
}

void LaunchWindow::on_openLinkButton_clicked()
{
    const QUrl url{common::ui::openGetUrlDialog(this)};
    if(!url.isValid()) {
        return;
    }

    geometrize::util::openJobs({ url.toString() }, true);
}

void LaunchWindow::on_runScriptButton_clicked()
{
    const QPair<QString, geometrize::script::ScriptOptions> result{common::ui::openGetScriptDialog(this)};
    geometrize::script::runScript(util::readFileAsString(result.first.toStdString()), &result.second);
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
