#include "gifexporter.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <thread>

#include <QImage>
#include <QRgb>
#include <QSet>
#include <QVector>

#include "BurstLinker.h"

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/core.h"
#include "geometrize/rasterizer/rasterizer.h"
#include "geometrize/rasterizer/scanline.h"
#include "geometrize/shape/shape.h"
#include "geometrize/shaperesult.h"

#include "image/imageloader.h"
#include "imageexporter.h"

namespace {

std::uint32_t* makeImageData(const QImage& image)
{
    // This is deleted by the library after the image is added
    const std::size_t imageSizePixels = image.width() * image.height();
    auto bgrs = new std::uint32_t[imageSizePixels];
    const uchar* imageBytes = image.constBits();

    int byteIndex = 0;
    for(std::size_t pixelIdx = 0; pixelIdx < imageSizePixels; pixelIdx++) {
        const std::int32_t r = imageBytes[byteIndex];
        const std::int32_t g = imageBytes[byteIndex + 1];
        const std::int32_t b = imageBytes[byteIndex + 2];

        const std::int32_t bgr = b << 16 | g << 8 | r;
        bgrs[pixelIdx] = static_cast<std::uint32_t>(bgr);

        byteIndex += 4;
    }

    return bgrs;
}

bool addFrame(
    const std::vector<geometrize::ShapeResult>& shapes,
    const std::uint32_t inputWidth,
    const std::uint32_t inputHeight,
    const std::uint32_t outputWidth,
    const std::uint32_t outputHeight,
    const std::uint32_t delayMs,
    blk::BurstLinker& gif)
{
    QImage image = geometrize::exporter::renderSvgShapeDataToImage(shapes, inputWidth, inputHeight, outputWidth, outputHeight);
    image = image.convertToFormat(QImage::Format_RGBA8888);

    if(image.isNull() || image.width() == 0 || image.height() == 0) {
        assert(0 && "Attempted to add invalid image");
        return false;
    }

    gif.connect(makeImageData(image), delayMs, blk::QuantizerType::Octree, blk::DitherType::NO, 0, 0);

    return true;
}

}

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
        const std::function<bool(std::size_t)>& frameSkipPredicate,
        const std::string& filePath)
{
    blk::BurstLinker gif;
    if(!gif.init(filePath.c_str(), outputWidth, outputHeight, 0, std::max(1U, std::thread::hardware_concurrency()))) {
        assert(0 && "Failed to initialize BurstLinker GIF export");
        return false;
    }

    // Could start by adding the final result
    //addFrame(data, inputWidth, inputHeight, outputWidth, outputHeight, 400, gif);

    for(std::uint32_t i = 0; i < data.size(); i++) {
        if(frameSkipPredicate(i)) {
            continue;
        }

        std::uint32_t delayMs = i == 0 ? 0 : std::max(20, static_cast<std::int32_t>(1000 / (i + 1)));
        if(i == data.size() - 1U) {
            delayMs = 2000; // Extra delay at end of animation
        }

        const std::vector<geometrize::ShapeResult> shapes(data.begin(), data.begin() + i);
        addFrame(shapes, inputWidth, inputHeight, outputWidth, outputHeight, delayMs, gif);
    }

    gif.release();
    return true;
}

}

}
