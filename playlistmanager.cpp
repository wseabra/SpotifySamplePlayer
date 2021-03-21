#include "playlistmanager.h"

PlaylistManager::PlaylistManager(QObject *parent):
    QObject(parent)
{
    m_playlistsVector_ptr = new QVector<Playlist>();

    m_playlistFile.setFileName(PLAYLIST_FILE);
}

PlaylistManager::~PlaylistManager()
{
    delete m_playlistsVector_ptr;
}

void PlaylistManager::loadPlaylistsFromFile()
{
    if (!m_playlistFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit playlistsLoaded(m_playlistsVector_ptr);
        return;
    }
    m_playlists = QJsonDocument::fromJson(m_playlistFile.readAll());
    m_playlistFile.close();
    processPlaylists();
    emit playlistsLoaded(m_playlistsVector_ptr);
}

void PlaylistManager::processPlaylists()
{
    QJsonObject playlistsObj = m_playlists.object();

    if (!playlistsObj.contains(PLAYLISTS_FIELD))
        return;

    auto playlistsArray = playlistsObj.value(PLAYLISTS_FIELD).toArray();

    //Loop's the playlist array
    for (int playlistIndex = 0; playlistIndex < playlistsArray.size(); playlistIndex++) {
        Playlist newPlaylist;

        QJsonObject playlistObject = playlistsArray.at(playlistIndex).toObject();

        newPlaylist.name = playlistObject.value(NAME_FIELD).toString();
        auto tracksArray = playlistObject.value(TRACKS_FIELD).toArray();

        //Loop's the tracks array
        for (int trackIndex = 0; trackIndex < tracksArray.size(); trackIndex++) {
            QJsonObject trackJson = tracksArray.at(trackIndex).toObject();

            //Getting the track fields to create the track object
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

void PlaylistManager::writePlaylistsToFile()
{
    if (!m_playlistFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        if(m_playlistFile.exists()){
            m_playlistFile.moveToTrash();
            m_playlistFile.open(QIODevice::WriteOnly | QIODevice::Text);
        }
    }
    m_playlistFile.write(m_playlists.toJson());
    m_playlistFile.close();
}

void PlaylistManager::createPlaylist(QString name, QVector<Track> newPlaylist)
{
    Playlist newPlaylistObj;
    newPlaylistObj.list = newPlaylist;
    newPlaylistObj.name = name;
    m_playlistsVector_ptr->push_back(newPlaylistObj);


    auto playlistsObj = m_playlists.object();

    QJsonObject newPlaylistJson;

    QJsonArray trackList;

    //for each track create a new json track object
    foreach (auto track, newPlaylist) {
        QJsonObject trackJson;
        trackJson[TRACK_ID_FIELD] = track.trackId;
        trackJson[NAME_FIELD] = track.name;
        trackJson[ALBUM_FIELD] = track.album;
        trackJson[ARTIST_FIELD] = track.artist;
        trackJson[IMAGE_FIELD] = track.image;
        trackJson[PREVIEW_URL_FIELD] = track.previewUrl;

        //append the new Track to the json track array
        trackList.append(trackJson);
    }

    //Populate the new playlist json object fields
    newPlaylistJson.insert(TRACKS_FIELD,trackList);
    newPlaylistJson.insert(NAME_FIELD,name);

    auto playlistArray = playlistsObj.value(PLAYLISTS_FIELD).toArray();
    playlistArray.append(newPlaylistJson);
    playlistsObj.insert(PLAYLISTS_FIELD,playlistArray);

    QJsonDocument newDoc(playlistsObj);
    m_playlists = newDoc;

    writePlaylistsToFile();

    emit playlistAdded(newPlaylistObj.name);
}

void PlaylistManager::deletePlaylist(int playlistIndex)
{
    QJsonObject playlistsObj = m_playlists.object();
    QJsonArray playlistArray = playlistsObj.value(PLAYLISTS_FIELD).toArray();

    playlistArray.removeAt(playlistIndex);

    //Updating the playlists array
    playlistsObj[PLAYLISTS_FIELD] = playlistArray;

    QJsonDocument newDoc(playlistsObj);
    m_playlists = newDoc;

    //Removing the playlist from the playlist vector
    m_playlistsVector_ptr->removeAt(playlistIndex);

    writePlaylistsToFile();

    emit playlistDeleted(playlistIndex);
}

void PlaylistManager::updatePlaylist(int playlistIndex, QVector<Track> updatedPlaylist)
{
    QJsonObject playlistsObj = m_playlists.object();
    QJsonArray playlistArray = playlistsObj.value(PLAYLISTS_FIELD).toArray();

    QJsonObject playlistObj = playlistArray.at(playlistIndex).toObject();
    QJsonArray newTrackList;

    foreach (auto track, updatedPlaylist) {
        QJsonObject trackJson;
        trackJson[TRACK_ID_FIELD] = track.trackId;
        trackJson[NAME_FIELD] = track.name;
        trackJson[ALBUM_FIELD] = track.album;
        trackJson[ARTIST_FIELD] = track.artist;
        trackJson[IMAGE_FIELD] = track.image;
        trackJson[PREVIEW_URL_FIELD] = track.previewUrl;

        //append the new Track to the json track array
        newTrackList.append(trackJson);
    }
    playlistObj.insert(TRACKS_FIELD,newTrackList);
    playlistArray[playlistIndex] = playlistObj;

    //Updating the playlists arrays
    playlistsObj[PLAYLISTS_FIELD] = playlistArray;

    (*m_playlistsVector_ptr)[playlistIndex].list = updatedPlaylist;

    QJsonDocument newDoc(playlistsObj);
    m_playlists = newDoc;

    writePlaylistsToFile();
}

Playlist PlaylistManager::getPlaylistByIndex(int index)
{
    Playlist returnedPlaylist;
    if(index < m_playlistsVector_ptr->size()){
        returnedPlaylist = m_playlistsVector_ptr->at(index);
    }
    return returnedPlaylist;
}
