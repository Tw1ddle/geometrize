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

/**
 * @brief exportCanvasAnimation Exports shape data to an animated canvas-based webpage.
 * @param data The shape data to export.
 * @return A string containing the content of the HTML canvas-based webpage.
 */
std::string exportCanvasAnimation(const std::vector<geometrize::ShapeResult>& data);

}

}
