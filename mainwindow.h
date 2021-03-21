#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QMediaPlayer>
#include <QAction>
#include <QListWidgetItem>
#include <QImage>
#include <QInputDialog>

#include "track.h"
#include "spotify.h"
#include "playlistmanager.h"
#include "imagedownloaderhelper.h"
#include "playqueue.h"
#include "constants.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //Spotify controller pointer
    Spotify *m_spotify_ptr;

    //Playlists controller pointer
    PlaylistManager *m_playlists_ptr;

    //Data vector for the explore panel
    QVector<Track> m_ExplorePanelList;

    //Current track information
    Track m_playingTrack;

    //Media player
    QMediaPlayer m_mediaPlayer;

    //Helper for downloading track covers
    ImageDownloaderHelper m_imageDownloader;

    //Queue controller pointer
    PlayQueue *m_queue_ptr;

    //This function executes spotify search
    void doSearch();

    //This function updates items in the explore panel
    void addItemsToExplore(QVector<Track> playlist);

    //This function moves a playlist to the explore panel
    void movePlayListToExplore();

//Slots beggining with on_ are created by QTCreator, you will not find the connect on the cpp file
private slots:
    //Spotify API slots
    void onAccessGranted();
    void onSearchFinished(QVector<Track> trackList);

    //Media control slots
    void onMediaStateChanged(QMediaPlayer::State state);
    void onMediaPlayerDurationChanged(qint64 duration);
    void onMediaPlayerPositionChanged(qint64 position);

    //Explore panel slots
    void onPlayTrackSelectedFromExplorer();
    void onAddTrackSelectedFromExplorer();
    void onAddAllSelectedFromExplorer();
    void on_ExploreView_customContextMenuRequested(const QPoint &pos);

    //Ui slots
    void on_SearchButton_clicked();
    void on_SearchLineEdit_returnPressed();
    void on_PlayButton_clicked();
    void on_TrackProgress_sliderMoved(int position);
    void on_ExploreView_itemDoubleClicked(QListWidgetItem *item);

    //Image helper slot
    void onDownloadCoverFinished(QImage image);

    //Queue control slots
    void onTrackAddedToQueue(Track track);
    void onAddedPlaylistToQueue(QVector<Track> playlist);
    void onIndexOfQueueChanged(int index);
    void onPlayingMediaChanged();
    void onTrackRemovedFromQueue(int index);
    void onQueueCleared();

    void on_QueueView_itemClicked(QListWidgetItem *item);
    void onPlaySelectedQueueItem();
    void onCreatePlaylistSelected();
    void onUpdatePlaylistSelected();
    void onRemoveSelectedTrackFromQueue();
    void onCleanQueueSelected();
    void on_QueueView_customContextMenuRequested(const QPoint &pos);

    //Playlist control slots
    void onPlaylistCreated(QString name);
    void onPlaylistsLoaded(QVector<Playlist>* playlistList);
    void onPlaylistDeleted(int index);
    void on_PlaylistsView_itemClicked(QListWidgetItem *item);
    void onPlayPlaylistSelected();
    void onDeletePlaylistSelected();
    void on_PlaylistsView_customContextMenuRequested(const QPoint &pos);
};
#endif // MAINWINDOW_H
