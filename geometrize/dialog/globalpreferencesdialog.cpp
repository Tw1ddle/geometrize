#include "globalpreferencesdialog.h"
#include "ui_globalpreferencesdialog.h"

#include <array>
#include <cassert>

#include <QColor>
#include <QColorDialog>
#include <QEvent>
#include <QPalette>
#include <QPushButton>

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

        connect(ui->preferenceCategoryList, &QListWidget::currentRowChanged, [this](const int currentRow) {
            ui->preferencePageStack->setCurrentIndex(currentRow);
        });

        populateUi();
        syncUiWithPreferences();
    }
    GlobalPreferencesDialogImpl operator=(const GlobalPreferencesDialogImpl&) = delete;
    GlobalPreferencesDialogImpl(const GlobalPreferencesDialogImpl&) = delete;
    ~GlobalPreferencesDialogImpl()
    {
        getPrefs().save(preferences::getGlobalPreferencesConfigPath());
    }

    void setGeometrizeLogo(const bool enabled)
    {
        getPrefs().setShouldGeometrizeAppLogoOnLaunch(enabled);
    }

    void setPopulateRecents(const bool enabled)
    {
        getPrefs().setShouldPopulateRecentItemsOnLaunch(enabled);
    }

    void setPopulateTemplates(const bool enabled)
    {
        getPrefs().setShouldPopulateTemplatesOnLaunch(enabled);
    }

    void setAutoLoadImageTaskSettings(const bool enabled)
    {
        getPrefs().setShouldAutoLoadImageTaskSettings(enabled);
    }

    void setAutoSaveImageTaskSettings(const bool enabled)
    {
        getPrefs().setShouldAutoSaveImageTaskSettings(enabled);
    }

    void setShowLaunchWindowConsoleByDefault(const bool enabled)
    {
        getPrefs().setShouldShowLaunchConsoleByDefault(enabled);
    }

    void openCustomImageBackgroundBackgroundColorPicker()
    {
        const std::array<std::int32_t, 4> col = getPrefs().getCustomImageTaskBackgroundOverrideColor();
        const QColor currentColor = QColor(col[0], col[1], col[2], col[3]);
        const QColor result = QColorDialog::getColor(currentColor, q, tr("Color Picker"), QColorDialog::ColorDialogOption::ShowAlphaChannel);
        if(!result.isValid()) {
            return;
        }
        setCustomImageTaskBackgroundOverrideColor(result.red(), result.green(), result.blue(), result.alpha());
    }

    void setCustomImageTaskBackgroundOverrideColor(const std::int32_t r, const std::int32_t g, const std::int32_t b, const std::int32_t a)
    {
        getPrefs().setCustomImageTaskBackgroundOverrideColor(r, g, b, a);
        setButtonColor(*ui->selectCustomImageBackgroundColorButton, r, g, b, a);
    }

    void openCustomImageAlphaReplacementColorPicker()
    {
        const std::array<std::int32_t, 4> col = getPrefs().getTargetImageAlphaReplacementColor();
        const QColor currentColor = QColor(col[0], col[1], col[2], col[3]);
        const QColor result = QColorDialog::getColor(currentColor, q, tr("Color Picker"), QColorDialog::ColorDialogOption::ShowAlphaChannel);
        if(!result.isValid()) {
            return;
        }
        setTargetImageAlphaReplacementColor(result.red(), result.green(), result.blue(), result.alpha());
    }

    void setTargetImageAlphaReplacementColor(const std::int32_t r, const std::int32_t g, const std::int32_t b, const std::int32_t a)
    {
        getPrefs().setTargetImageAlphaReplacementColor(r, g, b, a);
        setButtonColor(*ui->selectTargetImageAlphaReplacementColorButton, r, g, b, a);
    }

    void setShouldUseCustomImageTaskBackgroundOverrideColor(const bool enabled)
    {
        getPrefs().setShouldUseCustomImageTaskBackgroundOverrideColor(enabled);
    }

    void setShouldReplaceTargetImageAlphaWithCustomColor(const bool enabled)
    {
        getPrefs().setShouldReplaceTargetImageAlphaWithCustomColor(enabled);
    }

    void setShowVectorResultsByDefault(const bool enabled)
    {
        getPrefs().setShouldShowImageTaskVectorViewByDefault(enabled);
    }

    void setShowPixmapResultsByDefault(const bool enabled)
    {
        getPrefs().setShouldShowImageTaskPixmapViewByDefault(enabled);
    }

    void setShowImageTaskConsoleByDefault(const bool enabled)
    {
        getPrefs().setShouldShowImageTaskConsoleByDefault(enabled);
    }

    void setShowScriptEditorsByDefault(const bool enabled)
    {
        getPrefs().setShouldShowImageTaskScriptEditorByDefault(enabled);
    }

    void setImageTaskImageResize(const bool enabled)
    {
        getPrefs().setImageTaskImageResizeEnabled(enabled);
    }

    void setImageTaskImageResizeThresholdWidth(const int width)
    {
        getPrefs().setImageTaskResizeThreshold(width, getPrefs().getImageTaskResizeThreshold().second);
    }

    void setImageTaskImageResizeThresholdHeight(const int height)
    {
        getPrefs().setImageTaskResizeThreshold(getPrefs().getImageTaskResizeThreshold().first, height);
    }

    void setMaxThreadsPerImageTask(const int value)
    {
        assert(value >= 0);
        getPrefs().setImageTaskMaxThreads(static_cast<unsigned int>(value));
    }

    void onLanguageChange()
    {
        populateUi();
        ui->retranslateUi(q);
    }

private:
    void populateUi()
    {
    }

    geometrize::preferences::GlobalPreferences& getPrefs()
    {
        return geometrize::preferences::getGlobalPreferences();
    }

    void syncUiWithPreferences()
    {
        const geometrize::preferences::GlobalPreferences& prefs{getPrefs()};

        ui->geometrizeLogo->setChecked(prefs.shouldGeometrizeAppLogoOnLaunch());
        ui->populateRecents->setChecked(prefs.shouldPopulateRecentItemsOnLaunch());
        ui->populateTemplates->setChecked(prefs.shouldPopulateTemplatesOnLaunch());

        ui->autoLoadImageTaskSettings->setChecked(prefs.shouldAutoLoadImageTaskSettings());
        ui->autoSaveImageTaskSettings->setChecked(prefs.shouldAutoSaveImageTaskSettings());

        ui->useCustomImageTaskBackgroundColor->setChecked(prefs.shouldUseCustomImageTaskBackgroundOverrideColor());

        const std::array<std::int32_t, 4> bgCol = prefs.getCustomImageTaskBackgroundOverrideColor();
        setButtonColor(*ui->selectCustomImageBackgroundColorButton, bgCol[0], bgCol[1], bgCol[2], bgCol[3]);

        ui->replaceTargetImageAlphaWithCustomColor->setChecked(prefs.shouldUseImageAlphaReplacementColor());

        const std::array<std::int32_t, 4> alphaCol = prefs.getTargetImageAlphaReplacementColor();
        setButtonColor(*ui->selectTargetImageAlphaReplacementColorButton, alphaCol[0], alphaCol[1], alphaCol[2], alphaCol[3]);

        ui->showLaunchWindowConsoleByDefault->setChecked(prefs.shouldShowLaunchConsoleByDefault());
        ui->showVectorResultsByDefault->setChecked(prefs.shouldShowImageTaskVectorViewByDefault());
        ui->showPixmapResultsByDefault->setChecked(prefs.shouldShowImageTaskPixmapViewByDefault());
        ui->showImageTaskConsoleByDefault->setChecked(prefs.shouldShowImageTaskConsoleByDefault());
        ui->showScriptEditorsByDefault->setChecked(prefs.shouldShowImageTaskScriptEditorByDefault());

        ui->imageResize->setChecked(prefs.isImageTaskImageResizeEnabled());
        ui->resizeWidth->setValue(prefs.getImageTaskResizeThreshold().first);
        ui->resizeHeight->setValue(prefs.getImageTaskResizeThreshold().second);
        ui->maxThreadsPerImageTask->setValue(prefs.getImageTaskMaxThreads());
    }

    void setButtonColor(QPushButton& button, const std::int32_t r, const std::int32_t g, const std::int32_t b, const std::int32_t a)
    {
        const QString buttonStyle = "QPushButton {border: 0.05em solid lightgray; background-color: rgba(" +
                QString::number(r) + "," + QString::number(g) + "," + QString::number(b) + "," + QString::number(static_cast<float>(a) / 255.0f) + ") }";
        button.setStyleSheet(buttonStyle);
        button.update();
    }

    std::unique_ptr<Ui::GlobalPreferencesDialog> ui;
    GlobalPreferencesDialog* q;
};

GlobalPreferencesDialog::GlobalPreferencesDialog(QWidget* parent) : QDialog(parent), d{std::make_unique<GlobalPreferencesDialog::GlobalPreferencesDialogImpl>(this)}
{
}

GlobalPreferencesDialog::~GlobalPreferencesDialog()
{
}

void GlobalPreferencesDialog::on_geometrizeLogo_toggled(const bool checked)
{
    d->setGeometrizeLogo(checked);
}

void GlobalPreferencesDialog::on_populateRecents_toggled(const bool checked)
{
    d->setPopulateRecents(checked);
}

void GlobalPreferencesDialog::on_populateTemplates_toggled(const bool checked)
{
    d->setPopulateTemplates(checked);
}

void GlobalPreferencesDialog::on_useCustomImageTaskBackgroundColor_toggled(const bool checked)
{
    d->setShouldUseCustomImageTaskBackgroundOverrideColor(checked);
}

void GlobalPreferencesDialog::on_selectCustomImageBackgroundColorButton_clicked()
{
    d->openCustomImageBackgroundBackgroundColorPicker();
}

void GlobalPreferencesDialog::on_replaceTargetImageAlphaWithCustomColor_toggled(bool enabled)
{
    d->setShouldReplaceTargetImageAlphaWithCustomColor(enabled);
}

void GlobalPreferencesDialog::on_selectTargetImageAlphaReplacementColorButton_clicked()
{
    d->openCustomImageAlphaReplacementColorPicker();
}

void GlobalPreferencesDialog::on_autoLoadImageTaskSettings_toggled(const bool checked)
{
    d->setAutoLoadImageTaskSettings(checked);
}

void GlobalPreferencesDialog::on_autoSaveImageTaskSettings_toggled(const bool checked)
{
    d->setAutoSaveImageTaskSettings(checked);
}

void GlobalPreferencesDialog::on_showLaunchWindowConsoleByDefault_toggled(const bool checked)
{
    d->setShowLaunchWindowConsoleByDefault(checked);
}

void GlobalPreferencesDialog::on_showVectorResultsByDefault_toggled(const bool checked)
{
    d->setShowVectorResultsByDefault(checked);
}

void GlobalPreferencesDialog::on_showPixmapResultsByDefault_toggled(const bool checked)
{
    d->setShowPixmapResultsByDefault(checked);
}

void GlobalPreferencesDialog::on_showImageTaskConsoleByDefault_toggled(const bool checked)
{
    d->setShowImageTaskConsoleByDefault(checked);
}

void GlobalPreferencesDialog::on_showScriptEditorsByDefault_toggled(const bool checked)
{
    d->setShowScriptEditorsByDefault(checked);
}

void GlobalPreferencesDialog::on_imageResize_toggled(const bool checked)
{
    d->setImageTaskImageResize(checked);
}

void GlobalPreferencesDialog::on_resizeWidth_valueChanged(const int value)
{
    d->setImageTaskImageResizeThresholdWidth(value);
}

void GlobalPreferencesDialog::on_resizeHeight_valueChanged(const int value)
{
    d->setImageTaskImageResizeThresholdHeight(value);
}

void GlobalPreferencesDialog::on_maxThreadsPerImageTask_valueChanged(const int value)
{
    d->setMaxThreadsPerImageTask(value);
}

void GlobalPreferencesDialog::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QDialog::changeEvent(event);
}

}

}
