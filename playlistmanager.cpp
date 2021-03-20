#include "playlistmanager.h"

PlaylistManager::PlaylistManager(QObject *parent):
    QObject(parent)
{
    m_playlistsVector_ptr = new QVector<Playlist>();

    playlistFile.setFileName(PLAYLIST_FILE);
}

PlaylistManager::~PlaylistManager()
{
    delete m_playlistsVector_ptr;
}

bool PlaylistManager::loadPlaylistsFromFile()
{
    if (!playlistFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }
    playlists = QJsonDocument::fromJson(playlistFile.readAll());
    playlistFile.close();
    processPlaylists();
    emit playlistsLoaded(m_playlistsVector_ptr);
    return true;
}

void PlaylistManager::processPlaylists()
{
    QJsonObject playlistsObj = playlists.object();
    auto playlistsArray = playlistsObj.value(PLAYLISTS_FIELD).toArray();

    for (int playlistIndex = 0; playlistIndex < playlistsArray.size(); playlistIndex++) {
        Playlist newPlaylist;

        QJsonObject playlistObject = playlistsArray.at(playlistIndex).toObject();
        newPlaylist.name = playlistObject.value(NAME_FIELD).toString();
        auto tracksArray = playlistObject.value(TRACKS_FIELD).toArray();

        for (int trackIndex = 0; trackIndex < tracksArray.size(); trackIndex++) {
            QJsonObject trackJson = tracksArray.at(trackIndex).toObject();

            QString trackId = trackJson[TRACK_ID_FIELD].toString();
            QString name = trackJson[NAME_FIELD].toString();
            QString album = trackJson[ALBUM_FIELD].toString();
            QString artist = trackJson[ARTIST_FIELD].toString();
            QString image = trackJson[IMAGE_FIELD].toString();
            QString preview_url = trackJson[PREVIEW_URL_FIELD].toString();

            Track newTrack(trackId,name,album,artist,image,preview_url);

            newPlaylist.list.push_back(newTrack);
        }
        m_playlistsVector_ptr->push_back(newPlaylist);
    }
}

bool PlaylistManager::writePlaylistsToFile()
{
    if (!playlistFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        return false;
    }
    playlistFile.write(playlists.toJson());
    playlistFile.close();
    return true;
}

void PlaylistManager::createPlaylist(QString name, QVector<Track> newPlaylist)
{
    Playlist newPlaylistObj;
    newPlaylistObj.list = newPlaylist;
    newPlaylistObj.name = name;
    m_playlistsVector_ptr->push_back(newPlaylistObj);


    auto playlistsObj = playlists.object();
    QJsonObject newPlaylistJson;
    QJsonArray trackList;

    foreach (auto track, newPlaylist) {
        QJsonObject trackJson;
        trackJson[TRACK_ID_FIELD] = track.trackId;
        trackJson[NAME_FIELD] = track.name;
        trackJson[ALBUM_FIELD] = track.album;
        trackJson[ARTIST_FIELD] = track.artist;
        trackJson[IMAGE_FIELD] = track.image;
        trackJson[PREVIEW_URL_FIELD] = track.previewUrl;

        trackList.append(trackJson);
    }

    newPlaylistJson.insert(TRACKS_FIELD,trackList);
    newPlaylistJson.insert(NAME_FIELD,name);

    auto playlistArray = playlistsObj.value(PLAYLISTS_FIELD).toArray();
    playlistArray.append(newPlaylistJson);
    playlistsObj.insert(PLAYLISTS_FIELD,playlistArray);

    QJsonDocument newDoc(playlistsObj);
    playlists = newDoc;
    writePlaylistsToFile();

    emit playlistAdded(newPlaylistObj.name);
}

void PlaylistManager::deletePlaylist(int playlistIndex)
{
    QJsonObject playlistsObj = playlists.object();
    QJsonArray playlistArray = playlistsObj.value(PLAYLISTS_FIELD).toArray();

    playlistArray.removeAt(playlistIndex);

    playlistsObj[PLAYLISTS_FIELD] = playlistArray;

    QJsonDocument newDoc(playlistsObj);
    playlists = newDoc;

    m_playlistsVector_ptr->removeAt(playlistIndex);
    writePlaylistsToFile();
    emit playlistDeleted(playlistIndex);
}

Playlist PlaylistManager::getPlaylistByIndex(int index)
{
    Playlist returnedPlaylist;
    if(index < m_playlistsVector_ptr->size()){
        returnedPlaylist = m_playlistsVector_ptr->at(index);
    }
    return returnedPlaylist;
}
