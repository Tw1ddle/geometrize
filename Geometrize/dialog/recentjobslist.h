#pragma once

#include <QListWidget>

namespace geometrize
{

class RecentItems;

namespace dialog
{

/**
 * @brief The RecentJobsList class models the UI for a list of recently opened jobs/files.
 */
class RecentJobsList : public QListWidget
{
    Q_OBJECT
public:
    explicit RecentJobsList(QWidget *parent = 0);

    /**
     * @brief setRecentItems Sets the items that the list will keep track of.
     * @param items The list of items, the recent jobs list does not take ownership of this.
     */
    void setRecentItems(RecentItems* items);

    /**
     * @brief getRecentItems Gets the recent items object that the recent jobs list currently reflects.
     * @return The recent items object.
     */
    RecentItems* getRecentItems();

signals:
    void signal_contextMenuRequested(QListWidgetItem* item, QPoint pos);

private:
    virtual void contextMenuEvent(QContextMenuEvent* e) override;
    virtual void keyPressEvent(QKeyEvent* e) override;

    RecentItems* m_recents;
};

}

}
