#include "languageselectlist.h"

#include <QCoreApplication>
#include <QDirIterator>
#include <QEvent>
#include <QListWidgetItem>
#include <QString>

#include "common/sharedapp.h"
#include "preferences/globalpreferences.h"
#include "localization.h"

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

        q->connect(q, &QListWidget::itemPressed, [this](QListWidgetItem* item) {
            const QString isoCode{item->data(Qt::UserRole).toString()};

            geometrize::preferences::GlobalPreferences& prefs{geometrize::common::app::SharedApp::get().getGlobalPreferences()};
            prefs.setLanguageIsoCode(isoCode.toStdString());

            QLocale::setDefault(QLocale(isoCode));
            geometrize::installTranslatorsForLocale(*QCoreApplication::instance(), isoCode);
        });
    }

    LanguageSelectListImpl operator=(const LanguageSelectListImpl&) = delete;
    LanguageSelectListImpl(const LanguageSelectListImpl&) = delete;
    ~LanguageSelectListImpl()
    {
    }

    void onLanguageChange()
    {
        q->clear();
        setupLanguageSelect();
    }

private:
    void setupLanguageSelect()
    {
        // TODO avoid hardcoding
        // TODO start with no items selected
        QDirIterator it(":/translations/app/");
        int idx = 0;
        while (it.hasNext()) {
            it.next();
            const QString isoCodeFromFilename{it.fileName().remove(".qm", Qt::CaseInsensitive)}; // Extract the ISO 639-1 code from the translation file filename
            const QLocale locale{isoCodeFromFilename};

            const QLocale::Language language{locale.language()};
            const QString languageName{locale.languageToString(language)};

            const QIcon icon{geometrize::getFlagIconForIsoCode(isoCodeFromFilename)};
            QListWidgetItem* item{new QListWidgetItem(icon, languageName)};
            item->setData(Qt::UserRole, isoCodeFromFilename);
            q->insertItem(++idx, item);
        }

        q->sortItems(Qt::AscendingOrder);
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
    } else {
        QListWidget::changeEvent(event);
    }
}

}

}
