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
 * @brief exportImages Exports the image data to an image format that Qt supports (depending on the file extension).
 * @param images The images to save as image files.
 * @param targetDir The target directory.
 * @param baseFilename The base file name.
 * @param fileExtension The file extension (".jpg", ".png").
 * @return True if the images were saved, else false.
 */
bool exportImages(const std::vector<QImage>& images, const std::string& targetDir, const std::string& baseFilename, const std::string& fileExtension);

/**
 * @brief renderSvgShapeDataToImage Renders the given shape data to an image via an SVG.
 * @param shapes The shape data to render.
 * @param inputWidth The width of the canvas the SVG will be rendered to.
 * @param inputHeight The height of the canvas the SVG will be rendered to.
 * @param outputWidth The width of the image the SVG will be rasterized into.
 * @param outputHeight The height of the image the SVG will be rasterized into.
 * @return The image the shape data was rendered to.
 */
QImage renderSvgShapeDataToImage(
        const std::vector<geometrize::ShapeResult>& shapes,
        const std::uint32_t inputWidth,
        const std::uint32_t inputHeight,
        const std::uint32_t outputWidth,
        const std::uint32_t outputHeight);

/**
 * @brief exportRasterizedSvg Exports the shape data as an image by rasterizing an SVG rendered from the shape data.
 * @param shapes The shape data to export.
 * @param inputWidth The width of the canvas the SVG will be rendered to.
 * @param inputHeight The height of the canvas the SVG will be rendered to.
 * @param outputWidth The width of the image the SVG will be rasterized into.
 * @param outputHeight The height of the image the SVG will be rasterized into.
 * @param filePath The full path to the image file target (include the filename and file extension).
 * @return True if the image was saved, else false.
 */
bool exportRasterizedSvg(
        const std::vector<geometrize::ShapeResult>& shapes,
        std::uint32_t inputWidth,
        std::uint32_t inputHeight,
        std::uint32_t outputWidth,
        std::uint32_t outputHeight,
        const std::string& filePath);

/**
 * @brief exportRasterizedSvgs Exports the shape data to images by rasterizing SVGs rendered from the shape data.
 * Exports one image for each shape i.e. an image with one shape, two shapes, three shapes.
 * @param shapes The shape data to export.
 * @param inputWidth The width of the canvas the SVG will be rendered to.
 * @param inputHeight The height of the canvas the SVG will be rendered to.
 * @param outputWidth The width of the image the SVG will be rasterized into.
 * @param outputHeight The height of the image the SVG will be rasterized into.
 * @param targetDir The target directory.
 * @param baseFilename The base file name.
 * @param fileExtension The file extension (".jpg", ".png").
 * @return True if the images were saved, else false.
 */
bool exportRasterizedSvgs(
        const std::vector<geometrize::ShapeResult>& shapes,
        std::uint32_t inputWidth,
        std::uint32_t inputHeight,
        std::uint32_t outputWidth,
        std::uint32_t outputHeight,
        const std::string& targetDir,
        const std::string& baseFilename,
        const std::string& fileExtension);

}

}
