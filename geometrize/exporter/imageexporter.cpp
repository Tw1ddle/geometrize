#include "imageexporter.h"

#include <cassert>
#include <string>

#include <QByteArray>
#include <QImage>
#include <QPainter>
#include <QSvgRenderer>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/commonutil.h"
#include "geometrize/exporter/svgexporter.h"
#include "geometrize/shaperesult.h"

#include "image/imageloader.h"

namespace geometrize
{

namespace exporter
{

bool exportBitmap(const geometrize::Bitmap& bitmap, const std::string& filePath)
{
    const QImage image{geometrize::image::createImage(bitmap)};
    return exportImage(image, filePath);
}

bool exportImage(const QImage& image, const std::string& filePath)
{
    return image.save(QString::fromStdString(filePath));
}

bool exportImages(const std::vector<QImage>& images, const std::string& targetDir, const std::string& baseFilename, const std::string& fileExtension)
{
    for(std::size_t i = 0; i < images.size(); i++) {
        const std::string path{targetDir + baseFilename + "_" + std::to_string(i) + fileExtension};
        if(!images[i].save(QString::fromStdString(path))) {
            return false;
        }
    }
    return true;
}

QImage renderSvgShapeDataToImage(
        const std::vector<geometrize::ShapeResult>& shapes,
        const std::uint32_t inputWidth,
        const std::uint32_t inputHeight,
        const std::uint32_t outputWidth,
        const std::uint32_t outputHeight)
{
    const std::string data{geometrize::exporter::exportSVG(shapes, inputWidth, inputHeight)};
    const QByteArray arrayData(data.c_str(), static_cast<int>(data.length()));
    QSvgRenderer renderer;
    renderer.load(arrayData);

    if(!renderer.isValid()) {
        assert(0 && "SVG renderer is in an invalid state");
        return QImage();
    }

    QPainter painter;
    QImage image(outputWidth, outputHeight, QImage::Format_RGBA8888);
    image.fill(0);

    painter.begin(&image);
    renderer.render(&painter);
    painter.end();

    return image;
}

bool exportRasterizedSvg(
        const std::vector<geometrize::ShapeResult>& shapes,
        const std::uint32_t inputWidth,
        const std::uint32_t inputHeight,
        const std::uint32_t outputWidth,
        const std::uint32_t outputHeight,
        const std::string& filePath)
{
    const QImage image{geometrize::exporter::renderSvgShapeDataToImage(shapes, inputWidth, inputHeight, outputWidth, outputHeight)};
    return geometrize::exporter::exportImage(image, filePath);
}

bool exportRasterizedSvgs(
        const std::vector<geometrize::ShapeResult>& shapes,
        std::uint32_t inputWidth,
        std::uint32_t inputHeight,
        std::uint32_t outputWidth,
        std::uint32_t outputHeight,
        const std::string& targetDir,
        const std::string& baseFilename,
        const std::string& fileExtension)
{
    for(std::size_t i = 0; i < shapes.size(); i++) {
        const std::string path{targetDir + "/" + baseFilename + "_" + std::to_string(i) + fileExtension};

        const std::vector<geometrize::ShapeResult> shapeSubset(shapes.begin(), shapes.begin() + i);
        if(!exportRasterizedSvg(shapeSubset, inputWidth, inputHeight, outputWidth, outputHeight, path)) {
            return false;
        }
    }
    return true;
}

}

}
