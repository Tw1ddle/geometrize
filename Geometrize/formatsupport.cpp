#include "formatsupport.h"

namespace geometrize
{

namespace format
{

/**
 * @brief getSupportedImageFileExtensions Gets a list of supported image file extensions for loading.
 * @param includeDotPrefix Whether to include the dot prefix on file extensions e.g. "jpg" or ".jpg".
 * @return The vector of supported image file extensions.
 */
std::vector<std::string> getSupportedImageFileExtensions(const bool includeDotPrefix)
{
    const static std::vector<std::string> extensions{"jpg", "png"}; // TODO case sensitivity?
    // TODO

    return extensions;
}

/**
 * @brief getUrls Gets a list of URLs from a Qt MimeData item.
 * @param data The Mime data.
 * @return List of URLs.
 */
QList<QUrl> getUrls(const QMimeData* const data)
{
    return data->urls();
}

/**
 * @brief getLocalFiles Gets a list of local file paths from a Qt MimeData item.
 * @param data The Mime data.
 * @return List of local file paths.
 */
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

/**
 * @brief getRemoteUrls Gets a list of remote URLs from a Qt MimeData item.
 * @param data The Mime data.
 * @return  List of remote URLs.
 */
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
