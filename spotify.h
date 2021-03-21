#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QtNetworkAuth>
#include <QJsonDocument>
#include <QVector>

#include "constants.h"
#include "track.h"

//This class handles interactions with the Spotify Web API
class Spotify : public QObject
{
    Q_OBJECT
public:
    //The contructor receives the clientId and the secretId for the application
    //and configures the authetication parameters
    Spotify(QObject *parent = nullptr, QString clientId = "",QString clientSecret = "");

    //This function asks for the authetication from the spotify API
    void grant();

    //This function post a search using the search term given by the user
    void searchTrack(QString searchTerm);

    //Authentication status
    bool authenticated;
private:
    //The application Spotify claientId
    QString clientId;

    //The application Spotify claient secret
    QString clientSecret;

    //This object handles the authentication and requests to the API
    QOAuth2AuthorizationCodeFlow oAuth2Spotify;

    //This function is executed once the search is finished and emits the
    //searchFinished signal
    void onSearchFinished(QNetworkReply *reply);

    //This function process the search result and returns it in the vector of tracks format
    QVector<Track> processSearchResult(const QJsonDocument &result);

    //This function checks if the track has all the data
    //some track don't have the preview_url field and should be filtered
    bool checkTrackData(Track track);
private slots:
    //This funtion is executed once the acces is granted by the spotify API
    void onAccessGranted();

    //This function is executed once the authentication status changes
    void onStatusChanged(QAbstractOAuth::Status status);
signals:
    //This signal is emited once the search finishes,
    //sends the search result as vector of tracks
    void searchFinished(QVector<Track>);

    //This signal is emited once the acces is granted by the spotify API,
    //sends the authetication status
    void accessGranted(bool);

    //This signal is emited if the authentication status changes
    void accesStatusChanged(bool);
};

#endif // SPOTIFY_H
