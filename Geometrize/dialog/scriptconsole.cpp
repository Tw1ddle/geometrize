#include "scriptconsole.h"
#include "ui_scriptconsole.h"

#include "chaiscript/chaiscript.hpp"
#include "script/chaiscriptcreator.h"
#include "script/scriptrunner.h"

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

        connect(ui->commandLine, &QLineEdit::returnPressed, [this]() {
            const QString text{ui->commandLine->text()};
            const std::string stdText{ui->commandLine->text().toStdString()};

            ui->outputView->append(text);
            m_history.push_back(stdText);

            if(m_engine != nullptr) {
                if(text == "help") { // TODO?
                    printEngineFunctions();
                } else {
                    script::runScript(stdText, *m_engine, nullptr);
                }
            }
            ui->commandLine->clear();
        });
    }

    ScriptConsoleImpl operator=(const ScriptConsoleImpl&) = delete;
    ScriptConsoleImpl(const ScriptConsoleImpl&) = delete;
    ~ScriptConsoleImpl() = default;

    void setEngine(chaiscript::ChaiScript* engine)
    {
        m_engine = engine;
    }

private:
    void printEngineFunctions()
    {
        const auto state{m_engine->get_state()};
        const auto funcs{state.engine_state.m_functions};
        for(auto it = funcs.begin(); it != funcs.end(); ++it) {
            ui->outputView->append(QString::fromStdString(it->first));
        }
    }

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

}

}
