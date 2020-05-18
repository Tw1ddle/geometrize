#include "openurldialog.h"
#include "ui_openurldialog.h"

#include <QEvent>
#include <QLineEdit>
#include <QString>
#include <QUrl>

namespace geometrize
{

namespace dialog
{

OpenUrlDialog::OpenUrlDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::OpenUrlDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);
    populateUi();
}

OpenUrlDialog::~OpenUrlDialog()
{
    delete ui;
}

QUrl OpenUrlDialog::getUrl() const
{
    const QString userText{ui->urlLineEdit->text()};
    const QUrl url{QUrl::fromUserInput(userText)};

    // We want remote URLs only, local files are opened with an "Open File" button
    if(url.isLocalFile() || url.isEmpty()) {
        return QUrl::fromUserInput(ui->urlLineEdit->placeholderText());
    }
    return url;
}

void OpenUrlDialog::on_buttonBox_accepted()
{
    close();
}

void OpenUrlDialog::on_buttonBox_rejected()
{
    close();
}

void OpenUrlDialog::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        populateUi();
    }
    QDialog::changeEvent(event);
}

void OpenUrlDialog::populateUi()
{

}

}

}
