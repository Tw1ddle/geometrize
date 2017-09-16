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
    ui->artistText->setText("");
    ui->translatorText->setText("");

    const QString acknowledgementText{tr("Special thanks to: %1", "Message that thanks contributors to the project. A list of names of people who contributed to the project follows this.")
                                     .arg("\n Michael Fogleman \n Harish Persad")};
    ui->acknowledgementText->setText(acknowledgementText);
}

CreditsDialog::~CreditsDialog()
{
    delete ui;
}

}

}
