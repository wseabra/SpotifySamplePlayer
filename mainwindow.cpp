#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "Testing";
    ui->setupUi(this);

    m_spotify = new Spotify(this,"7520ad3e20044620af7b1b533e1be843","e7725197cf914057a4d6c5b06c4888fd");
    m_spotify->grant();
}

MainWindow::~MainWindow()
{
    delete ui;
}
