#include "searchpaths.h"

#include <string>
#include <vector>

#include <QCoreApplication>
#include <QStandardPaths>
#include <QStringList>

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
    return { "/../../scripts", "/scripts" };
}

std::vector<std::string> getTemplateSearchPaths()
{
    return { ":/templates/templates", "/../../templates", "/templates" };
}

std::string getDefaultTaskSettingsFilename()
{
    return "settings.json";
}

std::string getAutosaveTaskSettingsFilename(const std::string& hashOfFirstTargetImage)
{
    return "image_task_settings_" + hashOfFirstTargetImage + ".json";
}

std::vector<std::string> getAutosaveTaskSettingsSearchPaths()
{
    const QStringList locs = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    std::vector<std::string> stdLocs;
    for(const auto& loc : locs) {
        stdLocs.push_back(loc.toStdString());
    }
    return stdLocs;
}

}

}
