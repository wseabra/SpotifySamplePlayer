#include "track.h"
#include <QDebug>

Track::Track()
{

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
