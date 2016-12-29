#include "networkactions.h"

#include <QUrl>

#include "network/downloader.h"

namespace geometrize
{

namespace network
{

void downloadImage(const QUrl& url, const std::function<void(network::Downloader* self, QNetworkReply::NetworkError error)>& onComplete)
{
    new network::Downloader(url, onComplete);
}

void downloadWebpage(const QUrl& url, const std::function<void(network::Downloader* self, QNetworkReply::NetworkError error)>& onComplete)
{
    new network::Downloader(url, onComplete);
}

}

}
