#include "spotify.h"

#include <QDesktopServices>

Spotify::Spotify(QObject *parent, QString clientId, QString clientSecret):
    QObject(parent),
    authenticated(false),
    clientId(clientId),
    clientSecret(clientSecret)
{
    auto replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
    replyHandler->setCallbackPath("callback");
    oAuth2Spotify.setReplyHandler(replyHandler);
    oAuth2Spotify.setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
    oAuth2Spotify.setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));
    oAuth2Spotify.setClientIdentifierSharedKey(clientSecret);
    oAuth2Spotify.setClientIdentifier(clientId);
    oAuth2Spotify.setScope("user-read-private user-top-read playlist-read-private playlist-modify-public playlist-modify-private");


    connect(&oAuth2Spotify, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
                 &QDesktopServices::openUrl);
    connect(&oAuth2Spotify, &QOAuth2AuthorizationCodeFlow::statusChanged, this,
            &Spotify::onStatusChanged);
    connect(&oAuth2Spotify, &QOAuth2AuthorizationCodeFlow::granted, this,
            &Spotify::onAccessGranted);

}

void Spotify::grant(){
    oAuth2Spotify.grant();
}

void Spotify::searchTrack(QString searchTerm)
{
    QUrl searchUrl("https://api.spotify.com/v1/search?q="+searchTerm+"&type=track&market=from_token&limit=50");
    QNetworkReply *reply = oAuth2Spotify.get(searchUrl);
    qDebug() << "Searching tracks. Term: "+searchTerm+" url: "+searchUrl.toString();

    connect(reply,&QNetworkReply::finished,this,[=]() {
        onSearchFinished(reply);
    });
}

void Spotify::onAccessGranted() {
    authenticated = true;
    qDebug() << "Access to the account granted!";
    emit accessGranted(authenticated);
}

void Spotify::onStatusChanged(QAbstractOAuth::Status status){
    if ( (status == QAbstractOAuth::Status::Granted) || (status == QAbstractOAuth::Status::TemporaryCredentialsReceived) ){
        authenticated = true;
        qDebug() << "Status changed to granted!";
    }
    else if (status == QAbstractOAuth::Status::NotAuthenticated){
        authenticated = false;
        qDebug() << "Status changed to not granted!";
    }
    emit accesStatusChanged(authenticated);
}

void Spotify::onSearchFinished(QNetworkReply *reply)
{
   qDebug() << "Search finished";
   if(reply->error() != QNetworkReply::NoError){
       qDebug() << "error:" << reply->errorString();
       return;
   }
   QByteArray data = reply->readAll();
   QJsonDocument doc = QJsonDocument::fromJson(data);
   auto resultVector = processSearchResult(doc);
   emit searchFinished(resultVector);
}

QVector<Track> Spotify::processSearchResult(const QJsonDocument &result)
{
    qDebug() << "Processing search result";

    QJsonObject root = result.object();
    QVector<Track> resultList;
    if (root.contains("tracks")) {
        auto tracks = root.value("tracks");
        if (tracks.isObject() && tracks.toObject().contains("items")){
            auto tracksArray = tracks.toObject().value("items");
            if (tracksArray.isArray()) {
                for (int index = 0; index < tracksArray.toArray().size(); index++) {
                    Track trackObj;
                    auto trackItem = tracksArray.toArray().at(index);
                    QJsonObject track = trackItem.toObject();

                    trackObj.trackId = track.value("id").toString();
                    trackObj.name = track.value("name").toString();
                    trackObj.album = track.value("album").toObject().value("name").toString();
                    trackObj.artist = track.value("artists").toArray().at(0).toObject().value("name").toString();
                    trackObj.image = track.value("album").toObject().value("images").toArray().at(2).toObject().value("url").toString();
                    trackObj.previewUrl = track.value("preview_url").toString();

                    if(checkTrackData(trackObj)){
                        resultList.push_back(trackObj);
                    }

                }
            }
        }
    }
    return resultList;
}

bool Spotify::checkTrackData(Track track)
{
   if (track.trackId.isEmpty() || track.album.isEmpty() || track.artist.isEmpty()
           || track.image.isEmpty() || track.name.isEmpty() || track.previewUrl.isEmpty()){
       return false;
   }
   return true;
}


