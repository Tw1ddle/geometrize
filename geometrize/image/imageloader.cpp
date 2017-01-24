#include "image/imageloader.h"

#include <assert.h>

#include <QImage>
#include <QPixmap>
#include <QString>

#include "geometrize/core.h"

namespace geometrize
{

namespace image
{

BitmapData* createBitmapData(const QImage& image)
{
    if(image.isNull()) {
        assert(0 && "Image is null, will fail to create bitmap data");
        return nullptr;
    }

    if(image.width() == 0 || image.height() == 0) {
        assert(0 && "Image has zero width or height");
        return nullptr;
    }

    if(image.format() != QImage::Format_RGBA8888) {
        assert(0 && "Cannot create bitmap data from a non-RGBA8888 image");
        return nullptr;
    }

    const std::vector<uchar> data(image.bits(), image.bits() + image.byteCount());
    return new BitmapData(image.width(), image.height(), data);
}

QPixmap createPixmap(const BitmapData& data)
{
    if(data.getWidth() == 0 || data.getHeight() == 0) {
        assert(0 && "Bad bitmap data");
        return QPixmap();
    }

    QImage image(data.getDataRef().data(), data.getWidth(), data.getHeight(), QImage::Format_RGBA8888);
    return QPixmap::fromImage(image);
}

QImage loadImage(const QString& filePath)
{
    const QImage image(filePath);

    if(image.isNull()) {
        assert(0 && "Bad image data");
        return image;
    }

    return image.convertToFormat(QImage::Format_RGBA8888);
}

QPixmap loadPixmap(const QString& filePath)
{
    return QPixmap::fromImage(loadImage(filePath));
}

}

}
