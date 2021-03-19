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

//void MainWindow::playTrack(Track track)
//{
//    m_mediaPlayer.play();
//    m_mediaPlayer.setMedia(QUrl(track.previewUrl));
//    ui->TrackLabel->setText(track.trackLabel);
//    m_imageDownloader.downloadImage(track.image);
//}

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

void MainWindow::on_ExploreView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->ExploreView->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Play",this,SLOT(onPlayTrackSelectedFromExplorer()));
    myMenu.addAction("Adicionar a fila",this,SLOT(onAddTrackSelectedFromExplorer()));
    myMenu.addAction("Adicionar todos a fila",this,SLOT(onPlayTrackSelectedFromExplorer()));
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

void MainWindow::on_QueueView_itemClicked(QListWidgetItem *item)
{
    int row = ui->QueueView->currentRow();
    m_queue.updateIndex(row);
}
