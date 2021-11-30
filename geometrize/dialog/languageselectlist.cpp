#include "languageselectlist.h"

#include <QDirIterator>
#include <QEvent>
#include <QListWidgetItem>
#include <QString>

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

            QString isoCode{current->data(Qt::UserRole).toString()};
            geometrize::setLocaleAndUserInterfaceLanguage(isoCode);
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
    // Iterate over all of the embedded qm files and extract the ISO codes from the filenames
    void setupLanguageSelect()
    {
        const QStringList supportedLocaleCodes = geometrize::getSupportedLocaleCodes();
        int idx = 0;
        for (const QString& s : supportedLocaleCodes) {
            addItemAtIndex(++idx, s);
        }
        q->sortItems(Qt::AscendingOrder);
    }

    void addItemAtIndex(const int i, const QString& localeCode)
    {
        const QLocale locale{localeCode};
        const QLocale::Language language{locale.language()};
        const QString languageName{locale.languageToString(language)};
        const QIcon icon{geometrize::getFlagIconForLocaleCode(localeCode)};

        QListWidgetItem* const item{new QListWidgetItem(icon, languageName)};
        item->setData(Qt::UserRole, localeCode);
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
