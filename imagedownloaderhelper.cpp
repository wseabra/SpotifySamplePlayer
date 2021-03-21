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

   //once the download finishes call the function onDownloadFinished passing the
   //reply as parameter
    connect(reply,&QNetworkReply::finished,this,[=]() {
        onDownloadFinished(reply);
    });

}

void ImageDownloaderHelper::onDownloadFinished(QNetworkReply *reply)
{
    //Read the image passed by parameter and emits downloadFinished signal
    QImageReader imageReader(reply);
    QImage pic = imageReader.read();
    emit downloadFinished(pic);
}
