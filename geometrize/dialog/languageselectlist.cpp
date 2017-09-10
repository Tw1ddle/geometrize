#include "languageselectlist.h"

#include <QDirIterator>
#include <QEvent>
#include <QListWidgetItem>
#include <QString>

#include "common/sharedapp.h"
#include "localization/localization.h"
#include "preferences/globalpreferences.h"

namespace geometrize
{

namespace dialog
{

class LanguageSelectList::LanguageSelectListImpl
{
public:
    LanguageSelectListImpl(LanguageSelectList* pQ) : q{pQ}
    {
        setupLanguageSelect();

        q->connect(q, &QListWidget::currentItemChanged, [this](QListWidgetItem* current, QListWidgetItem* /*previous*/) {
            if(current == nullptr) {
                return;
            }

            const QString isoCode{current->data(Qt::UserRole).toString()};

            geometrize::preferences::GlobalPreferences& prefs{geometrize::preferences::getGlobalPreferences()};
            prefs.setLanguageIsoCode(isoCode.toStdString());

            QLocale::setDefault(QLocale(isoCode));
            geometrize::setTranslatorsForLocale(isoCode);
        });
    }

    LanguageSelectListImpl operator=(const LanguageSelectListImpl&) = delete;
    LanguageSelectListImpl(const LanguageSelectListImpl&) = delete;
    ~LanguageSelectListImpl()
    {
    }

    void onLanguageChange()
    {
        // Could iterate over items and translate the language labels
    }

private:
    void setupLanguageSelect()
    {
        QDirIterator it(geometrize::getAppTranslationResourceDirectory());
        int idx = 0;
        while (it.hasNext()) {
            it.next();
            QString fileName{it.fileName()};
            const QString isoCode{fileName.remove("geometrize_").remove(geometrize::getBinaryTranslationFileExtension())}; // TODO // Extract the ISO 639-1 code from the translation file filename
            addItemAtIndex(++idx, isoCode);
        }

        q->sortItems(Qt::AscendingOrder);
    }

    void addItemAtIndex(const int i, const QString isoCode)
    {
        const QLocale locale{isoCode};
        const QLocale::Language language{locale.language()};
        const QString languageName{locale.languageToString(language)};
        const QIcon icon{geometrize::getFlagIconForIsoCode(isoCode)};

        QListWidgetItem* const item{new QListWidgetItem(icon, languageName)};
        item->setData(Qt::UserRole, isoCode);
        q->insertItem(i, item);
    }

    LanguageSelectList* q;
};

LanguageSelectList::LanguageSelectList(QWidget* parent) : QListWidget(parent), d{std::make_unique<LanguageSelectList::LanguageSelectListImpl>(this)}
{
}

LanguageSelectList::~LanguageSelectList()
{
}

void LanguageSelectList::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }

    QWidget::changeEvent(event);
}

}

}
