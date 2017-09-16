#include "completionbox.h"

#include <QCompleter>
#include <QEvent>
#include <QStringListModel>

namespace geometrize
{

namespace dialog
{

class StringListModel : public QStringListModel
{
public:
    void setStringList(const QStringList& list)
    {
        QStringListModel::setStringList(list);
    }
};

class CompletionBox::CompletionBoxImpl
{
public:
    CompletionBoxImpl(CompletionBox* pQ) : q{pQ}
    {
        m_completer.setCompletionMode(QCompleter::PopupCompletion);
        m_completer.setCaseSensitivity(Qt::CaseInsensitive);
        m_completer.setFilterMode(Qt::MatchStartsWith);
        m_completer.setModel(&m_model);
        q->setCompleter(&m_completer);

        populateUi();
    }

    void addToCompletionList(const QString& item)
    {
        m_model.insertRows(m_model.rowCount(), 1);
        m_model.setData(m_model.index(m_model.rowCount() - 1), item);
    }

    void setCompletionList(const QStringList& completionList)
    {
        m_model.setStringList(completionList);
    }

    void onLanguageChange()
    {
        populateUi();
    }

private:
    void populateUi()
    {
    }

    QCompleter m_completer;
    StringListModel m_model;
    CompletionBox* q;
};

CompletionBox::CompletionBox(QWidget* parent) : QLineEdit(parent), d{std::make_unique<CompletionBox::CompletionBoxImpl>(this)}
{
}

CompletionBox::~CompletionBox()
{
}

void CompletionBox::setCompletionList(const QStringList& completionList)
{
    d->setCompletionList(completionList);
}

void CompletionBox::addToCompletionList(const QString& item)
{
    d->addToCompletionList(item);
}

void CompletionBox::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QLineEdit::changeEvent(event);
}

}

}
