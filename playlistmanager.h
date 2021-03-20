#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>

#include "track.h"
#include "constants.h"


class PlaylistManager : public QObject
{
    Q_OBJECT
public:
    PlaylistManager(QObject *parent);
    ~PlaylistManager();
    bool loadPlaylistsFromFile();
    void createPlaylist(QString name,QVector<Track> newPlaylist);
    void deletePlaylist(int playlistIndex);
    Playlist getPlaylistByIndex(int index);
private:
    QFile playlistFile;
    QJsonDocument playlists;
    QVector<Playlist> *m_playlistsVector_ptr;

    void processPlaylists();
    bool writePlaylistsToFile();
signals:
    void playlistsLoaded(QVector<Playlist>*);
    void playlistAdded(QString);
    void playlistDeleted(int);
};

#endif // PLAYLISTMANAGER_H
