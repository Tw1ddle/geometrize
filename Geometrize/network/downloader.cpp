#include "downloader.h"

namespace geometrize
{

namespace network
{
 
Downloader::Downloader(QUrl url, const std::function<void(Downloader*, QNetworkReply::NetworkError)>& onDownloaded) : QObject(nullptr), m_url{url}
{
    connect(&m_webCtrl, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    connect(this, &Downloader::signal_downloaded, onDownloaded);

    QNetworkRequest request(url);
    m_webCtrl.get(request);
}

void Downloader::downloadFinished(QNetworkReply* pReply)
{
    m_downloadedData = pReply->readAll();
    const QNetworkReply::NetworkError error{pReply->error()};
    pReply->deleteLater();

    // Note caller may delete the Downloader instance here
    emit signal_downloaded(this, error);
}

QByteArray Downloader::downloadedData() const
{
    return m_downloadedData;
}

QUrl Downloader::getUrl() const
{
    return m_url;
}

}

}
