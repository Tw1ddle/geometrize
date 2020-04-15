#include "scriptconsole.h"
#include "ui_scriptconsole.h"

#include <QEvent>

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
    ScriptConsoleImpl(ScriptConsole* pQ) : q{pQ}, ui{std::make_unique<Ui::ScriptConsole>()}, m_engine{nullptr}
    {
        ui->setupUi(q);
        ui->outputView->append("ChaiScript " + QString(chaiscript::compiler_name));
        ui->outputView->append(tr("Type 'help' in console for a list of commands", "Instructional text shown in the command/scripting console. The 'help' string should not be translated"));
        populateUi();

        connect(ui->commandLine, &geometrize::dialog::CommandLineEdit::signal_commandSubmitted, [this](const std::string& command) {

            ui->outputView->append(QString::fromStdString(command));

            if(command.empty()) {
                return;
            }

            if(m_engine != nullptr) {
                if(command == "help") {
                    const std::vector<std::string> functions{script::getEngineFunctionNames(*m_engine)};
                    for(const std::string& f : functions) {
                        append(QString::fromStdString(f));
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
                    script::runScript(command, *m_engine);
                }
            } else {
                ui->outputView->append(tr("No script engine is set up on this console, commands won't be handled"));
            }
        });
    }

    ScriptConsoleImpl operator=(const ScriptConsoleImpl&) = delete;
    ScriptConsoleImpl(const ScriptConsoleImpl&) = delete;
    ~ScriptConsoleImpl()
    {

    }

    void setEngine(chaiscript::ChaiScript* engine)
    {
        m_engine = engine;
        setCompletionList(engine);
    }

    std::vector<std::string> getHistory() const
    {
        return ui->commandLine->getHistory();
    }

    void setHistory(const std::vector<std::string>& history)
    {
        ui->commandLine->setHistory(history);
    }

    void append(const QString& message)
    {
        ui->outputView->append(message);
    }

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    void populateUi()
    {

    }

    void setCompletionList(chaiscript::ChaiScript* engine)
    {
        if(m_engine == nullptr) {
            ui->commandLine->setCompletionList({});
        } else {
            ui->commandLine->setCompletionList(script::getEngineFunctionNames(*engine));
        }
    }

    ScriptConsole* q;
    std::unique_ptr<Ui::ScriptConsole> ui;
    chaiscript::ChaiScript* m_engine;
    std::vector<std::string> m_history;
};

ScriptConsole::ScriptConsole(QWidget* parent) : QWidget(parent), d{std::make_unique<ScriptConsole::ScriptConsoleImpl>(this)}
{
}

ScriptConsole::~ScriptConsole()
{
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

void ScriptConsole::appendString(const std::string& str)
{
    d->append(QString::fromStdString(str));
}

void ScriptConsole::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

const std::string ScriptConsole::launchConsoleHistoryFilename = "launch_console_command_history.json";

}

}
