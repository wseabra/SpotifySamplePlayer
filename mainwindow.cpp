#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "Testing";
    ui->setupUi(this);

    m_playlists = new PlaylistManager();

    m_spotify = new Spotify(this,"7520ad3e20044620af7b1b533e1be843","e7725197cf914057a4d6c5b06c4888fd");
    m_spotify->grant();

    connect(m_spotify,&Spotify::accessGranted,this,&MainWindow::onAccessGranted);
    connect(m_spotify,&Spotify::searchFinished,this,&MainWindow::onSearchFinished);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAccessGranted()
{
    qDebug() << "MainWindow access granted";
}

void MainWindow::onSearchFinished(QVector<Track> trackList)
{
}



