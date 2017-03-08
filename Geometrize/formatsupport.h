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
 * @brief getReadableImageFileExtensions Gets the lower-case image file extensions that have read support.
 * @param includeDotPrefix Whether to include the dot prefix on file extensions e.g. "jpg" or ".jpg".
 * @return The list of supported image file extensions.
 */
std::vector<std::string> getReadableImageFileExtensions(bool includeDotPrefix);

/**
 * @brief getSupportedTemplateManifestFileExtensions Gets the supported template manifest file extensions for serialization and de-serialization.
 * @return The list of supported template manifest file extensions.
 */
std::vector<std::string> getSupportedTemplateManifestFileExtensions();

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
