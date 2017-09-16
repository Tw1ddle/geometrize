#include "folderselectionwidget.h"
#include "ui_folderselectionwidget.h"

#include <QEvent>

namespace geometrize
{

namespace dialog
{

FolderSelectionWidget::FolderSelectionWidget(QWidget* parent) : QWidget(parent),
    ui(new Ui::FolderSelectionWidget)
{
    ui->setupUi(this);
    populateUi();
}

FolderSelectionWidget::~FolderSelectionWidget()
{
    delete ui;
}

void FolderSelectionWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        populateUi();
        ui->retranslateUi(this);
    }
    QWidget::changeEvent(event);
}

void FolderSelectionWidget::populateUi()
{
}

}

}
