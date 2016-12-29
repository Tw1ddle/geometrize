#pragma once

#include <string>
#include <vector>

#include <QMimeData>
#include <QString>
#include <QStringList>
#include <QUrl>

namespace geometrize
{

namespace format
{

/**
 * @brief getSupportedImageFileExtensions Gets a list of supported image file extensions for loading.
 * @param includeDotPrefix Whether to include the dot prefix on file extensions e.g. "jpg" or ".jpg".
 * @return The list of supported image file extensions.
 */
std::vector<std::string> getSupportedImageFileExtensions(const bool includeDotPrefix);

/**
 * @brief getUrls Gets a list of URLs from a Qt MimeData item.
 * @param data The Mime data.
 * @return List of URLs.
 */
QList<QUrl> getUrls(const QMimeData* const data);

/**
 * @brief getLocalFiles Gets a list of local file paths from a Qt MimeData item.
 * @param data The Mime data.
 * @return List of local file paths.
 */
QStringList getLocalFiles(const QMimeData* const data);

/**
 * @brief getRemoteUrls Gets a list of remote URLs from a Qt MimeData item.
 * @param data The Mime data.
 * @return  List of remote URLs.
 */
QList<QUrl> getRemoteUrls(const QMimeData* const data);

}

}
