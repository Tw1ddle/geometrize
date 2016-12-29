#include "quitdialog.h"
#include "ui_quitdialog.h"

namespace geometrize
{

namespace dialog
{

// TODO create multiple versions of this - ones for closing different types of jobs, and one for closing the main application (with a different message!)

QuitDialog::QuitDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::QuitDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);
}

QuitDialog::~QuitDialog()
{
    delete ui;
}

void QuitDialog::on_saveButton_clicked()
{
    // TODO get the first unsaved/dirty job, open save dialog for it, then close the dialog. make user repeat until all are saved
}

void QuitDialog::on_dontSaveButton_clicked()
{
    QApplication::quit();
}

void QuitDialog::on_cancelButton_clicked()
{
    close();
}

}

}
