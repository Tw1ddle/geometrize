#include "recentitem.h"

namespace geometrize
{

RecentItem::RecentItem(const QString& key, const QString& displayName, const long long int timeStamp) : m_key{key}, m_displayName{displayName}, m_timeStamp{timeStamp}
{

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
