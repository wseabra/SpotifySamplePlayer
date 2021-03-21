#include "spotify.h"

#include <QDesktopServices>

Spotify::Spotify(QObject *parent, QString clientId, QString clientSecret):
    QObject(parent),
    authenticated(false),
    clientId(clientId),
    clientSecret(clientSecret)
{
    //Configuring the authentication object
    auto replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
    replyHandler->setCallbackPath("callback");
    oAuth2Spotify.setReplyHandler(replyHandler);
    oAuth2Spotify.setAuthorizationUrl(QUrl(SPOTIFY_AUTHORIZATION_URL));
    oAuth2Spotify.setAccessTokenUrl(QUrl(SPOTIFY_TOKEN_URL));
    oAuth2Spotify.setClientIdentifierSharedKey(clientSecret);
    oAuth2Spotify.setClientIdentifier(clientId);
    oAuth2Spotify.setScope(SPOTIFY_SCOPE);

    //COnnecting signals of the authorization object
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
    QUrl searchUrl(SEARCH_URL+searchTerm+SEARCH_URL_PARAMETER);
    QNetworkReply *reply = oAuth2Spotify.get(searchUrl);
    qDebug() << "Searching tracks. Term: "+searchTerm+" url: "+searchUrl.toString();

    //once the reply arrives call the onSeachFinished method
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
   //turn the response into json and call method to proccess it
   QByteArray data = reply->readAll();
   QJsonDocument doc = QJsonDocument::fromJson(data);
   auto resultVector = processSearchResult(doc);
   emit searchFinished(resultVector);
   reply->deleteLater();
}

QVector<Track> Spotify::processSearchResult(const QJsonDocument &result)
{
    qDebug() << "Processing search result";

    QJsonObject root = result.object();
    QVector<Track> resultList;
    //transforming json result into vector of tracks
    if (root.contains(TRACKS_FIELD)) {
        auto tracks = root.value(TRACKS_FIELD);
        if (tracks.isObject() && tracks.toObject().contains(ITEMS_FIELD)){
            auto tracksArray = tracks.toObject().value(ITEMS_FIELD);
            if (tracksArray.isArray()) {
                //loop tracks array, for each track create object and add to the result
                for (int index = 0; index < tracksArray.toArray().size(); index++) {
                    auto trackItem = tracksArray.toArray().at(index);
                    QJsonObject track = trackItem.toObject();

                    QString trackId = track.value(ID_FIELD).toString();
                    QString name = track.value(NAME_FIELD).toString();
                    QString album = track.value(ALBUM_FIELD).toObject().value(NAME_FIELD).toString();
                    QString artist = track.value(ARTISTS_FIELD).toArray().at(0).toObject().value(NAME_FIELD).toString();
                    QString image = track.value(ALBUM_FIELD).toObject()
                            .value(IMAGES_FIELD).toArray().at(2).toObject().value(URL_FIELD).toString();
                    QString previewUrl = track.value(PREVIEW_URL_FIELD).toString();

                    Track trackObj(trackId,name,album,artist,image,previewUrl);

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


