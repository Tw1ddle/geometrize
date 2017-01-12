#include "globalpreferencestabdialog.h"
#include "ui_globalpreferencestabdialog.h"

#include <QVariant>

namespace geometrize
{

namespace dialog
{

GlobalPreferencesTabDialog::GlobalPreferencesTabDialog(QWidget* parent) : QDialog(parent), ui(new Ui::GlobalPreferencesTabDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);

    connect(ui->preferenceCategoryList, &QListWidget::currentRowChanged, [this](const int currentRow) {
        ui->preferencePageStack->setCurrentIndex(currentRow);
    });
}

GlobalPreferencesTabDialog::~GlobalPreferencesTabDialog()
{
    delete ui;
}

}

}
