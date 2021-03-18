#include "playlistmanager.h"

PlaylistManager::PlaylistManager()
{
    playlistFile.setFileName(PLAYLIST_FILE);
    loadPlaylistsFromFile(playlists);
    auto playlistsObj = playlists.object();
    if (!playlists.object().contains("playlists")) {
        playlistsObj.insert("playlists",QJsonArray());
        QJsonDocument newDoc(playlistsObj);
        playlists = newDoc;
        writePlaylistsToFile(playlists);
    }
}

bool PlaylistManager::loadPlaylistsFromFile(QJsonDocument &playlists)
{
    if (!playlistFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }
    playlists = QJsonDocument::fromJson(playlistFile.readAll());
    playlistFile.close();
    return true;
}

bool PlaylistManager::writePlaylistsToFile(const QJsonDocument &playlists)
{
    if (!playlistFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        return false;
    }
    playlistFile.write(playlists.toJson());
    playlistFile.close();
    return true;
}

void PlaylistManager::createPlaylist(QString name)
{
    auto playlistsObj = playlists.object();
    QJsonObject newPlaylist;
    QJsonArray trackList;
    newPlaylist.insert("tracks",trackList);
    newPlaylist.insert("name",name);

    auto playlistArray = playlistsObj.value("playlists").toArray();
    playlistArray.append(newPlaylist);
    playlistsObj.insert("playlists",playlistArray);

    QJsonDocument newDoc(playlistsObj);
    playlists = newDoc;
    writePlaylistsToFile(playlists);
}

void PlaylistManager::addTrackToPlaylist(int playlistIndex, Track track)
{
    QJsonObject playlistsObj = playlists.object();
    QJsonArray playlistArray = playlistsObj.value("playlists").toArray();
    QJsonObject playlistItem = playlistArray[playlistIndex].toObject();
    QJsonArray trackArray = playlistItem.value("tracks").toArray();

    QJsonObject trackJson;
    trackJson["trackId"] = track.trackId;
    trackJson["name"] = track.name;
    trackJson["album"] = track.album;
    trackJson["artist"] = track.artist;
    trackJson["image"] = track.image;
    trackJson["preview_url"] = track.previewUrl;

    trackArray.append(trackJson);
    playlistItem["tracks"] = trackArray;
    playlistArray[playlistIndex] = playlistItem;
    playlistsObj["playlists"] = playlistArray;

    QJsonDocument newDoc(playlistsObj);
    playlists = newDoc;
    writePlaylistsToFile(playlists);
}

void PlaylistManager::removeTrackFromPlaylist(int playlistIndex, int trackIndex)
{
    QJsonObject playlistsObj = playlists.object();
    QJsonArray playlistArray = playlistsObj.value("playlists").toArray();
    QJsonObject playlistItem = playlistArray[playlistIndex].toObject();
    QJsonArray trackArray = playlistItem.value("tracks").toArray();

    trackArray.removeAt(trackIndex);

    playlistItem["tracks"] = trackArray;
    playlistArray[playlistIndex] = playlistItem;
    playlistsObj["playlists"] = playlistArray;

    QJsonDocument newDoc(playlistsObj);
    playlists = newDoc;
    writePlaylistsToFile(playlists);
}

void PlaylistManager::deletePlaylist(int playlistIndex)
{
    QJsonObject playlistsObj = playlists.object();
    QJsonArray playlistArray = playlistsObj.value("playlists").toArray();

    playlistArray.removeAt(playlistIndex);

    playlistsObj["playlists"] = playlistArray;

    QJsonDocument newDoc(playlistsObj);
    playlists = newDoc;
    writePlaylistsToFile(playlists);
}
