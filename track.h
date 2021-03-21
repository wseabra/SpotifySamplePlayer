#ifndef TRACK_H
#define TRACK_H

#include <QString>

//This class is a Model of a track
class Track
{
public:
    //This constructor creates an empty track
    Track();

    //This constructor creates an populated track
    Track(QString trackId, QString name, QString album,
          QString artist, QString image, QString previewUrl);

    QString trackId; //[items][n][id]
    QString name;//[items][n][name]
    QString album;//[items][n][album][name]
    QString artist;//[items][n][artists][0][name]
    QString image;//[items][n][album][images][2][url]
    QString previewUrl;//[items][n][preview_url]

    //Label for showing the track (name;artist;album)
    QString trackLabel;

    //Utility function to print track information for debug
    void printTrack();
};

#endif // TRACK_H
