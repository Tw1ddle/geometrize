#include "searchpaths.h"

#include <QCoreApplication>

namespace geometrize
{

namespace searchpaths
{

std::string getApplicationDirectoryPath()
{
    return QCoreApplication::applicationDirPath().toStdString();
}

std::vector<std::string> getScriptSearchPaths()
{
    // TODO some paths for debug builds, some for final release?

    return { "/../../scripts", "/scripts" };
}

std::vector<std::string> getTemplateSearchPaths()
{
    return { "/../../templates", "/templates" };
}

}

}
