#include "jobutil.h"

#include "chaiscript/chaiscript.hpp"

#include "formatsupport.h"
#include "script/scriptrunner.h"
#include "util.h"

namespace geometrize
{

namespace util
{

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
