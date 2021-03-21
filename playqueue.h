#ifndef PLAYQUEUE_H
#define PLAYQUEUE_H

#include <QObject>
#include <QVector>
#include <QMediaPlaylist>

#include "track.h"

class PlayQueue : public QObject
{
    Q_OBJECT
public:
    //The contructor set the index of the playlist to 0 and connect signals
    PlayQueue();

    //This function clears the Queue e add the playlist parameter to the Queue
    void replacePlayQueue(QVector<Track> playlist);

    //This function adds the playlist parameter withou clearing the Queue
    void addPlaylistToQueue(QVector<Track> playlist);

    //This function adds a single track to the Queue
    void addTrackToQueue(Track track);

    //This function clears the Queue
    void clearQueue();

    //This function removes a track from the playlist given a index
    void removeTrackFromQueue(int index);

    //This function updates the current track from the playlist to index
    void updateIndex(int index);

    //This function updates the current track from the playlist to the next one
    void gotoNextSong();

    //This function updates the current track from the playlist to the previous one
    void gotoPrevSong();

    //This function set the end of the Queue as the current track
    void goToTheEndOfTheQueue();

    //This function returns the currently playing track
    Track getCurrentTrack();

    //This function returns the Queue playlist as a QMediaPlaylist
    QMediaPlaylist& getPlaylist();

    //This function return the Queue playlist as a vector of tracks
    const QVector<Track>& getPlaylistVector();
private:
    //The playing Queue in Vector of tracks form
    QVector<Track> m_PlayQueuePanelList;

    //Information about the currently playing track
    Track m_currentTrack;

    //Index of the currently playing track
    int m_playingIndex;

    //The playing queue as QMediaPlaylist
    QMediaPlaylist m_playlist;
private slots:

    //This slot updates the current track index once the signal for index
    //updated is emited and emits the indexChanged signal with the index;
    void onCurrentIndexUpdated(int position);

    //This slot updates the current track information and emits the mediaChanged signal
    void onCurrentMediaChanged(const QMediaContent &content);
signals:
    //Signal emited when the queue is replaced with a playlist, sends the queue
    void replacedPlayQueue(QVector<Track>&);

    //Signal emited when a playlist is added to the queue, sends the added playlist
    void addedPlaylistToQueue(QVector<Track>&);

    //Signal emited when a track is added to the queue, sends the track
    void addedTrackToQueue(Track);

    //Signal emited when the queue is cleared
    void queueCleared();

    //Signal emited when the current track index changes, sends the index
    void indexChanged(int);

    //Signal emited when the current media changes
    void mediaChanged();

    //Signal emited when a track is removed, sends the index of the removed track
    void trackRemoved(int);
};

#endif // PLAYQUEUE_H
