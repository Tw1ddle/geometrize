#include "scriptconsole.h"
#include "ui_scriptconsole.h"

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

        connect(ui->commandLine, &QLineEdit::returnPressed, [this]() {
            if(m_engine != nullptr) {
                ui->outputView->append(ui->commandLine->text());
                script::runScript(ui->commandLine->text().toStdString(), *m_engine, nullptr);
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
