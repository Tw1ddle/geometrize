#pragma once

#include <QListWidget>

#include "recentitems.h"

namespace geometrize
{

namespace dialog
{

/**
 * @brief The RecentJobsList class models the UI for a list of recently opened jobs/files.
 */
class RecentJobsList : public QListWidget
{
public:
    explicit RecentJobsList(QWidget *parent = 0);

    /**
     * @brief setRecentItems Sets the items that the list will keep track of.
     * @param items The list of items, the recent jobs list does not take ownership of this.
     */
    void setRecentItems(RecentItems* items);

signals:
    void signal_contextMenuRequested(QPoint* pos);

private:
    virtual void contextMenuEvent(QContextMenuEvent* e) override;

    RecentItems* m_recents;
};

}

}
