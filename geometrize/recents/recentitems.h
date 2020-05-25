#pragma once

#include <memory>

#include <QObject>

#include "recentitem.h"

namespace geometrize
{

/**
 * @brief The RecentItems class encapsulates a list of items that were recently interacted with.
 * The class keeps a record of these items stored in preferences. Useful for keeping track of recently opened files.
 */
class RecentItems : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief RecentItems Creates a new RecentItems instance.
     * @param group The base path group to use for storing the recent items in settings e.g. "recent_image_paths", "recent_video_names" etc.
     * @param maxItems The maximum number of items to load. The most recent items will be loaded initially, and the oldest items will be replaced when new items are added over the limit.
     */
    RecentItems(const QString& group, unsigned int maxItems);

    RecentItems& operator=(RecentItems&) = delete;
    RecentItems(RecentItems&) = delete;
    RecentItems(RecentItems&&) = delete;
    virtual ~RecentItems();

    /**
     * @brief getItems Gets the recent items.
     * @return A list of the recent items, sorted from most recently interacted with to oldest.
     */
    QList<RecentItem> getItems() const;

    /**
     * @brief add Adds an item to the recent items.
     * @param itemId A unique id for the item (e.g. a complete file path).
     * @param itemDisplayName A display name for the item (e.g. a file name).
     */
    void add(const QString& itemId, const QString& itemDisplayName);

    /**
     * @brief remove Removes an item from the recent items.
     * @param itemId The item id to remove.
     */
    void remove(const QString& itemId);

    /**
     * @brief clear Removes all items from the recent items.
     */
    void clear();

signals:
    /**
     * @brief signal_added is emitted after an item is added.
     * @param item The item that was added.
     */
    void signal_added(const RecentItem& item);

    /**
     * @brief signal_removed is emitted after an item is removed.
     * @param itemId The id of the item that was removed.
     */
    void signal_removed(const QString& itemId);

    /**
     * @brief signal_cleared is emitted after the recent items are cleared.
     */
    void signal_cleared();

private:
    friend void swap(RecentItems& first, RecentItems& second);

    class RecentItemsImpl;
    std::unique_ptr<RecentItemsImpl> d;
};

/**
 * @brief getRecentItems Gets a reference to the app's recently opened files.
 * @return A reference to the app's recently opened files.
 */
RecentItems& getRecentItems();

}
