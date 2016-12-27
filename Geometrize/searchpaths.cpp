#include "searchpaths.h"

#include <QCoreApplication>

namespace geometrize
{

namespace searchpaths
{

QString getApplicationDirectoryPath()
{
    return QCoreApplication::applicationDirPath();
}

QStringList getScriptSearchPaths()
{
    // TODO some paths for debug builds, some for final release?

    return { "/../../scripts", "/scripts" };
}

QStringList getTemplateSearchpaths()
{
    return { "/../../templates", "/templates" };
}

}

}
