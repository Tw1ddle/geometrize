#include "imageexporter.h"

#include <assert.h>
#include <cstdint>

#include <QFileInfo>
#include <QImage>

#include "geometrize/bitmap/bitmap.h"

#include "image/imageloader.h"

namespace geometrize
{

namespace exporter
{

bool exportImage(const geometrize::Bitmap& bitmapData, const std::string& filePath)
{
    const QImage image{geometrize::image::createImage(bitmapData)};
    return image.save(QString::fromStdString(filePath));
}

}

}
