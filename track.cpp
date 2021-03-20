#include "track.h"
#include <QDebug>

Track::Track()
{

}

Track::Track(QString trackId, QString name, QString album,
             QString artist, QString image, QString previewUrl):
    trackId(trackId),
    name(name),
    album(album),
    artist(artist),
    image(image),
    previewUrl(previewUrl)
{
    trackLabel = name+"; "+album+"; "+artist;

}

void Track::printTrack()
{
    qDebug() << trackId;
    qDebug() << name;
    qDebug() << album;
    qDebug() << artist;
    qDebug() << image;
    qDebug() << previewUrl;
}
