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
        connect(m_recents, &RecentItems::signal_added, []() {
            qDebug() << "ADDED";
        });
        connect(m_recents, &RecentItems::signal_cleared, []() {
            qDebug() << "CLEARED";
        });
        connect(m_recents, &RecentItems::signal_removed, []() {
            qDebug() << "REMOVED";
        });
    }
}

}

}
