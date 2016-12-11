#pragma once

#include <QListWidget>

#include "recentitems.h"

namespace geometrize
{

namespace dialog
{

class RecentJobsList : public QListWidget
{
public:
    explicit RecentJobsList(QWidget *parent = 0);

    /**
     * @brief setRecentItems Sets the items that the list will keep track of.
     * @param items The list of items, the recent jobs list does not take ownership of this.
     */
    void setRecentItems(RecentItems* items);

private:
    RecentItems* m_recents;
};

}

}
