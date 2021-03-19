#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QMediaPlayer>
#include <QAction>
#include <QListWidgetItem>
#include <QImage>

#include "track.h"
#include "spotify.h"
#include "playlistmanager.h"
#include "imagedownloaderhelper.h"
#include "playqueue.h"

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
    Spotify *m_spotify;
    PlaylistManager *m_playlists;
    QVector<Track> m_ExplorePanelList;
    Track m_playingTrack;
    QMediaPlayer m_mediaPlayer;
    ImageDownloaderHelper m_imageDownloader;
    PlayQueue m_queue;

    void doSearch();
private slots:
    void onAccessGranted();
    void onSearchFinished(QVector<Track> trackList);

    void onMediaStateChanged(QMediaPlayer::State state);
    void onMediaPlayerDurationChanged(qint64 duration);
    void onMediaPlayerPositionChanged(qint64 position);

    void onPlayTrackSelectedFromExplorer();
    void onAddTrackSelectedFromExplorer();

    void on_SearchButton_clicked();
    void on_SearchLineEdit_returnPressed();
    void on_ExploreView_customContextMenuRequested(const QPoint &pos);
    void on_PlayButton_clicked();
    void on_TrackProgress_sliderMoved(int position);
    void on_ExploreView_itemDoubleClicked(QListWidgetItem *item);

    void onDownloadCoverFinished(QImage image);

    void onTrackAddedToQueue(Track track);
    void onIndexOfQueueChanged(int index);
    void onPlayingMediaChanged();

    void on_QueueView_itemClicked(QListWidgetItem *item);
};
#endif // MAINWINDOW_H
