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
    const QUrl url{QUrl::fromLocalFile(key)};
    if(url.isValid()) {
        if(url.isLocalFile()) {
            if(url.toString().endsWith(".chai", Qt::CaseInsensitive)) {
                return RecentItem::Type::LOCAL_CHAISCRIPT;
            } else {
                const QImageReader reader(key);
                if(reader.imageFormat() != QImage::Format_Invalid) {
                    return RecentItem::Type::LOCAL_IMAGE;
                }
            }
        } else {
            return RecentItem::Type::REMOTE_RESOURCE;
        }
    }

    const QFileInfo fileInfo{key};
    if(fileInfo.exists() && fileInfo.isFile()) {
        if(key.endsWith(".chai", Qt::CaseInsensitive)) {
            return RecentItem::Type::LOCAL_CHAISCRIPT;
        } else {
            const QImageReader reader(key);
            if(reader.imageFormat() != QImage::Format_Invalid) {
                return RecentItem::Type::LOCAL_IMAGE;
            }
        }
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
