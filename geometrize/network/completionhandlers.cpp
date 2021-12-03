#include "completionhandlers.h"

#include <optional>

#include <QDebug>
#include <QNetworkReply>
#include <QPixmap>
#include <QRegularExpression>
#include <QString>

#include "network/downloader.h"
#include "network/networkactions.h"
#include "task/imagetask.h"
#include "task/imagetaskcreator.h"
#include "task/taskutil.h"

namespace geometrize
{

namespace network
{

namespace completionhandlers
{

void onImageDownloadComplete(network::Downloader* self, const QNetworkReply::NetworkError error)
{
    const QByteArray data{self->getDownloadedData()};
    const QUrl url{self->getUrl()};
    self->deleteLater();

    if(error != QNetworkReply::NoError) {
        // TODO post error to console or somewhere?
        qDebug() << "FINISHED DOWNLOADING WEBPAGE WITH ERROR" << error;
        return;
    }

    QImage image{QImage::fromData(data)};
    if(image.isNull()) {
        qDebug() << "FAILED TO CREATE IMAGE FROM DOWNLOADED DATA BUFFER";
        return;
    }

    image = image.convertToFormat(QImage::Format_RGBA8888); // Note: to guarantee format is RGBA8888
    if(image.format() != QImage::Format_RGBA8888) {
        qDebug() << "FAILED TO CONVERT IMAGE TO RGBA8888";
        return;
    }

    task::createImageTaskAndWindow(url.toString().toStdString(), image, std::nullopt);
}

void onWebpageDownloadComplete(network::Downloader* self, const QNetworkReply::NetworkError error)
{
    const QByteArray data{self->getDownloadedData()};
    const QUrl url{self->getUrl()};
    self->deleteLater();

    if(error != QNetworkReply::NoError) {
        // TODO post error to console or somewhere?
        qDebug() << "FINISHED DOWNLOADING WEBPAGE WITH ERROR" << error; // TODO error checks
        return;
    }

    const QString document(data);

    QRegularExpression imageTagRegex("\\<img[^\\>]*src\\s*=\\s*\"([^\"]*)\"[^\\>]*\\>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::InvertedGreedinessOption);

    QStringList imageMatches;
    QStringList urlMatches;

    QRegularExpressionMatchIterator it = imageTagRegex.globalMatch(document);
    while(it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        imageMatches.append(match.captured(0)); // Should hold complete img tag
        urlMatches.append(match.captured(1)); // Should hold only src property
    }

    QList<QUrl> imageUrls;
    for(const QString& url : urlMatches) {
        imageUrls.push_back(QUrl(url));
    }

    const QString currentPathUrl{url.adjusted(QUrl::RemoveFilename | QUrl::StripTrailingSlash | QUrl::RemoveFragment).toString()};
    for(QUrl& imageUrl : imageUrls) {

        // Special cases for relative URLs
        if(imageUrl.scheme().isEmpty()) {
            const QString imageUrlString{imageUrl.toString(QUrl::PrettyDecoded)};
            if(imageUrlString.startsWith("//")) {
                // Some relative links are like //thedomain.com/example.png - so are relative and just missing the scheme
                if(!url.scheme().isEmpty()) {
                    imageUrl.setScheme(url.scheme()); // Take the scheme from the source page
                } else {
                    imageUrl.setScheme("http"); // Assume HTTP if it's '//' relative and have no clue about the scheme
                }
            } else if(imageUrlString.startsWith("/")) {
                imageUrl = imageUrl.resolved(currentPathUrl); // A URL relative to the current path
            } else {
                imageUrl = QUrl(currentPathUrl + imageUrl.toString()); // Should be relative path based on the document root, so append the base URL
            }
        }

        network::downloadImage(imageUrl, onImageDownloadComplete);
    }
}

}

}

}
