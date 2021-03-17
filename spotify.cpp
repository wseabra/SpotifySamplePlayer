#include "spotify.h"

#include <QDesktopServices>

Spotify::Spotify(QObject *parent, QString clientId, QString clientSecret):
    QObject(parent),
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
    QUrl searchUrl("https://api.spotify.com/v1/search?q="+searchTerm+"&type=track&market=from_token&limit=1");
    QNetworkReply *reply = oAuth2Spotify.get(searchUrl);

    connect(reply,&QNetworkReply::finished,this,[=]() {
        onSearchFinished(reply);
    });
}

void Spotify::onAccessGranted() {
   qDebug() << "Entrou!!!";
   searchTrack("Anitta");
}

void Spotify::onStatusChanged(QAbstractOAuth::Status status){
    if ( (status == QAbstractOAuth::Status::Granted) || (status == QAbstractOAuth::Status::TemporaryCredentialsReceived) )
        qDebug() << "Status granted!";
    else if (status == QAbstractOAuth::Status::NotAuthenticated)
        qDebug() << "Status not granted!";
}

void Spotify::onSearchFinished(QNetworkReply *reply)
{
   if(reply->error() != QNetworkReply::NoError){
       qDebug() << "error:" << reply->errorString();
       return;
   }
   QByteArray data = reply->readAll();
   QJsonDocument doc = QJsonDocument::fromJson(data);
   qDebug() << doc;
   emit searchFinished(doc);
}
