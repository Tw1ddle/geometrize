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

}

}
