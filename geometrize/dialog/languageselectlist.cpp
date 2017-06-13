#include "languageselectlist.h"

#include <QDirIterator>
#include <QString>

namespace geometrize
{

namespace dialog
{

class LanguageSelectList::LanguageSelectListImpl
{
public:
    LanguageSelectListImpl(LanguageSelectList* pQ) : q{pQ}
    {
        populateLanguageMenu();
    }

    LanguageSelectListImpl operator=(const LanguageSelectListImpl&) = delete;
    LanguageSelectListImpl(const LanguageSelectListImpl&) = delete;
    ~LanguageSelectListImpl()
    {
    }

private:
    void populateLanguageMenu()
    {
        // TODO avoid hardcoding
        // TODO start with no items selected
        QDirIterator it(":/translations/app/", QDirIterator::Subdirectories);
        int i = 0;
        while (it.hasNext()) {
            q->insertItem(i, it.next());
            i++;
        }
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
}

}

}
