#include "svgpreviewdialog.h"
#include "ui_svgpreviewdialog.h"

#include "geometrize/shaperesult.h"

namespace geometrize
{

namespace dialog
{

SvgPreviewDialog::SvgPreviewDialog(const std::vector<geometrize::ShapeResult>& shapes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SvgPreviewDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);

    ui->graphicsView->drawSvg(shapes);
}

SvgPreviewDialog::~SvgPreviewDialog()
{
    delete ui;
}

}

}
