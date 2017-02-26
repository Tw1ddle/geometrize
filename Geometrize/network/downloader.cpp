#include "downloader.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

namespace geometrize
{

namespace network
{

class Downloader::DownloaderImpl
{
public:
    DownloaderImpl(Downloader* pQ, const QUrl& url, const std::function<void(Downloader*, QNetworkReply::NetworkError)>& onDownloaded) : q{pQ}, m_url{url}
    {
        q->connect(&m_webCtrl, &QNetworkAccessManager::finished, [this](QNetworkReply* pReply) {
            m_downloadedData = pReply->readAll();
            const QNetworkReply::NetworkError error{pReply->error()};
            pReply->deleteLater();

            emit q->signal_downloaded(q, error); // Note caller may delete the Downloader instance at this point
        });
        q->connect(q, &Downloader::signal_downloaded, onDownloaded);

        const QNetworkRequest request(m_url);
        m_webCtrl.get(request);
    }
    DownloaderImpl& operator=(const DownloaderImpl&) = delete;
    DownloaderImpl(const DownloaderImpl&) = delete;
    ~DownloaderImpl() = default;

    QByteArray getDownloadedData() const
    {
        return m_downloadedData;
    }

    QUrl getUrl() const
    {
        return m_url;
    }

private:
    Downloader* q;
    QNetworkAccessManager m_webCtrl;
    QByteArray m_downloadedData;
    QUrl m_url;
};

Downloader::Downloader(const QUrl& url, const std::function<void(Downloader*, QNetworkReply::NetworkError)>& onDownloaded) : QObject(nullptr), d{std::make_unique<Downloader::DownloaderImpl>(this, url, onDownloaded)}
{
}

QByteArray Downloader::getDownloadedData() const
{
    return d->getDownloadedData();
}

QUrl Downloader::getUrl() const
{
    return d->getUrl();
}

}

}
