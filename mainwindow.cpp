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

    m_playlists = new PlaylistManager();

    m_spotify = new Spotify(this,"7520ad3e20044620af7b1b533e1be843","e7725197cf914057a4d6c5b06c4888fd");
    m_spotify->grant();

    connect(&m_imageDownloader,&ImageDownloaderHelper::downloadFinished,this,&MainWindow::onDownloadCoverFinished);

    connect(m_spotify,&Spotify::accessGranted,this,&MainWindow::onAccessGranted);
    connect(m_spotify,&Spotify::searchFinished,this,&MainWindow::onSearchFinished);

    auto& playlistObject = m_queue.getPlaylist();
    m_mediaPlayer.setPlaylist(&playlistObject);

    connect(&m_mediaPlayer,&QMediaPlayer::stateChanged,this,&MainWindow::onMediaStateChanged);
    connect(&m_mediaPlayer,&QMediaPlayer::durationChanged,this,&MainWindow::onMediaPlayerDurationChanged);
    connect(&m_mediaPlayer,&QMediaPlayer::positionChanged,this,&MainWindow::onMediaPlayerPositionChanged);

    connect(&m_queue,&PlayQueue::addedTrackToQueue,this,&MainWindow::onTrackAddedToQueue);
    connect(&m_queue,&PlayQueue::indexChanged,this,&MainWindow::onIndexOfQueueChanged);
    connect(&m_queue,&PlayQueue::mediaChanged,this,&MainWindow::onPlayingMediaChanged);
    connect(&m_queue,&PlayQueue::addedPlaylistToQueue,this,&MainWindow::onAddedPlaylistToQueue);
    connect(&m_queue,&PlayQueue::trackRemoved,this,&MainWindow::onTrackRemovedFromQueue);
    connect(&m_queue,&PlayQueue::queueCleared,this,&MainWindow::onQueueCleared);

}

MainWindow::~MainWindow()
{
    delete m_spotify;
    delete m_playlists;
    delete ui;
}

void MainWindow::doSearch()
{
    QString query;
    query = ui->SearchLineEdit->text();
    m_spotify->searchTrack(query);
}

void MainWindow::onAccessGranted()
{
    qDebug() << "MainWindow access granted";
}

void MainWindow::onSearchFinished(QVector<Track> trackList)
{
    ui->ExploreView->clear();
    foreach (auto track, trackList) {
        ui->ExploreView->addItem(track.trackLabel);
    }
    m_ExplorePanelList.swap(trackList);
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
    m_queue.addTrackToQueue(track);
    m_queue.goToTheEndOfTheQueue();
    m_mediaPlayer.play();
}

void MainWindow::onAddTrackSelectedFromExplorer()
{
    int row = ui->ExploreView->currentRow();
    Track track = m_ExplorePanelList[row];

    m_queue.addTrackToQueue(track);
}

void MainWindow::onAddAllSelectedFromExplorer()
{
    m_queue.addPlaylistToQueue(m_ExplorePanelList);
}

void MainWindow::on_ExploreView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->ExploreView->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Play",this,SLOT(onPlayTrackSelectedFromExplorer()));
    myMenu.addAction("Adicionar a fila",this,SLOT(onAddTrackSelectedFromExplorer()));
    myMenu.addAction("Adicionar todos a fila",this,SLOT(onAddAllSelectedFromExplorer()));
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
        ui->PlayButton->setText("Pause");
       break;
    case QMediaPlayer::State::PausedState :
        ui->PlayButton->setText("Play");
        break;
    case QMediaPlayer::State::StoppedState :
        ui->PlayButton->setText("Play");
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
    Track track = m_queue.getCurrentTrack();
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
    m_queue.updateIndex(row);
    if (m_mediaPlayer.state() != QMediaPlayer::State::PlayingState)
        m_mediaPlayer.play();
}

void MainWindow::onRemoveSelectedTrackFromQueue()
{
    int row = ui->QueueView->currentRow();
    m_queue.removeTrackFromQueue(row);
}

void MainWindow::onCleanQueueSelected()
{
    m_queue.clearQueue();
}

void MainWindow::on_QueueView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->QueueView->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Play",this,SLOT(onPlaySelectedQueueItem()));
    myMenu.addAction("Remover da fila",this,SLOT(onRemoveSelectedTrackFromQueue()));
    myMenu.addAction("Limpar fila",this,SLOT(onCleanQueueSelected()));
    myMenu.exec(globalPos);
}
