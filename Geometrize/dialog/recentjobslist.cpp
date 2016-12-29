#include "recentjobslist.h"

#include "assert.h"

#include <memory>

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QContextMenuEvent>
#include <QPoint>

#include "recentitems.h"

namespace geometrize
{

namespace dialog
{

class RecentJobsList::RecentJobsListImpl
{
public:
    RecentJobsListImpl(RecentJobsList* pQ) : q{pQ}
    {
    }

    RecentJobsListImpl operator=(const RecentJobsListImpl&) = delete;
    RecentJobsListImpl(const RecentJobsListImpl&) = delete;
    ~RecentJobsListImpl() = default;

    RecentItems* getRecentItems()
    {
        return m_recents;
    }

    void setRecentItems(RecentItems* recents)
    {
        // TODO disconnect?
        m_recents = recents;

        if(m_recents != nullptr) {
            connect(m_recents, &RecentItems::signal_added, [this](const QString& item, const bool preexisting) {
                qDebug() << "ADDED" << " " << item;
                if(preexisting) {
                    const QList<QListWidgetItem*> items{findItems(item, Qt::MatchExactly)};
                    for(auto* item : items) {
                        removeItemWidget(item);
                    }
                } else {
                    QListWidgetItem* widgetItem{new QListWidgetItem(item)};
                    widgetItem->setToolTip(item);
                    addItem(widgetItem);
                }
            });
            connect(m_recents, &RecentItems::signal_cleared, [this]() {
                qDebug() << "CLEARED";
                clear();
            });
            connect(m_recents, &RecentItems::signal_removed, [this](const QString& item) {
                qDebug() << "REMOVED" << " " << item;
                const QList<QListWidgetItem*> items{findItems(item, Qt::MatchExactly)};
                for(auto* item : items) {
                    removeItemWidget(item);
                }
            });
        }

        // TODO sort
        const auto items{recents->getItems()};
        for(auto& item : items) {
            QListWidgetItem* widgetItem{new QListWidgetItem(item.second)};
            widgetItem->setToolTip(item.second);
            q->addItem(widgetItem);
        }
    }

private:
    RecentJobsList* q;
    RecentItems* m_recents;
};

RecentJobsList::RecentJobsList(QWidget* parent) : QListWidget(parent), d{std::make_unique<RecentJobsList::RecentJobsListImpl>(this)}
{
}

void RecentJobsList::setRecentItems(RecentItems* recents)
{
    d->setRecentItems(recents);
}

RecentItems* RecentJobsList::getRecentItems()
{
    return d->getRecentItems();
}

void RecentJobsList::contextMenuEvent(QContextMenuEvent* e)
{
    if(e->reason() != QContextMenuEvent::Mouse) {
        return;
    }
    emit signal_contextMenuRequested(itemAt(e->pos()), mapToGlobal(e->pos()));
}

void RecentJobsList::keyPressEvent(QKeyEvent* e)
{
    QListWidget::keyPressEvent(e);

    if(e->matches(QKeySequence::Copy)) {
        const QList<QListWidgetItem*> items{selectedItems()};
        QStringList strings;
        for(const QListWidgetItem* item : items) {
            strings.push_back(item->text());
        }
        QApplication::clipboard()->setText(strings.join("\n"));
    } else if(e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace) {
        qDebug() << "TODO remove item";
    }
}

}

}
