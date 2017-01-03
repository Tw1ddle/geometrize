#include "openurldialog.h"
#include "ui_openurldialog.h"

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
}

OpenUrlDialog::~OpenUrlDialog()
{
    delete ui;
}

QUrl OpenUrlDialog::getUrl() const
{
    const QString userText{ui->urlLineEdit->text()};
    const QUrl url{QUrl::fromUserInput(userText)};

    if(url.isLocalFile()) { // We want remote URLs only, local files are opened with an "Open File" button
        return QUrl();
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

}

}
