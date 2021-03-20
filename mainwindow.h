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
    Spotify *m_spotify_ptr;
    PlaylistManager *m_playlists_ptr;
    QVector<Track> m_ExplorePanelList;
    Track m_playingTrack;
    QMediaPlayer m_mediaPlayer;
    ImageDownloaderHelper m_imageDownloader;
    PlayQueue *m_queue_ptr;

    void doSearch();
    void addItemsToExplore(QVector<Track> playlist);
    void movePlayListToExplore();

private slots:
    void onAccessGranted();
    void onSearchFinished(QVector<Track> trackList);

    void onMediaStateChanged(QMediaPlayer::State state);
    void onMediaPlayerDurationChanged(qint64 duration);
    void onMediaPlayerPositionChanged(qint64 position);

    void onPlayTrackSelectedFromExplorer();
    void onAddTrackSelectedFromExplorer();
    void onAddAllSelectedFromExplorer();

    void on_SearchButton_clicked();
    void on_SearchLineEdit_returnPressed();
    void on_ExploreView_customContextMenuRequested(const QPoint &pos);
    void on_PlayButton_clicked();
    void on_TrackProgress_sliderMoved(int position);
    void on_ExploreView_itemDoubleClicked(QListWidgetItem *item);

    void onDownloadCoverFinished(QImage image);

    void onTrackAddedToQueue(Track track);
    void onAddedPlaylistToQueue(QVector<Track> playlist);
    void onIndexOfQueueChanged(int index);
    void onPlayingMediaChanged();
    void onTrackRemovedFromQueue(int index);
    void onQueueCleared();

    void on_QueueView_itemClicked(QListWidgetItem *item);
    void onPlaySelectedQueueItem();
    void onCreatePlaylistSelected();
    void onRemoveSelectedTrackFromQueue();
    void onCleanQueueSelected();
    void on_QueueView_customContextMenuRequested(const QPoint &pos);

    void onPlaylistCreated(QString name);
    void onPlaylistsLoaded(QVector<Playlist>* playlistList);
    void onPlaylistDeleted(int index);
    void on_PlaylistsView_itemClicked(QListWidgetItem *item);
    void onPlayPlaylistSelected();
    void onDeletePlaylistSelected();
    void on_PlaylistsView_customContextMenuRequested(const QPoint &pos);
};
#endif // MAINWINDOW_H
