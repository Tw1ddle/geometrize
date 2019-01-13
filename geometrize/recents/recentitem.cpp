#include "recentitem.h"

#include <QFileInfo>
#include <QImageReader>
#include <QUrl>

namespace geometrize
{

RecentItem::RecentItem(const QString& key, const QString& displayName, const long long int timeStamp) : m_key{key}, m_displayName{displayName}, m_timeStamp{timeStamp}
{
}

RecentItem::Type RecentItem::getTypeForKey(const QString& key)
{
    const QUrl url{QUrl::fromUserInput(key)};
    if(url.scheme() == "file") {
        if(key.endsWith(".chai", Qt::CaseInsensitive)) {
            return RecentItem::Type::LOCAL_CHAISCRIPT;
        } else {
            return RecentItem::Type::LOCAL_IMAGE;
        }
    }

    if(url.isValid()) {
        return RecentItem::Type::REMOTE_RESOURCE;
    }

    return RecentItem::Type::UNKNOWN;
}

QString RecentItem::getKey() const
{
    return m_key;
}

QString RecentItem::getDisplayName() const
{
    return m_displayName;
}

long long int RecentItem::getTimeStamp() const
{
    return m_timeStamp;
}

}
