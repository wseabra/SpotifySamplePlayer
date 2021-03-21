#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "track.h"
#include <QVector>

// Typedefs
typedef struct {
    QVector<Track> list;
    QString name;
}Playlist;

//Defines
#define PLAYLIST_FILE "playlists.json"
#define CLIENT_ID "7520ad3e20044620af7b1b533e1be843"
#define CLIENT_SECRET "e7725197cf914057a4d6c5b06c4888fd"
#define SPOTIFY_AUTHORIZATION_URL "https://accounts.spotify.com/authorize"
#define SPOTIFY_TOKEN_URL "https://accounts.spotify.com/api/token"
#define SPOTIFY_SCOPE "user-read-private user-top-read playlist-read-private playlist-modify-public playlist-modify-private"
#define SEARCH_URL "https://api.spotify.com/v1/search?q="
#define SEARCH_URL_PARAMETER "&type=track&market=from_token&limit=50"


//Strings
#define PLAY "Play"
#define PAUSE "Pause"
#define ADD_TO_QUEUE "Adicionar a fila"
#define ADD_ALL_TO_QUEUE "Adicionar todos a fila"
#define SEARCH_RESULT "Resultado da busca"
#define CREATE_PLAYLIST "Criar playlist"
#define REMOVE_FROM_QUEUE "Remover da fila"
#define CLEAN_QUEUE "Limpar fila"
#define DELETE_PLAYLIST "Apagar playlist"
#define PLAYLIST_NAME "Nome da playlist:"
#define NEW_PLAYLIST "Nova playlist"
#define UPDATE_PLAYLIST "Atualizar playlist"


//Json fields
#define TRACKS_FIELD "tracks"
#define ITEMS_FIELD "items"
#define ID_FIELD "id"
#define NAME_FIELD "name"
#define ALBUM_FIELD "album"
#define ARTISTS_FIELD "artists"
#define IMAGES_FIELD "images"
#define URL_FIELD "url"
#define PREVIEW_URL_FIELD "preview_url"
#define PLAYLISTS_FIELD "playlists"
#define TRACK_ID_FIELD "trackId"
#define ARTIST_FIELD "artist"
#define IMAGE_FIELD "image"
#endif // CONSTANTS_H
