#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QFile>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "track.h"

#define PLAYLIST_FILE "playlists.json"

class PlaylistManager
{
public:
    PlaylistManager();
    void createPlaylist(QString name);
    void addTrackToPlaylist(int playlistIndex, Track track);
    void removeTrackFromPlaylist(int playlistIndex, int trackIndex);
    void deletePlaylist(int playlistIndex);
private:
    QFile playlistFile;
    QJsonDocument playlists;

    bool loadPlaylistsFromFile(QJsonDocument &playlists);
    bool writePlaylistsToFile(const QJsonDocument &playlists);
};

#endif // PLAYLISTMANAGER_H
