#include "gifexporter.h"

#include <algorithm>
#include <cstdint>

#include <QImage>
#include <QRgb>
#include <QSet>
#include <QVector>

#include "qgifimage.h"

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/core.h"
#include "geometrize/rasterizer/rasterizer.h"
#include "geometrize/rasterizer/scanline.h"
#include "geometrize/shape/shape.h"
#include "geometrize/shaperesult.h"

#include "image/imageloader.h"
#include "imageexporter.h"

namespace geometrize
{

namespace exporter
{

bool exportGIF(
        const std::vector<geometrize::ShapeResult>& data,
        std::uint32_t inputWidth,
        std::uint32_t inputHeight,
        std::uint32_t outputWidth,
        std::uint32_t outputHeight,
        const std::string& filePath)
{
    QGifImage gif;

    std::vector<QImage> images;
    for(std::size_t i = 0; i < data.size(); i++) {
        const std::vector<geometrize::ShapeResult> shapes(data.begin(), data.begin() + i);
        images.emplace_back(geometrize::exporter::renderSvgShapeDataToImage(shapes, inputWidth, inputHeight, outputWidth, outputHeight));
    }

    // TODO implement shape/frame skip and other options to limit size
    for(std::uint32_t i = 0; i < images.size(); i++) {
        std::int32_t delayMs = std::max(20, static_cast<std::int32_t>(1000 / (i + 1)));
        if(i == images.size() - 1U) {
            delayMs = 2000; // Extra delay at end of animation
        }
        gif.addFrame(images[i], QPoint(0, 0), delayMs);
    }

    return gif.save(QString::fromStdString(filePath));
}

}

}
