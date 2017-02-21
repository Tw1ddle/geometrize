#include "gifexporter.h"

#include <cstdint>

#include <QImage>

#include "qgifimage.h"

namespace geometrize
{

namespace exporter
{

bool exportGIF(const std::vector<QImage>& images, const std::string& filePath)
{
    QGifImage gif;

    for(const QImage& image : images) {
        gif.addFrame(image);
    }

    return gif.save(QString::fromStdString(filePath));
}

}

}
