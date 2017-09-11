#include "globalpreferencesdialog.h"
#include "ui_globalpreferencesdialog.h"

#include <cassert>

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

    void setShowLaunchWindowConsoleByDefault(const bool enabled)
    {
        getPrefs().setShouldShowLaunchConsoleByDefault(enabled);
    }

    void setShowVectorResultsByDefault(const bool enabled)
    {
        getPrefs().setShouldShowImageJobVectorViewByDefault(enabled);
    }

    void setShowPixmapResultsByDefault(const bool enabled)
    {
        getPrefs().setShouldShowImageJobPixmapViewByDefault(enabled);
    }

    void setShowImageJobConsoleByDefault(const bool enabled)
    {
        getPrefs().setShouldShowImageJobConsoleByDefault(enabled);
    }

    void setShowScriptEditorsByDefault(const bool enabled)
    {
        getPrefs().setShouldShowImageJobScriptEditorByDefault(enabled);
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
        assert(value >= 0);
        getPrefs().setImageJobMaxThreads(static_cast<unsigned int>(value));
    }

private:
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

        ui->showLaunchWindowConsoleByDefault->setChecked(prefs.shouldShowLaunchConsoleByDefault());
        ui->showVectorResultsByDefault->setChecked(prefs.shouldShowImageJobVectorViewByDefault());
        ui->showPixmapResultsByDefault->setChecked(prefs.shouldShowImageJobPixmapViewByDefault());
        ui->showImageJobConsoleByDefault->setChecked(prefs.shouldShowImageJobConsoleByDefault());
        ui->showScriptEditorsByDefault->setChecked(prefs.shouldShowImageJobScriptEditorByDefault());

        ui->imageResize->setChecked(prefs.isImageJobImageResizeEnabled());
        ui->resizeWidth->setValue(prefs.getImageJobResizeThreshold().first);
        ui->resizeHeight->setValue(prefs.getImageJobResizeThreshold().second);
        ui->maxThreadsPerImageJob->setValue(prefs.getImageJobMaxThreads());
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

void GlobalPreferencesDialog::on_showImageJobConsoleByDefault_toggled(const bool checked)
{
    d->setShowImageJobConsoleByDefault(checked);
}

void GlobalPreferencesDialog::on_showScriptEditorsByDefault_toggled(const bool checked)
{
    d->setShowScriptEditorsByDefault(checked);
}

void GlobalPreferencesDialog::on_imageResize_toggled(const bool checked)
{
    d->setImageJobImageResize(checked);
}

void GlobalPreferencesDialog::on_resizeWidth_valueChanged(const int value)
{
    d->setImageJobImageResizeThresholdWidth(value);
}

void GlobalPreferencesDialog::on_resizeHeight_valueChanged(const int value)
{
    d->setImageJobImageResizeThresholdHeight(value);
}

void GlobalPreferencesDialog::on_maxThreadsPerImageJob_valueChanged(const int value)
{
    d->setMaxThreadsPerImageJob(value);
}

}

}
