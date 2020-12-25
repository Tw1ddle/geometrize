#pragma once

#include <string>

#include <QStringList>

#include "manifest/templatemanifest.h"

class QImage;
class QString;

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{

namespace util
{

void openTask(const QString& urlStr, bool addToRecents);

void openTasks(const QStringList& urls, bool addToRecents);

bool openTemplate(chaiscript::ChaiScript& engine, const std::string& templateFolder);

TemplateManifest getTemplateManifest(const std::string& templateFolder);

}

}
