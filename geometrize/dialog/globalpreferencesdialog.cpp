#include "globalpreferencesdialog.h"
#include "ui_globalpreferencesdialog.h"

#include "preferences/globalpreferences.h"

namespace geometrize
{

namespace dialog
{

class GlobalPreferencesDialog::GlobalPreferencesDialogImpl
{
public:
    GlobalPreferencesDialogImpl(GlobalPreferencesDialog* pQ) : ui{std::make_unique<Ui::GlobalPreferencesDialog>()}, q{pQ}
    {
        q->setWindowFlags(q->windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
        ui->setupUi(q);

        // TODO
        connect(ui->preferenceCategoryList, &QListWidget::currentRowChanged, [this](const int currentRow) {
            ui->preferencePageStack->setCurrentIndex(currentRow);
        });

        syncUiWithPreferences();
    }
    GlobalPreferencesDialogImpl operator=(const GlobalPreferencesDialogImpl&) = delete;
    GlobalPreferencesDialogImpl(const GlobalPreferencesDialogImpl&) = delete;
    ~GlobalPreferencesDialogImpl()
    {
        getPrefs().save(preferences::getGlobalPreferencesConfigPath());
    }

    void setImageJobImageResize(const bool enabled)
    {
        getPrefs().setImageJobImageResizeEnabled(enabled);
    }

    void setImageJobImageResizeThresholdWidth(const int width)
    {
        getPrefs().setImageJobResizeThreshold(width, getPrefs().getImageJobResizeThreshold().second);
    }

    void setImageJobImageResizeThresholdHeight(const int height)
    {
        getPrefs().setImageJobResizeThreshold(getPrefs().getImageJobResizeThreshold().first, height);
    }

    void setMaxThreadsPerImageJob(const int value)
    {
        // TODO
    }

private:
    geometrize::preferences::GlobalPreferences& getPrefs()
    {
        return geometrize::preferences::getGlobalPreferences();
    }

    void syncUiWithPreferences()
    {
        ui->imageResizeCheckbox->setChecked(getPrefs().isImageJobImageResizeEnabled());
        ui->resizeWidthSpinBox->setValue(getPrefs().getImageJobResizeThreshold().first);
        ui->resizeHeightSpinBox->setValue(getPrefs().getImageJobResizeThreshold().second);
    }

    GlobalPreferencesDialog* q;
    std::unique_ptr<Ui::GlobalPreferencesDialog> ui;
};

GlobalPreferencesDialog::GlobalPreferencesDialog(QWidget* parent) : QDialog(parent), d{std::make_unique<GlobalPreferencesDialog::GlobalPreferencesDialogImpl>(this)}
{
}

GlobalPreferencesDialog::~GlobalPreferencesDialog()
{
}

void GlobalPreferencesDialog::on_imageResizeCheckbox_toggled(bool checked)
{
    d->setImageJobImageResize(checked);
}

void GlobalPreferencesDialog::on_resizeWidthSpinBox_valueChanged(int value)
{
    d->setImageJobImageResizeThresholdWidth(value);
}

void GlobalPreferencesDialog::on_resizeHeightSpinBox_valueChanged(int value)
{
    d->setImageJobImageResizeThresholdHeight(value);
}

void GlobalPreferencesDialog::on_maxThreadsPerImageJobSpinBox_valueChanged(int value)
{
    d->setMaxThreadsPerImageJob(value);
}

}

}
