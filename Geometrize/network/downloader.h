#pragma once

#include <functional>

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

namespace geometrize
{

namespace network
{

/**
 * @brief The Downloader class is a helper class for downloading files over the network.
 */
class Downloader : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Downloader Creates a new Downloader.
     * @param url The URL to download.
     * @param onDownloaded The callback triggered when the download completes.
     */
    explicit Downloader(QUrl url, const std::function<void(Downloader*, QNetworkReply::NetworkError)>& onDownloaded);
    virtual ~Downloader() = default;
    Downloader& operator=(const Downloader&) = delete;
    Downloader(const Downloader&) = delete;

    /**
     * @brief downloadedData Gets the data the Downloader has downloaded.
     * @return The downloaded data.
     */
    QByteArray downloadedData() const;

    /**
     * @brief getUrl Gets the URL the Downloader was made to download.
     * @return The downloaded data.
     */
    QUrl getUrl() const;

signals:
    /**
     * @brief signal_downloaded Dispatched after a download completes.
     * @param downloader A reference to the downloader object. Remember to delete the downloader if necessary.
     * @param error The network error (if any).
     */
    void signal_downloaded(Downloader* downloader, QNetworkReply::NetworkError error);

private slots:
    void downloadFinished(QNetworkReply* pReply);

private:
    QNetworkAccessManager m_webCtrl;
    QByteArray m_downloadedData;
    QUrl m_url;
};

}

}
