#pragma once

#include <string>
#include <vector>

namespace geometrize
{
struct ShapeResult;
}

namespace geometrize
{

namespace exporter
{

enum class ShapeDataFormat
{
    JSON,
    CUSTOM_ARRAY
};

/**
 * @brief exportShapeData Exports shape data to a specified format (JSON or a custom textual array format).
 * @param data The shape data to export.
 * @param format The format to save to data in.
 * @return A string containing the exported shape data, empty if the format was unrecognized.
 */
std::string exportShapeData(const std::vector<geometrize::ShapeResult>& data, ShapeDataFormat format);

}

}
