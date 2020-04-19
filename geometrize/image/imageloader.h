#pragma once

#include <string>

class QImage;
class QPixmap;

namespace geometrize
{
class Bitmap;
}

namespace geometrize
{

namespace image
{

/**
 * @brief createBitmap Creates a bitmap data object from the given image.
 * @param image The image to create the bitmap data from.
 * @return The new bitmap data.
 */
Bitmap createBitmap(const QImage& image);

/**
 * @brief imageToBitmapWithDownscaling Creates a bitmap data object from the given image, downscaling in the process based on global preferences for image resizing
 * @param image The image to create the bitmap data from.
 * @return The new bitmap data.
 */
geometrize::Bitmap convertImageToBitmapWithDownscaling(const QImage& image);

/**
 * @brief createImage Creates an image from the bitmap data object. Assumes RGBA8888 format.
 * @param data The bitmap data, RGBA8888 bytes (must be a multiple of 4).
 * @return The pixmap created from the bytes data.
 */
QImage createImage(const Bitmap& data);

/**
 * @brief createPixmap Creates a pixmap from the bitmap data object. Assumes RGBA8888 format.
 * @param data The bitmap data, RGBA8888 bytes (must be a multiple of 4).
 * @return The pixmap created from the bytes data.
 */
QPixmap createPixmap(const Bitmap& data);

/**
 * @brief loadImage Loads an image from the image at the file path. Converts to RGBA8888 format.
 * @param filePath The file path to the image.
 * @return The image loaded from the image file.
 */
QImage loadImage(const std::string& filePath);

/**
 * @brief convertImageToRgba8888 Returns a copy of the image in the RGBA8888 format.
 * @param image The image to convert.
 * @return A copy of the given image in RGBA8888 format.
 */
QImage convertImageToRgba8888(const QImage& image);

/**
 * @brief loadPixmap Loads a pixmap from the image at the file path. Converts to RGBA8888 format.
 * @param filePath The file path to the pixmap.
 * @return The pixmap loaded from the image file.
 */
QPixmap loadPixmap(const std::string& filePath);

}

}
