#include "preferencestabdialog.h"
#include "ui_preferencestabdialog.h"

#include <QVariant>

namespace geometrize
{

namespace dialog
{

PreferencesTabDialog::PreferencesTabDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::PreferencesTabDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);

    connect(ui->preferenceCategoryList, &QListWidget::currentRowChanged, [this](const int currentRow) {
        ui->preferencePageStack->setCurrentIndex(currentRow);
    });
}

PreferencesTabDialog::~PreferencesTabDialog()
{
    delete ui;
}

}

}
