#pragma once

#include <string>
#include <vector>

namespace geometrize
{
class Bitmap;

struct ShapeResult;
}

namespace geometrize
{

namespace exporter
{

/**
 * @brief exportGIF Exports shape data to a GIF image.
 * @param data The shape data to export.
 * @param inputWidth The width of the canvas each frame will be rendered to.
 * @param inputHeight The height of the canvas each frame will be rendered to.
 * @param outputWidth The width of the image each frame will be rasterized into.
 * @param outputHeight The height of the image each frame will be rasterized into.
 * @param filePath The full path to the GIF image file target (include the filename and .gif extension).
 * @return True if the GIF was saved, else false.
 */
bool exportGIF(
        const std::vector<geometrize::ShapeResult>& data,
        std::uint32_t inputWidth,
        std::uint32_t inputHeight,
        std::uint32_t outputWidth,
        std::uint32_t outputHeight,
        const std::string& filePath);

}

}
