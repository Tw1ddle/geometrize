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
 * @param backgroundBitmap The initial background bitmap image.
 * @param targetBitmap The final target bitmap.
 * @param data The shape data to export.
 * @param filePath The full path to the GIF image file target (include the filename and .gif extension).
 * @return True if the GIF was saved, else false.
 */
bool exportGIF(const geometrize::Bitmap& backgroundBitmap, const geometrize::Bitmap& targetBitmap, const std::vector<geometrize::ShapeResult>& data, const std::string& filePath);

}

}
