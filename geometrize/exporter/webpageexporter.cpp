#include "webpageexporter.h"

#include <cassert>
#include <string>
#include <vector>

#include <QFile>
#include <QString>

#include "geometrize/shaperesult.h"

#include "exporter/shapedataexporter.h"

namespace
{

const QString RENDERER_SCRIPT_TAG = "::WEB_EXPORT_RENDERER_SCRIPT_TAG::";
const QString SHAPE_DATA_TAG = "::WEB_EXPORT_DATA_EMBEDDED_TAG::";

std::string exportWebpage(const QString& templatePath, const QString& exportScriptPath, const std::vector<geometrize::ShapeResult>& data)
{
    QFile templateFile{templatePath};
    if(!templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        assert(0 && "Failed to open template file");
        return "";
    }

    QFile exportScript{exportScriptPath};
    if(!exportScript.open(QIODevice::ReadOnly | QIODevice::Text)) {
        assert(0 && "Failed to open export script file");
        return "";
    }

    QString templateSource{templateFile.readAll()};
    const QString exportScriptSource{exportScript.readAll()};
    templateSource.replace(RENDERER_SCRIPT_TAG, exportScriptSource);

    const std::string shapeData{geometrize::exporter::exportShapeData(data, geometrize::exporter::ShapeDataFormat::JSON)};

    // NOTE this embeds the data in a HTML5 data attribute
    // There is a risk this could result in improperly escaped output
    templateSource.replace(SHAPE_DATA_TAG, QString::fromStdString(shapeData));

    return templateSource.toStdString();
}

}

namespace geometrize
{

namespace exporter
{

std::string exportCanvasWebpage(const std::vector<geometrize::ShapeResult>& data)
{
    return exportWebpage(":/web_export/templates/web_export_template.html", ":/web_export/templates/backend_canvas.js", data);
}

std::string exportWebGLWebpage(const std::vector<geometrize::ShapeResult>& data)
{
    return exportWebpage(":/web_export/templates/web_export_template.html", ":/web_export/templates/backend_threejs.js", data);
}

}

}
