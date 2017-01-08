#include "scriptconsole.h"
#include "ui_scriptconsole.h"

namespace geometrize
{

namespace dialog
{

ScriptConsole::ScriptConsole(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ScriptConsole)
{
    ui->setupUi(this);
}

ScriptConsole::~ScriptConsole()
{
    delete ui;
}

}

}
