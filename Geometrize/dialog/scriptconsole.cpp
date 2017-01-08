#include "scriptconsole.h"
#include "ui_scriptconsole.h"

namespace geometrize
{

namespace dialog
{

class ScriptConsole::ScriptConsoleImpl
{
public:
    ScriptConsoleImpl(ScriptConsole* pQ, Ui::ScriptConsole* pUi) : q{pQ}, ui{pUi}
    {
        ui->setupUi(q);
    }

    ScriptConsoleImpl operator=(const ScriptConsoleImpl&) = delete;
    ScriptConsoleImpl(const ScriptConsoleImpl&) = delete;
    ~ScriptConsoleImpl() = default;

private:
    ScriptConsole* q;
    Ui::ScriptConsole* ui;
};

ScriptConsole::ScriptConsole(QWidget* parent) : QWidget(parent), ui(new Ui::ScriptConsole), d{std::make_unique<ScriptConsole::ScriptConsoleImpl>(this, ui)}
{
}

ScriptConsole::~ScriptConsole()
{
    delete ui;
}

}

}
