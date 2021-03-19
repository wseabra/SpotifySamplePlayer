#include "imagedownloaderhelper.h"

ImageDownloaderHelper::ImageDownloaderHelper(QObject *parent) : QObject(parent)
{
    downloader = new QNetworkAccessManager(this);
}

ImageDownloaderHelper::~ImageDownloaderHelper()
{
    delete downloader;
}

void ImageDownloaderHelper::downloadImage(QString url)
{
   QUrl imageUrl(url);

   QNetworkReply *reply = downloader->get(QNetworkRequest(imageUrl));

    connect(reply,&QNetworkReply::finished,this,[=]() {
        onDownloadFinished(reply);
    });

}

void ImageDownloaderHelper::onDownloadFinished(QNetworkReply *reply)
{
    QImageReader imageReader(reply);
    QImage pic = imageReader.read();
    emit downloadFinished(pic);
}
