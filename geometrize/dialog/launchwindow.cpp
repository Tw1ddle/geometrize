#include "launchwindow.h"
#include "ui_launchwindow.h"

#include <QCloseEvent>

#include "chaiscript/chaiscript.hpp"

#include "common/constants.h"
#include "common/formatsupport.h"
#include "common/uiactions.h"
#include "common/util.h"
#include "image/imageloader.h"
#include "preferences/globalpreferences.h"
#include "recents/recentitems.h"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"
#include "serialization/serializationutil.h"
#include "task/imagetask.h"
#include "task/taskutil.h"

namespace geometrize
{

namespace dialog
{

class LaunchWindow::LaunchWindowImpl
{
public:
    LaunchWindowImpl(LaunchWindow* pQ) : ui{std::make_unique<Ui::LaunchWindow>()}, q{pQ}, m_engine{script::createLaunchWindowEngine()}
    {
        ui->setupUi(q);
        ui->consoleWidget->setVisible(false);
        ui->consoleWidget->setEngine(m_engine.get());

        const QString moreResourcesLabel{tr("Get more online")};
        ui->moreResourcesLink->setText(R"(<a href=")" + constants::MORE_RESOURCES_URL + R"(" style="text-decoration:none;">)" + moreResourcesLabel + R"(</a>)");

        const QString tutorialsLabel{tr("Video tutorials")};
        ui->tutorialsLink->setText(R"(<a href=")" + constants::VIDEO_TUTORIAL_URL + R"(" style="text-decoration:none;">)" + tutorialsLabel + R"(</a>)");

        if(preferences::getGlobalPreferences().shouldPopulateRecentItemsOnLaunch()) {
            ui->recentsList->setRecentItems(&geometrize::getRecentItems());
        }

        loadConsoleHistory();
        connect(ui->actionScript_Console, &QAction::toggled, [this](const bool checked) {
            setConsoleVisibility(checked);
        });

        connect(ui->templateGrid, &dialog::TemplateGrid::signal_templateLoaded, [this](const QString& templateFolder, const bool /*success*/) {
            ui->templatesSearchEdit->addToCompletionList(QString::fromStdString(util::getTemplateManifest(templateFolder.toStdString()).getName()));
        });

        connect(ui->templatesSearchEdit, &dialog::CompletionBox::textChanged, [this](const QString& text) {
            ui->templateGrid->setItemFilter(text);
        });

        if(preferences::getGlobalPreferences().shouldPopulateTemplatesOnLaunch()) {
            ui->templateGrid->loadTemplates();
        }

        if(preferences::getGlobalPreferences().shouldShowLaunchConsoleByDefault()) {
            setConsoleVisibility(true);
        }

        setupLogo();
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
        if(ui->actionScript_Console->isChecked() != visible) {
            ui->actionScript_Console->setChecked(visible);
        }
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

    void loadGlobalSettingsTemplate()
    {
        const QString path{common::ui::openLoadGlobalSettingsDialog(q)};
        if(path.isEmpty()) {
            return;
        }
        geometrize::preferences::getGlobalPreferences().load(path.toStdString());
    }

    void saveGlobalSettingsTemplate() const
    {
        const QString path{common::ui::openSaveGlobalSettingsDialog(q)};
        if(path.isEmpty()) {
            return;
        }
        geometrize::preferences::getGlobalPreferences().save(path.toStdString());
    }

private:
    void setupLogo()
    {
        const QString logoPath{":/logos/logo_small.png"};

        if(!geometrize::preferences::getGlobalPreferences().shouldGeometrizeAppLogoOnLaunch()) {
            ui->logoLabel->setPixmap(logoPath);
        } else {
            QImage image(logoPath);
            image = image.convertToFormat(QImage::Format_RGBA8888); // Note doing this to guarantee format is RGBA8888

            geometrize::Bitmap logoBitmap{image::createBitmap(image)};
            geometrize::Bitmap initialBitmap{logoBitmap.getWidth(), logoBitmap.getHeight(), geometrize::rgba{0, 0, 0, 0}};
            m_logoTask = std::make_unique<task::ImageTask>("Logo Image Task", logoBitmap, initialBitmap);
            m_logoTask->getPreferences().setShapeTypes(geometrize::ShapeTypes::RECTANGLE);
            m_logoTask->getPreferences().setShapeAlpha(255U);

            ui->logoLabel->setPixmap(image::createPixmap(m_logoTask->getCurrent()));

            connect(m_logoTask.get(), &task::ImageTask::signal_modelDidStep, [this](std::vector<geometrize::ShapeResult> results) {
                const QPixmap pixmap{image::createPixmap(m_logoTask->getCurrent())};
                ui->logoLabel->setPixmap(pixmap);

                m_logoTaskShapeCount += results.size();
                m_logoTaskStepCount += 1;
                const QString logoToolTip{tr("Logo: %1 shapes added in %2 steps").arg(QString::number(m_logoTaskShapeCount)).arg(m_logoTaskStepCount)};
                ui->logoLabel->setToolTip(logoToolTip);

                m_logoTaskSteps++;
                if(m_logoTaskSteps < m_maxLogoTaskSteps) {
                    m_logoTask->stepModel();
                }
            });
            m_logoTask->stepModel();
        }
    }

    std::unique_ptr<Ui::LaunchWindow> ui;
    LaunchWindow* q;
    std::unique_ptr<chaiscript::ChaiScript> m_engine;

    std::unique_ptr<task::ImageTask> m_logoTask;
    std::size_t m_logoTaskShapeCount{0};
    std::size_t m_logoTaskStepCount{0};
    const std::size_t m_maxLogoTaskSteps{300};
    std::atomic_int m_logoTaskSteps{0};
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
    QStringList tasks;
    for(const QUrl& url : urls) {
        const QString urlString{url.toString()};

        if(urlString.endsWith(".chai") && url.isLocalFile()) {
            scripts.push_back(url.toLocalFile());
        } else {
            tasks.push_back(urlString);
        }
    }

    if(!scripts.empty()) {
        for(const QString& scriptPath : scripts) {
            geometrize::script::runScript(util::readFileAsString(scriptPath.toStdString()));
        }
    }

    if(!tasks.empty()) {
        util::openTasks(tasks, true);
    }
}

void LaunchWindow::closeEvent(QCloseEvent* /*event*/)
{
    d->saveConsoleHistory();
}

void LaunchWindow::on_actionGlobal_Preferences_triggered()
{
    common::ui::openGlobalPreferences(this);
}

void LaunchWindow::on_actionLoad_Global_Preferences_triggered()
{
    d->loadGlobalSettingsTemplate();
}

void LaunchWindow::on_actionSave_Global_Preferences_triggered()
{
    d->saveGlobalSettingsTemplate();
}

void LaunchWindow::on_actionClear_Recents_triggered()
{
    geometrize::getRecentItems().clear();
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

    geometrize::util::openTasks({ imagePath }, true);
}

void LaunchWindow::on_openLinkButton_clicked()
{
    const QUrl url{common::ui::openGetUrlDialog(this)};
    if(!url.isValid()) {
        return;
    }

    geometrize::util::openTasks({ url.toString() }, true);
}

void LaunchWindow::on_runScriptButton_clicked()
{
    const std::string result{common::ui::openGetScriptDialog(this).toStdString()};
    geometrize::script::runScript(util::readFileAsString(result));
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
