#ifndef RECENTITEMS_H
#define RECENTITEMS_H

#include <memory>

#include <QObject>

namespace geometrize
{

/**
 * @brief The RecentItems class encapsulates a list of items that were recently interacted with.
 * The class keeps a record of these items stored in preferences. Useful for keeping track of recently opened files etc.
 */
class RecentItems : public QObject
{
    Q_OBJECT
public:
    static const QString RECENT_FILES_SETTINGS_GROUP; ///< The base path group for keeping track of recently opened files.

    /**
     * @brief RecentItems Creates a new RecentItems instance.
     * @param group The base path group to use for storing the recent items in settings e.g. "recent_image_paths", "recent_video_names" etc.
     */
    RecentItems(const QString& group);

    RecentItems& operator=(const RecentItems&) = delete;
    RecentItems(const RecentItems&) = delete;
    ~RecentItems();

    /**
     * @brief add Adds an item to the recent items.
     * @param itemId A unique id for the item (e.g. a complete file path).
     */
    void add(const QString& itemId);

    /**
     * @brief remove Removes an item from the recent items.
     * @param itemId The unique id for the item.
     */
    void remove(const QString& itemId);

    /**
     * @brief clear Removes all items from the recent items.
     */
    void clear();

signals:
    /**
     * @brief signal_created is emitted after the RecentItems instance is created for the first time.
     */
    void signal_created();

    /**
     * @brief signal_added is emitted after an item is added.
     * @param itemId The unique id for the item.
     * @param preexisting Whether the item already existed in the recent items.
     */
    void signal_added(const QString& itemId, bool preexisting);

    /**
     * @brief signal_removed is emitted after an item is removed.
     * @param itemId The unique id for the item.
     */
    void signal_removed(const QString& itemId);

    /**
     * @brief signal_cleared is emitted after the recent items are cleared.
     */
    void signal_cleared();

private:
    class RecentItemsImpl;
    std::unique_ptr<RecentItemsImpl> d;
};

}

#endif // RECENTITEMS_H
