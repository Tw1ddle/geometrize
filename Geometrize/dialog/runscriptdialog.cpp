#include "runscriptdialog.h"
#include "ui_runscriptdialog.h"

#include "common/uiactions.h"
#include "script/scriptrunner.h"
#include "util.h"

namespace geometrize
{

namespace dialog
{

RunScriptDialog::RunScriptDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::RunScriptDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);
}

RunScriptDialog::~RunScriptDialog()
{
    delete ui;
}

QString RunScriptDialog::getScriptFilepath()
{
    return ui->scriptPathEdit->text();
}

geometrize::script::ScriptOptions RunScriptDialog::getScriptOptions()
{
    return geometrize::script::ScriptOptions(ui->scriptFlagsEdit->text());
}

void RunScriptDialog::on_scriptFileDialogButton_clicked()
{
    const QString scriptPath{common::ui::openSelectScriptDialog(this)};
    if(scriptPath.isEmpty()) {
        return;
    }
    ui->scriptPathEdit->setText(scriptPath);
}

void RunScriptDialog::on_buttonBox_accepted()
{
    const std::string scriptPath{getScriptFilepath().toStdString()};
    if(!util::fileExists(scriptPath)) {
        return;
    }

    const std::string script{util::readFileAsString(scriptPath)};
    geometrize::script::runScript(script, getScriptOptions());
}

}

}
