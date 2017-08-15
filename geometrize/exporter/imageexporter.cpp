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

bool exportRasterizedSvg(
        const std::vector<geometrize::ShapeResult>& shapes,
        const std::uint32_t inputWidth,
        const std::uint32_t inputHeight,
        const std::uint32_t outputWidth,
        const std::uint32_t outputHeight,
        const geometrize::rgba backgroundColor,
        const std::string& filePath)
{
    const std::string data{geometrize::exporter::exportSVG(shapes, inputWidth, inputHeight, backgroundColor)};
    const QByteArray arrayData(data.c_str(), static_cast<int>(data.length()));
    QSvgRenderer renderer;
    renderer.load(arrayData);

    if(!renderer.isValid()) {
        assert(0 && "SVG renderer has invalid state");
        return false;
    }

    QPainter painter;
    QImage image(outputWidth, outputHeight, QImage::Format_RGBA8888);
    image.fill(0);

    painter.begin(&image);
    renderer.render(&painter);
    painter.end();

    return geometrize::exporter::exportImage(image, filePath);
}

}

}
