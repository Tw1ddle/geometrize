#include "completionhandlers.h"

#include <QDebug>
#include <QNetworkReply>
#include <QPixmap>
#include <QRegExp>
#include <QString>

#include "job/imagejob.h"
#include "job/jobutil.h"
#include "network/downloader.h"
#include "network/networkactions.h"

namespace geometrize
{

namespace network
{

namespace completionhandlers
{

void onImageDownloadComplete(network::Downloader* self, QNetworkReply::NetworkError error)
{
    qDebug() << "FINISHED DOWNLOADING IMAGE WITH ERROR" << error; // TODO error checks

    QImage image;
    image.loadFromData(self->getDownloadedData());
    image = image.convertToFormat(QImage::Format_RGBA8888); // Note doing this to guarantee format is RGBA8888

    util::openImageJobFromWeb(image, self->getUrl().toString(), true);

    delete self;
}

void onWebpageDownloadComplete(network::Downloader* self, QNetworkReply::NetworkError error)
{
    qDebug() << "FINISHED DOWNLOADING WEBPAGE WITH ERROR" << error; // TODO error checks

    const QString document(self->getDownloadedData());

    QRegExp imageTagRegex("\\<img[^\\>]*src\\s*=\\s*\"([^\"]*)\"[^\\>]*\\>", Qt::CaseInsensitive);
    imageTagRegex.setMinimal(true);

    QStringList imageMatches;
    QStringList urlMatches;

    int offset{0};
    while((offset = imageTagRegex.indexIn(document, offset)) != -1) {
        offset += imageTagRegex.matchedLength();
        imageMatches.append(imageTagRegex.cap(0)); // Should hold complete img tag
        urlMatches.append(imageTagRegex.cap(1)); // Should hold only src property
    }

    qDebug() << imageMatches;
    qDebug() << urlMatches;

    QList<QUrl> imageUrls;
    for(const QString& url : urlMatches) {
        imageUrls.push_back(QUrl(url));
    }

    const QString baseUrl{self->getUrl().adjusted(QUrl::RemoveFilename | QUrl::StripTrailingSlash).toString()};
    for(QUrl& url : imageUrls) {
        if(url.isRelative()) {
            url = QUrl(baseUrl + url.toString());
        }
        network::downloadImage(url, onImageDownloadComplete);
    }

    delete self;
}

}

}

}
