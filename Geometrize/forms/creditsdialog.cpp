#include "creditsdialog.h"
#include "ui_creditsdialog.h"

CreditsDialog::CreditsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreditsDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);

    ui->developerText->setText("Sam Twidale");
    ui->artistText->setText(tr("Nothing here yet!"));
    ui->translatorText->setText(tr("Nothing here yet!"));

    ui->acknowledgementText->setText(tr("Special thanks to: \n", "A list of people whose effort helped in some way follows"));
    ui->acknowledgementText->append("Michael Fogleman");
    ui->acknowledgementText->append("Harish Persad");
}

CreditsDialog::~CreditsDialog()
{
    delete ui;
}
