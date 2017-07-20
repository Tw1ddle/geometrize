#include "imageexporter.h"

#include <cassert>

#include <QImage>

#include "geometrize/bitmap/bitmap.h"

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

}

}
