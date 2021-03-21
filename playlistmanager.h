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

//This class manages the playlist file and the playlist list loaded on memory
class PlaylistManager : public QObject
{
    Q_OBJECT
public:
    //The constructor allocates the playlist vector
    //and set the playlist file name
    PlaylistManager(QObject *parent);

    //The destructor deletes the playlist vector from memory
    ~PlaylistManager();

    //This function loads the playlist json to memory and calls
    //the function to proccess the playlists
    void loadPlaylistsFromFile();

    //This function creates a playlist given a name and a track vector
    //and save the new playlist to the playlist json file
    void createPlaylist(QString name,QVector<Track> newPlaylist);

    //This function deletes the playlist given an index
    //and save the deletion to the playlist json file
    void deletePlaylist(int playlistIndex);

    //This function updates a playlist given an index and an updated track list
    //and saves the update to the playlist json file
    void updatePlaylist(int playlistIndex,QVector<Track> updatedPlaylist);

    //This function returns a Playlist given an index
    Playlist getPlaylistByIndex(int index);
private:

    //Object for accessing the playlist file
    QFile m_playlistFile;

    //Json object that holds the playlists list
    QJsonDocument m_playlists;

    //Pointer for a vector of playlists loaded to memory
    QVector<Playlist> *m_playlistsVector_ptr;

    //This function process the loaded json object and saves the loaded
    //palylists in the playlist vector
    void processPlaylists();

    //This function writes the playlist json object to the file
    void writePlaylistsToFile();
signals:

    //Signal emited once the playlist is loaded and proccess returning the
    //playlists vector pointer
    void playlistsLoaded(QVector<Playlist>*);

    //Signal emited once the new playlist is added, returning the playlist name
    void playlistAdded(QString);

    //Signal emited once the playlist is loaded, returning the index deleted
    void playlistDeleted(int);
};

#endif // PLAYLISTMANAGER_H
