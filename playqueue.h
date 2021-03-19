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
    PlayQueue();
    void replacePlayQueue(QVector<Track> playlist);
    void addPlaylistToQueue(QVector<Track> playlist);
    void addTrackToQueue(Track track);
    void clearQueue();
    void updateIndex(int index);
    void goToTheEndOfTheQueue();
    Track getCurrentTrack();
    QMediaPlaylist& getPlaylist();
    const QVector<Track>& getPlaylistVector();
private:
    QVector<Track> m_PlayQueuePanelList;
    Track m_currentTrack;
    int m_playingIndex;
    QMediaPlaylist m_playlist;
private slots:
    void onCurrentIndexUpdated(int position);
    void onCurrentMediaChanged(const QMediaContent &content);
signals:
    void replacedPlayQueue(QVector<Track>&);
    void addedPlaylistToQueue(QVector<Track>&);
    void addedTrackToQueue(Track);
    void queueCleared();
    void indexChanged(int);
    void mediaChanged();
};

#endif // PLAYQUEUE_H
