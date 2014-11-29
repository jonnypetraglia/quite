#include "video_manager.h"
#include "mainwindow.h"

VideoManager::VideoManager(MainWindow* window, QStatusBar* status_bar, const char* filetype_check_slot)
{
    main_window = window;
    video_player = new QMediaPlayer;
    video_widget = new QVideoWidget;
    video_player->setVideoOutput(video_widget);
    video_widget->setStyleSheet("QWidget { background-color: black; }");

    FILETYPES << "*.mov" << "*.mp4"; //<< "*.webm"

    initChecks(status_bar, filetype_check_slot);
}

VideoManager::~VideoManager()
{
    qDebug() << "Destroying videoManager";
    delete video_player;
    delete video_widget;
    qDebug() << "Destroyed videoManager";
}


void VideoManager::load(QString file)
{
    main_window->setCentralWidget(video_widget);

    qDebug() << "Playing " << QUrl::fromLocalFile(file);

    video_player->setMedia(QUrl::fromLocalFile(file));
    //videoWidget->show();
    video_player->setMuted(true);
    video_player->play();
    setSpeed(0);
}

void VideoManager::setSpeed(double speedFrom0To100)
{
    video_player->setPlaybackRate(speedFrom0To100 / 100);
    qDebug() << "speed " << speedFrom0To100;
}

void VideoManager::resize(QResizeEvent *qre)
{
    //TODO: what do when movie resize
}
