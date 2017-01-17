#pragma once

#include <string>

#include "templatemanifest.h"

class QPixmap;
class QString;
class QStringList;
class QWidget;

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{

namespace job
{
class ImageJobContext;
}

}

namespace geometrize
{

namespace util
{

// Image job creation/opening
void openJobs(const QStringList& urls);
job::ImageJobContext* createImageJob(QWidget* parent, const QString& displayName, const QPixmap& pixmap);

bool openTemplate(chaiscript::ChaiScript& engine, const std::string& templateFolder);

TemplateManifest getTemplateManifest(const std::string& templateFolder);

}

}
