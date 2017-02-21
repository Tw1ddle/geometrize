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
 * @brief exportWebGLAnimation Exports shape data to an animated WebGL-based webpage.
 * @param data The shape data to export.
 * @return A string containing the content of the HTML WebGL-based webpage.
 */
std::string exportWebGLAnimation(const std::vector<geometrize::ShapeResult>& data);

}

}
