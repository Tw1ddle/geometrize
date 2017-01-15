#include "scriptconsole.h"
#include "ui_scriptconsole.h"

#include "chaiscript/chaiscript.hpp"
#include "script/scriptrunner.h"
#include "script/scriptutil.h"

namespace geometrize
{

namespace dialog
{

class ScriptConsole::ScriptConsoleImpl
{
public:
    ScriptConsoleImpl(ScriptConsole* pQ, Ui::ScriptConsole* pUi) : q{pQ}, ui{pUi}, m_engine{nullptr}
    {
        ui->setupUi(q);

        ui->outputView->append("ChaiScript " + QString::fromStdString(m_engine->build_id()));
        ui->outputView->append(tr("Type 'help' for a list of commands"));

        connect(ui->commandLine, &geometrize::dialog::CommandLineEdit::signal_commandSubmitted, [this](const std::string& command) {
            // TODO refactor so the real CLI uses this function with a REPL-like setup

            ui->outputView->append(QString::fromStdString(command));

            if(command.empty()) {
                return;
            }

            if(m_engine != nullptr) {
                if(command == "help") {
                    const std::vector<std::string> functions{script::getEngineFunctionNames(*m_engine)};
                    for(const std::string& f : functions) {
                        ui->outputView->append(QString::fromStdString(f));
                    }
                } else if(command == "clearHistory") {
                    ui->commandLine->clearHistory();
                } else if(command == "history") {
                   const std::vector<std::string> history{ui->commandLine->getHistory()};
                   for(const std::string& command : history) {
                        ui->outputView->append(QString::fromStdString(command));
                    }
                } else if(command == "clear") {
                    ui->outputView->clear();
                } else {
                    script::runScript(command, *m_engine, nullptr);
                }
            }
        });
    }

    ScriptConsoleImpl operator=(const ScriptConsoleImpl&) = delete;
    ScriptConsoleImpl(const ScriptConsoleImpl&) = delete;
    ~ScriptConsoleImpl() = default;

    void setEngine(chaiscript::ChaiScript* engine)
    {
        m_engine = engine;
    }

    std::vector<std::string> getHistory() const
    {
        return ui->commandLine->getHistory();
    }

    void setHistory(const std::vector<std::string>& history)
    {
        ui->commandLine->setHistory(history);
    }

private:
    ScriptConsole* q;
    Ui::ScriptConsole* ui;
    chaiscript::ChaiScript* m_engine;
    std::vector<std::string> m_history;
};

ScriptConsole::ScriptConsole(QWidget* parent) : QWidget(parent), ui(new Ui::ScriptConsole), d{std::make_unique<ScriptConsole::ScriptConsoleImpl>(this, ui)}
{
}

ScriptConsole::~ScriptConsole()
{
    delete ui;
}

void ScriptConsole::setEngine(chaiscript::ChaiScript* engine)
{
    d->setEngine(engine);
}

std::vector<std::string> ScriptConsole::getHistory() const
{
    return d->getHistory();
}

void ScriptConsole::setHistory(const std::vector<std::string>& history)
{
    d->setHistory(history);
}

std::string ScriptConsole::getConsoleHistoryFilenameForJob(const std::string& jobUrl)
{
    return ""; // TODO
}

const std::string ScriptConsole::launchConsoleHistoryFilename = "launch_console_command_history.json";

}

}
