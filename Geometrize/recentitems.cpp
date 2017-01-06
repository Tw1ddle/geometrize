#include "recentitems.h"

#include <assert.h>
#include <atomic>

#include <QDateTime>
#include <QSettings>

namespace geometrize
{

const QString RecentItems::RECENTLY_OPENED_ITEMS_SETTINGS_GROUP = "recently_opened_items";

class RecentItems::RecentItemsImpl
{
public:
    RecentItemsImpl(const QString group) : m_group{group}
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
        const QStringList keys{settings.childKeys()};
        QList<RecentItem> recentItems;
        for(const QString& key : keys) {
            settings.beginGroup(key);
            RecentItem item;
            item.m_displayName = settings.value(DISPLAY_NAME_KEY, "").toString();
            item.m_itemKey = settings.value(ITEM_PATH_KEY, "").toString();
            item.m_timeStamp = settings.value(TIME_STAMP_KEY, 0).toLongLong();
            settings.endGroup();
            recentItems.push_back(item);
        }
        settings.endGroup();

        return recentItems;
    }

    bool contains(const QString& value)
    {
        QSettings settings;
        settings.beginGroup(m_group);
        const QStringList keys{settings.childKeys()};
        for(const QString& key : keys) {
            settings.beginGroup(key);
            if(settings.value(ITEM_PATH_KEY) == value) {
                return true;
            }
            settings.endGroup();
        }
        settings.endGroup();

        return false;
    }

    void addItem(const QString& value)
    {
        const QString key{getItemKey()};

        QSettings settings;
        settings.beginGroup(m_group);
        settings.beginGroup(key);

        settings.setValue(ITEM_PATH_KEY, QVariant(value));
        settings.setValue(TIME_STAMP_KEY, QVariant(msSinceEpochAsString()));
        settings.setValue(DISPLAY_NAME_KEY, QVariant(value));

        settings.endGroup();
        settings.endGroup();
    }

    void removeItem(const QString& value)
    {
        QSettings settings;
        settings.beginGroup(m_group);
        const QStringList keys{settings.childKeys()};
        for(const QString& key : keys) {
            settings.beginGroup(key);
            if(settings.value(ITEM_PATH_KEY) == value) {
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

private:
    static QString getItemKey()
    {
        return msSinceEpochAsString() + "_" + getId();
    }

    static QString msSinceEpochAsString()
    {
        return QString::number(QDateTime::currentMSecsSinceEpoch());
    }

    static QString getId()
    {
        static std::atomic_int id = 0;
        return QString::number(id++);
    }

    const QString m_group; ///< The base path group for storing the recent items in settings e.g. "recent_image_paths" etc.

    static const QString ITEM_PATH_KEY; ///< Key for the path or URL to the item.
    static const QString TIME_STAMP_KEY; ///< Key for the timestamp of when the item was added or replaced.
    static const QString DISPLAY_NAME_KEY; ///< Key for the display name of the item.
};

const QString RecentItems::RecentItemsImpl::ITEM_PATH_KEY{"path"};
const QString RecentItems::RecentItemsImpl::TIME_STAMP_KEY{"timestamp"};
const QString RecentItems::RecentItemsImpl::DISPLAY_NAME_KEY{"displayName"};

void swap(RecentItems& first, RecentItems& second)
{
    std::swap(first.d, second.d);
}

RecentItems::RecentItems(const QString& group) : d{std::make_unique<RecentItemsImpl>(group)}
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

void RecentItems::add(const QString& itemId)
{
    const bool preexisting{d->contains(itemId)};

    d->addItem(itemId);

    emit signal_added(itemId, preexisting);
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
