#include "runscriptdialog.h"
#include "ui_runscriptdialog.h"

#include <QEvent>

#include "common/uiactions.h"
#include "common/util.h"
#include "script/scriptrunner.h"

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
    populateUi();
}

RunScriptDialog::~RunScriptDialog()
{
    delete ui;
}

QString RunScriptDialog::getScriptFilepath()
{
    return ui->scriptPathEdit->text();
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
    geometrize::script::runScript(script);
}

void RunScriptDialog::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        populateUi();
    }
    QDialog::changeEvent(event);
}

void RunScriptDialog::populateUi()
{

}

}

}
