#pragma once

#include <QNetworkReply>

namespace geometrize
{

namespace network
{
class Downloader;
}

}

namespace geometrize
{

namespace network
{

namespace completionhandlers
{

/**
 * @brief onImageDownloadComplete A callback when an image download completes.
 * @param self The downloader that downloaded the image. Note this callback is responsible for deleting the downloader.
 * @param error The download error, if any.
 */
void onImageDownloadComplete(network::Downloader* self, QNetworkReply::NetworkError error);

/**
 * @brief onWebpageDownloadComplete A callback when a webpage download completes.
 * @param self The downloader that downloaded the webpage. Note this callback is responsible for deleting the downloader.
 * @param error The download error, if any.
 */
void onWebpageDownloadComplete(network::Downloader* self, QNetworkReply::NetworkError error);

}

}

}
