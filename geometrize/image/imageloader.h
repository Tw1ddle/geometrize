#pragma once

class QImage;
class QPixmap;
class QString;

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
QImage loadImage(const QString& filePath);

/**
 * @brief loadPixmap Loads a pixmap from the image at the file path. Converts to RGBA8888 format.
 * @param filePath The file path to the pixmap.
 * @return The pixmap loaded from the image file.
 */
QPixmap loadPixmap(const QString& filePath);

}

}
