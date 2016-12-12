#include "openurldialog.h"
#include "ui_openurldialog.h"

#include <QUrl>

namespace geometrize
{

namespace dialog
{

OpenUrlDialog::OpenUrlDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenUrlDialog)
{
    ui->setupUi(this);
}

OpenUrlDialog::~OpenUrlDialog()
{
    delete ui;
}

QUrl OpenUrlDialog::getUrl() const
{
    return QUrl(); // TODO get the current input in the text box and try to convert to URL
}

}

}
