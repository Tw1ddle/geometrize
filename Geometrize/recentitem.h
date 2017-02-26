#pragma once

#include <QString>

namespace geometrize
{

/**
 * @brief The RecentItem class models an item that was recently interacted with.
 */
class RecentItem
{
public:
    RecentItem(const QString& key, const QString& displayName, const long long int timeStamp);

    enum class Type
    {
        LOCAL_IMAGE,
        LOCAL_CHAISCRIPT,
        REMOTE_RESOURCE,
        UNKNOWN
    };

    /**
     * @brief getTypeForKey Gets the recent item type for the given recent item key.
     * @param key The recent item key.
     * @return The recent item type.
     */
    static RecentItem::Type getTypeForKey(const QString& key);

    QString getKey() const;
    QString getDisplayName() const;
    long long int getTimeStamp() const;

private:
    QString m_key; ///< Key, path, URL or other unique identifier for the item.
    QString m_displayName; ///< Display name of the item, suitable for showing to the user.
    long long int m_timeStamp; ///< Timestamp, millis since epoch, typically when the item was last accessed or used.
};

}
