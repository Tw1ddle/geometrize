#include "languageselectwidget.h"
#include "ui_languageselectwidget.h"

#include "common/sharedapp.h"
#include "preferences/globalpreferences.h"
#include "localization.h"

namespace geometrize
{

namespace dialog
{

class LanguageSelectWidget::LanguageSelectWidgetImpl
{
public:
    LanguageSelectWidgetImpl(LanguageSelectWidget* pQ) : q{pQ}, ui{std::make_unique<Ui::LanguageSelectWidget>()}
    {
        ui->setupUi(q);

        setup();
    }

    LanguageSelectWidgetImpl operator=(const LanguageSelectWidgetImpl&) = delete;
    LanguageSelectWidgetImpl(const LanguageSelectWidgetImpl&) = delete;
    ~LanguageSelectWidgetImpl()
    {
    }

    void onLanguageChange()
    {
        setup();
    }

private:
    void setup()
    {
        const geometrize::preferences::GlobalPreferences& prefs{geometrize::common::app::SharedApp::get().getGlobalPreferences()};
        const std::string languageCode{prefs.getLanguageIsoCode()};
        const QLocale locale{QString::fromStdString(languageCode)};

        ui->currentLanguageNameLabel->setText(locale.languageToString(locale.language()));

        const QIcon icon{geometrize::getFlagIconForIsoCode(locale.name())};
        ui->currentLanguageFlagLabel->setPixmap(icon.pixmap(icon.availableSizes().last()));
    }

    LanguageSelectWidget* q;
    std::unique_ptr<Ui::LanguageSelectWidget> ui;
};

LanguageSelectWidget::LanguageSelectWidget(QWidget* parent) : QWidget(parent), d{std::make_unique<LanguageSelectWidget::LanguageSelectWidgetImpl>(this)}
{
}

LanguageSelectWidget::~LanguageSelectWidget()
{
}

void LanguageSelectWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    } else {
        QWidget::changeEvent(event);
    }
}

}

}
