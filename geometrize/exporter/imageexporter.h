#pragma once

#include <string>

class QImage;

namespace geometrize
{
class Bitmap;
}

namespace geometrize
{

namespace exporter
{

/**
 * @brief exportImage Exports the image data to an image format that Qt supports (depending on the file extension).
 * @param bitmapData The image data to save as an image file.
 * @param filePath The full path to the bitmap image file target (include the filename and file extension).
 * @return True if the image was saved, else false.
 */
bool exportImage(const geometrize::Bitmap& bitmapData, const std::string& filePath);

/**
 * @brief exportImage Exports the image data to an image format that Qt supports (depending on the file extension).
 * @param image The image to save as an image file.
 * @param filePath The full path to the bitmap image file target (include the filename and file extension).
 * @return True if the image was saved, else false.
 */
bool exportImage(const QImage& image, const std::string& filePath);

}

}
