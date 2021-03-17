#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QtNetworkAuth>
#include <QJsonDocument>

class Spotify : public QObject
{
    Q_OBJECT
public:
    Spotify(QObject *parent = nullptr, QString clientId = "",QString clientSecret = "");
    void grant();
    void searchTrack(QString searchTerm);
private:
    QString clientId;
    QString clientSecret;
    QOAuth2AuthorizationCodeFlow oAuth2Spotify;
    void onSearchFinished(QNetworkReply *reply);
private slots:
    void onAccessGranted();
    void onStatusChanged(QAbstractOAuth::Status status);
signals:
    void searchFinished(QJsonDocument);
};

#endif // SPOTIFY_H
