#include "recentjobslist.h"

#include "assert.h"

#include <QDebug>

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

}

}
