#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMenu>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "Testing";
    ui->setupUi(this);

    m_playlists_ptr = new PlaylistManager(this);

    m_spotify_ptr = new Spotify(this,CLIENT_ID,CLIENT_SECRET);
    m_spotify_ptr->grant();

    connect(&m_imageDownloader,&ImageDownloaderHelper::downloadFinished,this,&MainWindow::onDownloadCoverFinished);

    connect(m_spotify_ptr,&Spotify::accessGranted,this,&MainWindow::onAccessGranted);
    connect(m_spotify_ptr,&Spotify::searchFinished,this,&MainWindow::onSearchFinished);

    m_queue_ptr = new PlayQueue();
    auto& playlistObject = m_queue_ptr->getPlaylist();
    m_mediaPlayer.setPlaylist(&playlistObject);

    connect(&m_mediaPlayer,&QMediaPlayer::stateChanged,this,&MainWindow::onMediaStateChanged);
    connect(&m_mediaPlayer,&QMediaPlayer::durationChanged,this,&MainWindow::onMediaPlayerDurationChanged);
    connect(&m_mediaPlayer,&QMediaPlayer::positionChanged,this,&MainWindow::onMediaPlayerPositionChanged);

    connect(m_queue_ptr,&PlayQueue::addedTrackToQueue,this,&MainWindow::onTrackAddedToQueue);
    connect(m_queue_ptr,&PlayQueue::indexChanged,this,&MainWindow::onIndexOfQueueChanged);
    connect(m_queue_ptr,&PlayQueue::mediaChanged,this,&MainWindow::onPlayingMediaChanged);
    connect(m_queue_ptr,&PlayQueue::addedPlaylistToQueue,this,&MainWindow::onAddedPlaylistToQueue);
    connect(m_queue_ptr,&PlayQueue::trackRemoved,this,&MainWindow::onTrackRemovedFromQueue);
    connect(m_queue_ptr,&PlayQueue::queueCleared,this,&MainWindow::onQueueCleared);

    connect(m_playlists_ptr,&PlaylistManager::playlistAdded,this,&MainWindow::onPlaylistCreated);
    connect(m_playlists_ptr,&PlaylistManager::playlistsLoaded,this,&MainWindow::onPlaylistsLoaded);
    connect(m_playlists_ptr,&PlaylistManager::playlistDeleted,this,&MainWindow::onPlaylistDeleted);

    m_playlists_ptr->loadPlaylistsFromFile();

}

MainWindow::~MainWindow()
{
    delete m_spotify_ptr;
    delete m_playlists_ptr;
    delete m_queue_ptr;
    delete ui;
}

void MainWindow::doSearch()
{
    QString query;
    query = ui->SearchLineEdit->text();
    m_spotify_ptr->searchTrack(query);
}

void MainWindow::addItemsToExplore(QVector<Track> playlist)
{
    ui->ExploreView->clear();
    foreach (auto track, playlist) {
        ui->ExploreView->addItem(track.trackLabel);
    }
    m_ExplorePanelList.swap(playlist);

}

void MainWindow::movePlayListToExplore()
{
   int row = ui->PlaylistsView->currentRow();
   Playlist selectedPlaylist = m_playlists_ptr->getPlaylistByIndex(row);
   addItemsToExplore(selectedPlaylist.list);
   ui->ExploreLabel->setText(selectedPlaylist.name);
}

void MainWindow::onAccessGranted()
{
    qDebug() << "MainWindow access granted";
}

void MainWindow::onSearchFinished(QVector<Track> trackList)
{
    addItemsToExplore(trackList);
    ui->ExploreLabel->setText(SEARCH_RESULT);
}

void MainWindow::on_SearchButton_clicked()
{
    doSearch();
}

void MainWindow::on_SearchLineEdit_returnPressed()
{
    doSearch();
}

void MainWindow::onPlayTrackSelectedFromExplorer()
{
    int row = ui->ExploreView->currentRow();
    Track track = m_ExplorePanelList[row];
    m_queue_ptr->addTrackToQueue(track);
    m_queue_ptr->goToTheEndOfTheQueue();
    m_mediaPlayer.play();
}

void MainWindow::onAddTrackSelectedFromExplorer()
{
    int row = ui->ExploreView->currentRow();
    Track track = m_ExplorePanelList[row];

    m_queue_ptr->addTrackToQueue(track);
}

void MainWindow::onAddAllSelectedFromExplorer()
{
    m_queue_ptr->addPlaylistToQueue(m_ExplorePanelList);
}

void MainWindow::on_ExploreView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->ExploreView->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction(PLAY,this,SLOT(onPlayTrackSelectedFromExplorer()));
    myMenu.addAction(ADD_TO_QUEUE,this,SLOT(onAddTrackSelectedFromExplorer()));
    myMenu.addAction(ADD_ALL_TO_QUEUE,this,SLOT(onAddAllSelectedFromExplorer()));
    myMenu.exec(globalPos);
}

void MainWindow::on_PlayButton_clicked()
{
    switch (m_mediaPlayer.state()) {
    case QMediaPlayer::State::PlayingState :
        m_mediaPlayer.pause();
       break;
    case QMediaPlayer::State::PausedState :
        m_mediaPlayer.play();
        break;
    case QMediaPlayer::State::StoppedState :
        m_mediaPlayer.play();
        break;
    }
}

void MainWindow::onMediaStateChanged(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::State::PlayingState :
        ui->PlayButton->setText(PAUSE);
       break;
    case QMediaPlayer::State::PausedState :
        ui->PlayButton->setText(PLAY);
        break;
    case QMediaPlayer::State::StoppedState :
        ui->PlayButton->setText(PLAY);
        break;
    }

}

void MainWindow::onMediaPlayerDurationChanged(qint64 duration)
{
    QTime durationTime(0,0,0);
    durationTime =  durationTime.addMSecs(duration);
    ui->DurationLabel->setText(durationTime.toString("mm:ss"));
    ui->TrackProgress->setMaximum(duration);
}

void MainWindow::onMediaPlayerPositionChanged(qint64 position)
{
    QTime elapsed(0,0,0);
    elapsed = elapsed.addMSecs(position);
    ui->PlayedLabel->setText(elapsed.toString("mm:ss"));
    ui->TrackProgress->setValue(position);
}

void MainWindow::on_TrackProgress_sliderMoved(int position)
{
   m_mediaPlayer.setPosition(position);
}

void MainWindow::on_ExploreView_itemDoubleClicked(QListWidgetItem *item)
{
    int row = ui->ExploreView->currentRow();
    m_playingTrack = m_ExplorePanelList[row];
    onPlayTrackSelectedFromExplorer();
}

void MainWindow::onDownloadCoverFinished(QImage image)
{
    ui->TrackCover->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::onTrackAddedToQueue(Track track)
{
    ui->QueueView->addItem(track.trackLabel);
}

void MainWindow::onAddedPlaylistToQueue(QVector<Track> playlist)
{
    foreach (auto track, playlist) {
        ui->QueueView->addItem(track.trackLabel);
    }
}

void MainWindow::onIndexOfQueueChanged(int index)
{
    ui->QueueView->setCurrentRow(index);
}

void MainWindow::onPlayingMediaChanged()
{
    Track track = m_queue_ptr->getCurrentTrack();
    ui->TrackLabel->setText(track.trackLabel);
    m_imageDownloader.downloadImage(track.image);
}

void MainWindow::onTrackRemovedFromQueue(int index)
{
    auto item = ui->QueueView->takeItem(index);
    delete item;
}

void MainWindow::onQueueCleared()
{
    ui->QueueView->clear();
}

void MainWindow::on_QueueView_itemClicked(QListWidgetItem *item)
{
    onPlaySelectedQueueItem();
}

void MainWindow::onPlaySelectedQueueItem()
{
    int row = ui->QueueView->currentRow();
    m_queue_ptr->updateIndex(row);
    if (m_mediaPlayer.state() != QMediaPlayer::State::PlayingState)
        m_mediaPlayer.play();
}

void MainWindow::onCreatePlaylistSelected()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr(CREATE_PLAYLIST),
                                         tr(PLAYLIST_NAME), QLineEdit::Normal,
                                         tr(NEW_PLAYLIST), &ok);
    if (ok && !text.isEmpty()){
        m_playlists_ptr->createPlaylist(text,m_queue_ptr->getPlaylistVector());
    }
}

void MainWindow::onRemoveSelectedTrackFromQueue()
{
    int row = ui->QueueView->currentRow();
    m_queue_ptr->removeTrackFromQueue(row);
}

void MainWindow::onCleanQueueSelected()
{
    m_queue_ptr->clearQueue();
}

void MainWindow::on_QueueView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->QueueView->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction(PLAY,this,SLOT(onPlaySelectedQueueItem()));
    myMenu.addAction(CREATE_PLAYLIST,this,SLOT(onCreatePlaylistSelected()));
    myMenu.addAction(REMOVE_FROM_QUEUE,this,SLOT(onRemoveSelectedTrackFromQueue()));
    myMenu.addAction(CLEAN_QUEUE,this,SLOT(onCleanQueueSelected()));
    myMenu.exec(globalPos);
}

void MainWindow::onPlaylistCreated(QString name)
{
    ui->PlaylistsView->addItem(name);
}

void MainWindow::onPlaylistsLoaded(QVector<Playlist>* playlistList)
{
    for (int i = 0; i < playlistList->size(); i++)
    {
        Playlist playlist = playlistList->at(i);
        ui->PlaylistsView->addItem(playlist.name);
    }
}

void MainWindow::onPlaylistDeleted(int index)
{
    auto item = ui->PlaylistsView->takeItem(index);
    delete item;
}

void MainWindow::on_PlaylistsView_itemClicked(QListWidgetItem *item)
{
    movePlayListToExplore();
}

void MainWindow::onPlayPlaylistSelected()
{
    movePlayListToExplore();
    m_queue_ptr->addPlaylistToQueue(m_ExplorePanelList);
    m_mediaPlayer.play();
}

void MainWindow::onDeletePlaylistSelected()
{
    int row = ui->PlaylistsView->currentRow();
    m_playlists_ptr->deletePlaylist(row);
}

void MainWindow::on_PlaylistsView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->PlaylistsView->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction(PLAY,this,SLOT(onPlayPlaylistSelected()));
    myMenu.addAction(DELETE_PLAYLIST,this,SLOT(onDeletePlaylistSelected()));
    myMenu.exec(globalPos);
}
