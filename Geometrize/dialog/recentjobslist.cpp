#include "recentjobslist.h"

#include "assert.h"

#include <QDebug>
#include <QContextMenuEvent>

namespace geometrize
{

namespace dialog
{

RecentJobsList::RecentJobsList(QWidget *parent) : QListWidget(parent), m_recents{nullptr}
{
}

void RecentJobsList::setRecentItems(RecentItems* recents)
{
    // TODO disconnect

    m_recents = recents;

    // TODO sort
    const auto items{recents->getItems()};
    for(auto& item : items) {
        addItem(item.second);
    }

    if(m_recents != nullptr) {
        connect(m_recents, &RecentItems::signal_added, [this](const QString& item, const bool preexisting) {
            qDebug() << "ADDED" << " " << item;
            if(preexisting) {
                const QList<QListWidgetItem*> items{findItems(item, Qt::MatchExactly)};
                for(auto* item : items) {
                    removeItemWidget(item);
                }
            } else {
                addItem(item);
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
}

void RecentJobsList::contextMenuEvent(QContextMenuEvent* e)
{
    // TODO

    // if you only want the mouse context events (like right click)
    if(e->reason() != QContextMenuEvent::Mouse) return;

    // get the item unter the mouse cursor
    QListWidgetItem * clickedItem = itemAt(e->pos());

    // do what you like here
    // e.g. show a dialog to ask whether the item should be deleted
    // or show a widget with delete button (you can position it freely with setGeometry(...))
    // etc.

   // emit signal_contextMenuRequested(nullptr); // TODO
}

}

}
