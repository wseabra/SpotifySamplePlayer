#ifndef IMAGEDOWNLOADERHELPER_H
#define IMAGEDOWNLOADERHELPER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImage>
#include <QImageReader>

class ImageDownloaderHelper : public QObject
{
    Q_OBJECT
public:
    explicit ImageDownloaderHelper(QObject *parent = nullptr);
    ~ImageDownloaderHelper();
    void downloadImage(QString url);

private:
    QNetworkAccessManager *downloader;
    void onDownloadFinished(QNetworkReply *reply);

signals:
    void downloadFinished(QImage);

};

#endif // IMAGEDOWNLOADERHELPER_H
