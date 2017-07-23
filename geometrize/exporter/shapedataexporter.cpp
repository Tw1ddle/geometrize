#include "shapedataexporter.h"

#include <cassert>
#include <string>
#include <vector>

#include "geometrize/exporter/shapearrayexporter.h"
#include "geometrize/exporter/shapejsonexporter.h"
#include "geometrize/shaperesult.h"

namespace geometrize
{

namespace exporter
{

std::string exportShapeData(const std::vector<geometrize::ShapeResult>& data, const ShapeDataFormat format)
{
    switch(format) {
    case ShapeDataFormat::JSON:
        return exporter::exportShapeJson(data);
    case ShapeDataFormat::CUSTOM_ARRAY:
        return exporter::exportShapeArray(data);
    }

    assert(0 && "Unsupported shape data format specified, will default to exporting JSON");
    return exporter::exportShapeJson(data);
}

}

}
