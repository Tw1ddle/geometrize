#include "creditsdialog.h"
#include "ui_creditsdialog.h"

namespace geometrize
{

namespace dialog
{

CreditsDialog::CreditsDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CreditsDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);

    ui->developerText->setText("Sam Twidale");
    ui->artistText->setText(tr("Nobody here yet!"));
    ui->translatorText->setText(tr("Nobody here yet!"));

    ui->acknowledgementText->setText(tr("Special thanks to: \n", "A list of names of people who contributed to the project follows"));
    ui->acknowledgementText->append("Michael Fogleman");
    ui->acknowledgementText->append("Harish Persad");
}

CreditsDialog::~CreditsDialog()
{
    delete ui;
}

}

}
