#include "webglanimationexporter.h"

#include <string>
#include <vector>

#include "geometrize/shaperesult.h"

#include "exporter/shapedataexporter.h"

namespace geometrize
{

namespace exporter
{

std::string exportWebGLAnimation(const std::vector<geometrize::ShapeResult>& data)
{
    const std::string shapeData{exporter::exportShapeData(data, ShapeDataFormat::CUSTOM_ARRAY)};

    // TODO insert the shape data into the source code for a webpage
    std::string pageSource;

    return pageSource;
}

}

}
