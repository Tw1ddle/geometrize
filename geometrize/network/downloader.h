#pragma once

#include <functional>
#include <memory>

#include <QByteArray>
#include <QNetworkReply>
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
    explicit Downloader(const QUrl& url, const std::function<void(Downloader*, QNetworkReply::NetworkError)>& onDownloaded);
    virtual ~Downloader();
    Downloader& operator=(const Downloader&) = delete;
    Downloader(const Downloader&) = delete;

    /**
     * @brief getDownloadedData Gets the data the Downloader has downloaded.
     * @return The downloaded data.
     */
    QByteArray getDownloadedData() const;

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

private:
    class DownloaderImpl;
    std::unique_ptr<DownloaderImpl> d;
};

}

}
