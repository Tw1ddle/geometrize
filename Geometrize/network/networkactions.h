#pragma once

#include <functional>

#include <QNetworkReply>
#include <QUrl>

#include "network/downloader.h"

namespace geometrize
{

namespace network
{

/**
 * @brief downloadImage Downloads an image over the network.
 * @param url The URL of the image.
 * @param onComplete A callback invoked when the download completes, or fails. Note that this callback is usually responsible for deleting the downloader.
 */
void downloadImage(const QUrl& url, const std::function<void(network::Downloader* self, QNetworkReply::NetworkError error)>& onComplete);

/**
 * @brief downloadWebpage Downloads a whole webpage over the network.
 * @param url The URL of the webpage.
 * @param onComplete A callback invoked when the webpage download completes, or fails. Note that this callback is usually responsible for deleting the downloader.
 */
void downloadWebpage(const QUrl& url, const std::function<void(network::Downloader* self, QNetworkReply::NetworkError error)>& onComplete);

}

}
