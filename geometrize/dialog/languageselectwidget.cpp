#include "languageselectwidget.h"
#include "ui_languageselectwidget.h"

#include <cassert>

#include <QEvent>
#include <QList>
#include <QIcon>

#include "localization/localization.h"
#include "preferences/globalpreferences.h"

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

        populateUi();
    }

    LanguageSelectWidgetImpl operator=(const LanguageSelectWidgetImpl&) = delete;
    LanguageSelectWidgetImpl(const LanguageSelectWidgetImpl&) = delete;
    ~LanguageSelectWidgetImpl()
    {
    }

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    void populateUi()
    {
        const geometrize::preferences::GlobalPreferences& prefs{geometrize::preferences::getGlobalPreferences()};
        const std::string localeName{prefs.getLocaleName()};
        const QLocale locale{QString::fromStdString(localeName)};
        ui->currentLanguageNameLabel->setText(locale.nativeLanguageName());
        ui->currentCountryNameLabel->setText(locale.nativeCountryName());

        const QString extractedLocaleName{locale.name()};
        const QIcon icon{geometrize::getFlagIconForLocaleCode(extractedLocaleName)};
        assert(!icon.isNull());
        assert(!icon.availableSizes().empty());
        if(!icon.isNull() && !icon.availableSizes().empty()) {
            ui->currentLanguageFlagLabel->setPixmap(icon.pixmap(icon.availableSizes().last()));
        }

        ui->currentLocaleIconLabel->setPixmap(QPixmap(":/icons/locate.png"));
        ui->currentLocaleNameLabel->setText(locale.name());
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
    }
    QWidget::changeEvent(event);
}

}

}
