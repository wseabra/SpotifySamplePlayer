#include "playqueue.h"

PlayQueue::PlayQueue()
{

    m_playlist.setCurrentIndex(0);
    connect(&m_playlist,&QMediaPlaylist::currentIndexChanged,this,&PlayQueue::onCurrentIndexUpdated);
    connect(&m_playlist,&QMediaPlaylist::currentMediaChanged,this,&PlayQueue::onCurrentMediaChanged);
}

void PlayQueue::replacePlayQueue(QVector<Track> playlist)
{
    m_PlayQueuePanelList.swap(playlist);
    m_playlist.clear();
    m_playlist.setCurrentIndex(0);
    foreach (auto track, m_PlayQueuePanelList) {
        m_playlist.addMedia(QUrl(track.previewUrl));
    }
    emit replacedPlayQueue(m_PlayQueuePanelList);
}

void PlayQueue::addPlaylistToQueue(QVector<Track> playlist)
{
    foreach (auto track, playlist) {
        m_PlayQueuePanelList.push_back(track);
        m_playlist.addMedia(QUrl(track.previewUrl));
    }
    emit addedPlaylistToQueue(playlist);
}

void PlayQueue::addTrackToQueue(Track track)
{
    m_PlayQueuePanelList.push_back(track);
    m_playlist.addMedia(QUrl(track.previewUrl));
    emit addedTrackToQueue(track);
}

void PlayQueue::clearQueue()
{
    m_PlayQueuePanelList.clear();
    m_playlist.clear();
    emit queueCleared();
}

void PlayQueue::removeTrackFromQueue(int index)
{
    if(!m_playlist.media(index).isNull() && index < m_PlayQueuePanelList.size()) {
        m_playlist.removeMedia(index);
        m_PlayQueuePanelList.removeAt(index);
        emit trackRemoved(index);
    }
}

void PlayQueue::updateIndex(int index)
{
    m_playlist.setCurrentIndex(index);
}

void PlayQueue::gotoNextSong()
{
    m_playlist.next();
}

void PlayQueue::gotoPrevSong()
{
    m_playlist.previous();
}

void PlayQueue::goToTheEndOfTheQueue()
{
    m_playlist.setCurrentIndex(m_PlayQueuePanelList.size() -1);
}

Track PlayQueue::getCurrentTrack()
{
    return m_currentTrack;
}

QMediaPlaylist &PlayQueue::getPlaylist()
{
    return m_playlist;
}

const QVector<Track> &PlayQueue::getPlaylistVector()
{
   return m_PlayQueuePanelList;
}

void PlayQueue::onCurrentIndexUpdated(int position)
{
   m_playingIndex = position;
   emit indexChanged(m_playingIndex);
}

void PlayQueue::onCurrentMediaChanged(const QMediaContent &content)
{
    if (!content.isNull()){
        m_currentTrack = m_PlayQueuePanelList[m_playingIndex];
        emit mediaChanged();
    }
}
