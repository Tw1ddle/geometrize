#include "folderselectionwidget.h"
#include "ui_folderselectionwidget.h"

namespace geometrize
{

namespace dialog
{

FolderSelectionWidget::FolderSelectionWidget(QWidget* parent) : QWidget(parent),
    ui(new Ui::FolderSelectionWidget)
{
    ui->setupUi(this);
}

FolderSelectionWidget::~FolderSelectionWidget()
{
    delete ui;
}

}

}
