#pragma once

#include <vector>

class QImage;

namespace geometrize
{

namespace exporter
{

/**
 * @brief exportGIF Exports shape data to a GIF image.
 * @param images The images to export.
 * @param filePath The full path to the GIF image file target (include the filename and .gif extension).
 * @return True if the GIF was saved, else false.
 */
bool exportGIF(const std::vector<QImage>& images, const std::string& filePath);

}

}
