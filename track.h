#ifndef TRACK_H
#define TRACK_H

#include <QString>

class Track
{
public:
    Track();
    QString trackId; //[items][n][id]
    QString name;//[items][n][name]
    QString album;//[items][n][album][name]
    QString artist;//[items][n][artists][0][name]
    QString image;//[items][n][album][images][2][url]
    QString previewUrl;//[items][n][preview_url]

    void printTrack();
};

#endif // TRACK_H
