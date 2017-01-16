#pragma once

#include <string>

#include "templatemanifest.h"

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{

namespace util
{

bool openTemplate(chaiscript::ChaiScript& engine, const std::string& templateFolder);

TemplateManifest getTemplateManifest(const std::string& templateFolder);

}

}
