#ifndef IMAGEDOWNLOADERHELPER_H
#define IMAGEDOWNLOADERHELPER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImage>
#include <QImageReader>

//This class is used for download images, specifically the track cover
class ImageDownloaderHelper : public QObject
{
    Q_OBJECT
public:
    //The constructor allocates the QNetworkAccessManager object
    explicit ImageDownloaderHelper(QObject *parent = nullptr);

    //The destructor deletes the QNetworkAccessManager object
    ~ImageDownloaderHelper();

    //This function Downloads the track cover
    //from the url utilising the QNetworkAccessManager
    void downloadImage(QString url);

private:
    //This object is used to get the image using http GET
    QNetworkAccessManager *downloader;

    //This function is executed after the image download is finished,
    //once finished emits the downloadFinished signal with the resulting QImage
    void onDownloadFinished(QNetworkReply *reply);

signals:
    //This signal is emitted once the image download is finished
    //with the resulted QImage
    void downloadFinished(QImage);

};

#endif // IMAGEDOWNLOADERHELPER_H
