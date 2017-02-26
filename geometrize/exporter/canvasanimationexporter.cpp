#include "canvasanimationexporter.h"

#include <assert.h>
#include <string>
#include <vector>

#include <QFile>
#include <QString>

#include "geometrize/shaperesult.h"

#include "exporter/shapedataexporter.h"

namespace geometrize
{

namespace exporter
{

const QString SHAPE_DATA_TAG = "::SHAPE_DATA_TAG::";

std::string exportCanvasAnimation(const std::vector<geometrize::ShapeResult>& data)
{
    QFile file{":/web_templates/canvas_export.html"};
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        assert(0 && "Failed to open WebGL template");
        return "";
    }

    QString templateSource{file.readAll()};

    const std::string shapeData{exporter::exportShapeData(data, ShapeDataFormat::CUSTOM_ARRAY)};
    templateSource.replace(SHAPE_DATA_TAG, QString::fromStdString(shapeData));

    return templateSource.toStdString();
}

}

}
