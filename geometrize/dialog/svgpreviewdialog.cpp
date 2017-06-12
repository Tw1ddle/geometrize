#include "svgpreviewdialog.h"
#include "ui_svgpreviewdialog.h"

namespace geometrize
{

namespace dialog
{

SvgPreviewDialog::SvgPreviewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SvgPreviewDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
}

SvgPreviewDialog::~SvgPreviewDialog()
{
    delete ui;
}

}

}
