#include "recentitems.h"

#include "assert.h"

#include <QDateTime>
#include <QDebug>
#include <QHash>
#include <QSettings>

namespace geometrize
{

const QString RecentItems::RECENT_FILES_SETTINGS_GROUP = "recently_opened_files";

class RecentItems::RecentItemsImpl
{
public:
    RecentItemsImpl(const QString group) : m_group{group}
    {
        assert(m_group.length() > 0 && "Base group cannot be empty");
        assert(!m_group.contains('/') && "Base group must not contain forward slashes");
        assert(!m_group.contains('\\') && "Base group must not contain backslashes");

        #ifdef QT_DEBUG
        QSettings settings;
        settings.beginGroup(m_group);
        qDebug() << "Saved recent files keys: " << settings.allKeys();
        settings.endGroup();
        #endif
    }

    ~RecentItemsImpl() = default;
    RecentItemsImpl& operator=(const RecentItemsImpl&) = delete;
    RecentItemsImpl(const RecentItemsImpl&) = delete;

    bool contains(const QString& value)
    {
        QSettings settings;

        settings.beginGroup(m_group);
        const QStringList keys{settings.childKeys()};
        settings.endGroup();

        for(const QString& key : keys) {
            if(key.startsWith(QString(qHash(value)))) {
                return true;
            }
        }

        return false;
    }

    void addItem(const QString& value)
    {
        const QString key{getItemKey(value)};

        QSettings settings;
        settings.beginGroup(m_group);

        if(!settings.contains(key)) {
            // Add the new key
            settings.setValue(key, value);
        } else {
            // Replace the old key
            const QStringList keys{settings.childKeys()};
            for(const QString& k : keys) {
                if(settings.value(k, "").toString() == value) {
                    settings.remove(k);
                    settings.setValue(key, value);
                }
            }
        }

        settings.endGroup();
    }

    void removeItem(const QString& value)
    {
        QSettings settings;
        settings.beginGroup(m_group);
        const QStringList keys{settings.childKeys()};
        for(const QString& key : keys) {
            if(settings.value(key, "").toString() == value) {
                settings.remove(key);
            }
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
    static QString getItemKey(const QString& value)
    {
        return msSinceEpoch() + "_" + getStringHash(value);
    }

    static QString getStringHash(const QString& s)
    {
        return QString::number(qHash(s));
    }

    static QString msSinceEpoch()
    {
        return QString::number(QDateTime::currentMSecsSinceEpoch());
    }

    const QString m_group; ///< The base path group for storing the recent items in settings e.g. "recent_image_paths", "recent_video_names" etc.
};

RecentItems::RecentItems(const QString& group) : d{std::make_unique<RecentItemsImpl>(group)}
{
    emit signal_created();
}

RecentItems::~RecentItems()
{
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
