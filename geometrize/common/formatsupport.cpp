#include "formatsupport.h"

#include <QImageReader>

namespace geometrize
{

namespace format
{

std::vector<std::string> getReadableImageFileExtensions(const bool includeDotPrefix)
{
    std::vector<std::string> extensions;

    const QList<QByteArray> supportedImageFormats{QImageReader::supportedImageFormats()};
    for(int i = 0; i < supportedImageFormats.count(); i++) {
        QString format;
        if(includeDotPrefix) {
            format += ".";
        }
        format += QString(supportedImageFormats.at(i)).toLower();
        extensions.push_back(format.toStdString());
    }

    return extensions;
}

std::vector<std::string> getSupportedTemplateManifestFileExtensions()
{
    const static std::vector<std::string> extensions{"json", "xml"}; // TODO case sensitivity?

    return extensions;
}

QList<QUrl> getUrls(const QMimeData* const data)
{
    return data->urls();
}

}

}
