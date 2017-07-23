#include "image/imageloader.h"

#include <cassert>

#include <QImage>
#include <QPixmap>
#include <QString>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/core.h"

namespace geometrize
{

namespace image
{

Bitmap createBitmap(const QImage& image)
{
    assert(!image.isNull() && "Image is null, will fail to create bitmap data");
    assert((image.width() != 0 && image.height() != 0) && "Image has zero width or height");
    assert((image.format() == QImage::Format_RGBA8888) && "Cannot create bitmap data from a non-RGBA8888 image");

    const std::vector<uchar> data(image.bits(), image.bits() + image.byteCount());
    return Bitmap(image.width(), image.height(), data);
}

QImage createImage(const Bitmap& data)
{
    if(data.getWidth() == 0 || data.getHeight() == 0) {
        assert(0 && "Bad bitmap data");
        return QImage();
    }

    // Note! This takes a shallow copy of the data, and so depends on the bitmap itself continuing to live on
    return QImage(data.getDataRef().data(), data.getWidth(), data.getHeight(), QImage::Format_RGBA8888);
}

QPixmap createPixmap(const Bitmap& data)
{
    return QPixmap::fromImage(createImage(data));
}

QImage loadImage(const std::string& filePath)
{
    const QImage image(QString::fromStdString(filePath));

    if(image.isNull()) {
        assert(0 && "Bad image data");
        return image;
    }

    return image.convertToFormat(QImage::Format_RGBA8888);
}

QPixmap loadPixmap(const std::string& filePath)
{
    return QPixmap::fromImage(loadImage(filePath));
}

}

}
