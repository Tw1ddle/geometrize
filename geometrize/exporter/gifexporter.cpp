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
#include "geometrize/scanline.h"
#include "geometrize/shape/shape.h"
#include "geometrize/shaperesult.h"

#include "image/imageloader.h"

namespace geometrize
{

namespace exporter
{

bool exportGIF(const geometrize::Bitmap& backgroundBitmap, const geometrize::Bitmap& targetBitmap, const std::vector<geometrize::ShapeResult>& data, const std::string& filePath)
{
    QGifImage gif;

    // Create an array of all the frames.
    // Start from the initial bitmap, drawing all the shapes on.
    std::vector<geometrize::Bitmap> bitmaps;
    bitmaps.push_back(backgroundBitmap);
    geometrize::Bitmap baseBitmap(backgroundBitmap);
    for(const geometrize::ShapeResult& shapeResult : data) {
        geometrize::core::drawLines(baseBitmap, shapeResult.color, shapeResult.shape->rasterize());
        bitmaps.push_back(geometrize::Bitmap(baseBitmap));
    }

    // TODO implement shape/frame skip and other options to limit size
    for(std::uint32_t i = 0; i < bitmaps.size(); i++) {
        const std::int32_t delayMs = std::max(20, static_cast<std::int32_t>(1000 / (i + 1)));
        gif.addFrame(image::createImage(bitmaps[i]), QPoint(0, 0), delayMs);
    }

    return gif.save(QString::fromStdString(filePath));
}

}

}
