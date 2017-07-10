#include "recentitems.h"

#include <assert.h>
#include <atomic>

#include <QDateTime>
#include <QSettings>

namespace geometrize
{

const QString RecentItems::RECENTLY_OPENED_ITEMS_SETTINGS_GROUP = "recently_opened_items";
const unsigned int RecentItems::MAX_RECENTLY_OPENED_ITEMS_COUNT = 75;

class RecentItems::RecentItemsImpl
{
public:
    RecentItemsImpl(const QString group, unsigned int maxItems) : m_group{group}, m_maxItems{maxItems}
    {
        assert(m_group.length() > 0 && "Base group cannot be empty");
        assert(!m_group.contains('/') && "Base group must not contain forward slashes");
        assert(!m_group.contains('\\') && "Base group must not contain backslashes");
    }

    ~RecentItemsImpl() = default;
    RecentItemsImpl& operator=(const RecentItemsImpl&) = default;
    RecentItemsImpl(const RecentItemsImpl&) = default;

    QList<RecentItem> getItems() const
    {
        QSettings settings;
        settings.beginGroup(m_group);
        const QStringList childGroups{settings.childGroups()};
        QList<RecentItem> recentItems;
        for(const QString& group : childGroups) {
            settings.beginGroup(group);
            const RecentItem item(settings.value(ID_KEY, "").toString(), settings.value(DISPLAY_NAME_KEY, "").toString(), settings.value(TIME_STAMP_KEY, 0).toLongLong());
            settings.endGroup();
            recentItems.push_back(item);
        }
        settings.endGroup();

        std::sort(recentItems.begin(), recentItems.end(), [](const RecentItem& a, const RecentItem& b) {
            return a.getTimeStamp() < b.getTimeStamp();
        });
        return recentItems;
    }

    bool contains(const QString& value) const
    {
        QSettings settings;
        settings.beginGroup(m_group);
        const QStringList childGroups{settings.childGroups()};
        for(const QString& group : childGroups) {
            settings.beginGroup(group);
            if(settings.value(ID_KEY) == value) {
                return true;
            }
            settings.endGroup();
        }
        settings.endGroup();

        return false;
    }

    RecentItem addItem(const QString& itemId, const QString& itemDisplayName)
    {
        const RecentItem item(itemId, itemDisplayName, msSinceEpoch());
        const QString group{getItemGroup()};

        QSettings settings;
        settings.beginGroup(m_group);
        settings.beginGroup(group);

        settings.setValue(ID_KEY, QVariant(item.getKey()));
        settings.setValue(DISPLAY_NAME_KEY, QVariant(item.getDisplayName()));
        settings.setValue(TIME_STAMP_KEY, QVariant(item.getTimeStamp()));

        settings.endGroup();
        settings.endGroup();

        return item;
    }

    void removeItem(const QString& value)
    {
        QSettings settings;
        settings.beginGroup(m_group);
        const QStringList childGroups{settings.childGroups()};
        for(const QString& group : childGroups) {
            settings.beginGroup(group);
            if(settings.value(ID_KEY) == value) {
                settings.remove("");
            }
            settings.endGroup();
        }
        settings.endGroup();
    }

    void clearItems()
    {
        QSettings settings;
        settings.beginGroup(m_group);
        settings.remove("");
        settings.endGroup();
    }

    unsigned int getItemCount() const
    {
        QSettings settings;
        settings.beginGroup(m_group);
        const int count{settings.childGroups().length()};
        settings.endGroup();

        return count;
    }

    unsigned int getMaxItemCount() const
    {
        return m_maxItems;
    }

    QString getOldestItemKey() const
    {
        QList<RecentItem> items{getItems()};
        if(items.empty()) {
            return "";
        }
        return items.front().getKey();
    }

private:
    static QString getItemGroup()
    {
        return msSinceEpochAsString() + "_" + getId();
    }

    static QString msSinceEpochAsString()
    {
        return QString::number(msSinceEpoch());
    }

    static long long int msSinceEpoch()
    {
        return QDateTime::currentMSecsSinceEpoch();
    }

    static QString getId()
    {
        static std::atomic_int id = 0;
        return QString::number(id++);
    }

    const QString m_group; ///< The base path group for storing the recent items in settings e.g. "recent_image_paths" etc.
    const unsigned int m_maxItems; ///< The maximum number of items.

    static const QString ID_KEY; ///< Key for the path or URL to the item.
    static const QString DISPLAY_NAME_KEY; ///< Key for the display name of the item.
    static const QString TIME_STAMP_KEY; ///< Key for the timestamp of when the item was added or replaced.
};

const QString RecentItems::RecentItemsImpl::ID_KEY{"id"};
const QString RecentItems::RecentItemsImpl::DISPLAY_NAME_KEY{"displayname"};
const QString RecentItems::RecentItemsImpl::TIME_STAMP_KEY{"timestamp"};

void swap(RecentItems& first, RecentItems& second)
{
    std::swap(first.d, second.d);
}

RecentItems::RecentItems(const QString& group, const unsigned int maxItems) : d{std::make_unique<RecentItemsImpl>(group, maxItems)}
{
}

RecentItems::~RecentItems()
{
}

RecentItems& RecentItems::operator=(RecentItems& other)
{
    swap(*this, other);
    return *this;
}

RecentItems::RecentItems(RecentItems& other)
{
    swap(*this, other);
}

RecentItems::RecentItems(RecentItems&& other)
{
    swap(*this, other);
}

QList<RecentItem> RecentItems::getItems() const
{
    return d->getItems();
}

void RecentItems::add(const QString& itemId, const QString& itemDisplayName)
{
    if(d->getItemCount() > d->getMaxItemCount()) {
        remove(d->getOldestItemKey());
    }

    const bool preexisting{d->contains(itemId)};
    if(!preexisting) {
        const RecentItem item{d->addItem(itemId, itemDisplayName)};
        emit signal_added(item);
    }
}

void RecentItems::remove(const QString& itemId)
{
    d->removeItem(itemId);
    emit signal_removed(itemId);
}

void RecentItems::clear()
{
    d->clearItems();
    emit signal_cleared();
}

}
