#ifndef TRACK_H
#define TRACK_H

#include <QString>

class Track
{
public:
    Track();
    Track(QString trackId, QString name, QString album,
          QString artist, QString image, QString previewUrl);
    QString trackId; //[items][n][id]
    QString name;//[items][n][name]
    QString album;//[items][n][album][name]
    QString artist;//[items][n][artists][0][name]
    QString image;//[items][n][album][images][2][url]
    QString previewUrl;//[items][n][preview_url]
    QString trackLabel;

    void printTrack();
};

#endif // TRACK_H
