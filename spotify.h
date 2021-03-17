#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QtNetworkAuth>
#include <QJsonDocument>
#include <QVector>

#include "track.h"

class Spotify : public QObject
{
    Q_OBJECT
public:
    Spotify(QObject *parent = nullptr, QString clientId = "",QString clientSecret = "");
    void grant();
    void searchTrack(QString searchTerm);
    bool authenticated;
private:
    QString clientId;
    QString clientSecret;
    QOAuth2AuthorizationCodeFlow oAuth2Spotify;
    void onSearchFinished(QNetworkReply *reply);
    QVector<Track> processSearchResult(QJsonDocument result);
    bool checkTrackData(Track track);
private slots:
    void onAccessGranted();
    void onStatusChanged(QAbstractOAuth::Status status);
signals:
    void searchFinished(QVector<Track>);
    void accessGranted(bool);
    void accesStatusChanged(bool);
};

#endif // SPOTIFY_H
