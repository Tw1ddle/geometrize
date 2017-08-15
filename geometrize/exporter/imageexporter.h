#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "geometrize/bitmap/rgba.h"

class QImage;

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
 * @brief exportBitmap Exports the Geometrize bitmap data to an image format that Qt supports (depending on the file extension).
 * @param bitmap The image data to save as an image file.
 * @param filePath The full path to the image file target (include the filename and file extension).
 * @return True if the image was saved, else false.
 */
bool exportBitmap(const geometrize::Bitmap& bitmap, const std::string& filePath);

/**
 * @brief exportImage Exports the image data to an image format that Qt supports (depending on the file extension).
 * @param image The image to save as an image file.
 * @param filePath The full path to the image file target (include the filename and file extension).
 * @return True if the image was saved, else false.
 */
bool exportImage(const QImage& image, const std::string& filePath);

/**
 * @brief exportRasterizedSvg Exports the shape data as an image by rasterizing an SVG rendered from the shape data.
 * @param shapes The shape data to export.
 * @param inputWidth The width of the canvas the SVG will be rendered to.
 * @param inputHeight The height of the canvas the SVG will be rendered to.
 * @param outputWidth The width of the image the SVG will be rasterized into.
 * @param outputHeight The height of the image the SVG will be rasterized into.
 * @param backgroundColor The background colour of the image.
 * @param filePath The full path to the image file target (include the filename and file extension).
 * @return True if the image was saved, else false.
 */
bool exportRasterizedSvg(
        const std::vector<geometrize::ShapeResult>& shapes,
        std::uint32_t inputWidth,
        std::uint32_t inputHeight,
        std::uint32_t outputWidth,
        std::uint32_t outputHeight,
        const geometrize::rgba backgroundColor,
        const std::string& filePath);

}

}
