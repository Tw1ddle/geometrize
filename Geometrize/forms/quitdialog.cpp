#include "quitdialog.h"
#include "ui_quitdialog.h"

QuitDialog::QuitDialog(QWidget *parent) :
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
