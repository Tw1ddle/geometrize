#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
