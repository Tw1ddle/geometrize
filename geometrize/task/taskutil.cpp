#include "taskutil.h"

#include <QDir>
#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QUrl>

#include <optional>

#include "chaiscript/chaiscript.hpp"

#include "common/formatsupport.h"
#include "common/searchpaths.h"
#include "common/util.h"
#include "network/completionhandlers.h"
#include "network/downloader.h"
#include "network/networkactions.h"
#include "recents/recentitems.h"
#include "script/scriptrunner.h"
#include "task/imagetask.h"
#include "task/imagetaskcreator.h"

namespace geometrize
{

namespace util
{

void openTask(const QString& urlStr, bool addToRecents)
{
    if(urlStr.length() == 0) {
        return;
    }

    if(addToRecents) {
        geometrize::getRecentItems().add(urlStr, urlStr);
    }

    const QUrl url{QUrl::fromUserInput(urlStr)};
    if(url.isLocalFile()) {
        geometrize::task::createImageTaskAndWindow(url.toLocalFile().toStdString(), url.toLocalFile().toStdString(), std::nullopt);
        return;
    }

    std::vector<std::string> imageExtensions{format::getReadableImageFileExtensions(true)};
    for(const std::string& ext : imageExtensions) {
        if(url.toString().endsWith(QString::fromStdString(ext), Qt::CaseInsensitive)) {
            network::downloadImage(url, network::completionhandlers::onImageDownloadComplete);
            return;
        }
    }

    network::downloadWebpage(url, network::completionhandlers::onWebpageDownloadComplete);
}

void openTasks(const QStringList& urls, const bool addToRecents)
{
    for(const QString& url : urls) {
        openTask(url, addToRecents);
    }
}

bool openTemplate(chaiscript::ChaiScript& engine, const std::string& templateFolder)
{
    const std::vector<std::string> scripts{util::getScriptsForPath(templateFolder)};
    if(scripts.empty()) {
        // Could not find script for template, so assume it's an image task
        const std::string imageFile{util::getFirstFileWithExtensions(templateFolder, format::getReadableImageFileExtensions(true))};
        if(imageFile.empty()) {
            return false;
        }

        // Apply settings file if a default one is available
        const auto prefs = [&templateFolder]() -> std::optional<preferences::ImageTaskPreferences> {
            const std::string settingsFile{geometrize::searchpaths::getDefaultTaskSettingsFilename()};
            if(util::directoryContainsFile(templateFolder, settingsFile)) {
                const std::string settingsPath{QDir(QString::fromStdString(templateFolder)).filePath(QString::fromStdString(settingsFile)).toStdString()};
                const preferences::ImageTaskPreferences prefs(settingsPath);
                return prefs;
            }
            return std::nullopt;
        }();

        task::createImageTaskAndWindow(imageFile, imageFile, prefs);

        return true;
    }

    const std::string script{util::readFileAsString(scripts.front())};
    engine.set_global(chaiscript::var(templateFolder), "templateDirectory");
    geometrize::script::runScript(script, engine);

    return true;
}

TemplateManifest getTemplateManifest(const std::string& templateFolder)
{
    return TemplateManifest(util::getFirstFileWithExtensions(templateFolder, format::getSupportedTemplateManifestFileExtensions()));
}

}

}
