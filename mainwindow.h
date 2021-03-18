#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "track.h"

#include "spotify.h"
#include "playlistmanager.h"

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
private slots:
    void onAccessGranted();
    void onSearchFinished(QVector<Track> trackList);
};
#endif // MAINWINDOW_H
