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
            if(m_engine != nullptr) {
                if(ui->commandLine->text() == "help") { // TODO?
                    const auto state{m_engine->get_state()};
                    const auto funcs{state.engine_state.m_functions};
                    for(auto it = funcs.begin(); it != funcs.end(); ++it) {
                        ui->outputView->append(QString::fromStdString(it->first));
                    }
                } else {
                    script::runScript(ui->commandLine->text().toStdString(), *m_engine, nullptr);
                }
            }
            ui->outputView->append(ui->commandLine->text());
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
    ScriptConsole* q;
    Ui::ScriptConsole* ui;
    chaiscript::ChaiScript* m_engine;
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
