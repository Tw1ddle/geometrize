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

QStringList getLocalFiles(const QMimeData* const data)
{
    QStringList files;

    if(!data->hasUrls()) {
        return files;
    }

    for(const QUrl& url : data->urls()) {
        if(url.isLocalFile()) {
            files.push_back(url.toLocalFile());
        }
    }

    return files;
}

QList<QUrl> getRemoteUrls(const QMimeData* const data)
{
    QList<QUrl> urls;

    if(!data->hasUrls()) {
        return urls;
    }

    for(const QUrl& url : data->urls()) {
        if(!url.isLocalFile()) {
            urls.push_back(url);
        }
    }

    return urls;
}

}

}
