#include "jobutil.h"

#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QUrl>

#include "chaiscript/chaiscript.hpp"

#include "geometrize/bitmap/bitmapdata.h"
#include "geometrize/bitmap/rgba.h"

#include "job/imagejobcontext.h"
#include "formatsupport.h"
#include "dialog/imagejobwindow.h"
#include "network/completionhandlers.h"
#include "network/downloader.h"
#include "network/networkactions.h"
#include "script/scriptrunner.h"
#include "util.h"

namespace geometrize
{

namespace util
{

job::ImageJobContext* createImageJob(QWidget* parent, const QString& displayName, const QPixmap& pixmap)
{
    dialog::ImageJobWindow* imageJobWindow = new dialog::ImageJobWindow(parent);
    imageJobWindow->show(); // TODO cleanup
    imageJobWindow->setWindowTitle(displayName);

    // TODO QImage instead?
    //BitmapData data(pixmap.data_ptr().);

    return new job::ImageJobContext(displayName, BitmapData(10, 10, rgba{0, 0, 0, 0})); // TODO
}

void openJobs(const QStringList& urls)
{
    if(urls.empty()) {
        return;
    }

    for(const QString& s : urls) {
        const QUrl url{QUrl::fromUserInput(s)};
        if(url.isLocalFile()) {
            createImageJob(nullptr, url.toString(), url.toLocalFile());
        } else if(url.toString().endsWith(".png")) { // TODO need list of supported formats
            network::downloadImage(url, network::completionhandlers::onImageDownloadComplete);
        } else {
            network::downloadWebpage(url, network::completionhandlers::onWebpageDownloadComplete);
        }
    }
}

bool openTemplate(chaiscript::ChaiScript& engine, const std::string& templateFolder)
{
    const std::vector<std::string> scripts{util::getScriptsForPath(templateFolder)};

    if(scripts.empty()) {
        assert(0 && "Could not find script for template");
        return false;
    }

    const std::string script{util::readFileAsString(scripts.front())};
    engine.set_global(chaiscript::var(templateFolder), "templateDirectory");
    geometrize::script::runScript(script, engine, nullptr);

    return true;
}

TemplateManifest getTemplateManifest(const std::string& templateFolder)
{
    return TemplateManifest(util::getFirstFileWithExtensions(templateFolder, format::getSupportedTemplateManifestFileExtensions()));
}

}

}
