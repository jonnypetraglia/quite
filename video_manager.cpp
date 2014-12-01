#include "video_manager.h"
#include "main_window.h"

VideoManager::VideoManager(MainWindow* window)
    : MediaManager(QStringList() << "mov" << "mp4") //, << "webm")
{
    main_window = window;
    video_player = new QMediaPlayer;
    video_widget = new QVideoWidget;
    video_player->setVideoOutput(video_widget);
    video_widget->setStyleSheet("QWidget { background-color: black; }");
}

VideoManager::~VideoManager()
{
    //delete video_player;
    //delete video_widget;
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

void VideoManager::faster()
{
    double speed = getSpeed();
    if(speed > 100)
    {
        if(speed >= 250)
            return;
        else
            speed += 10;
    } else {
        if(speed >= 60)
            speed += 5;
        else
            speed += 2;
    }
    main_window->setSpeed(speed);
}

void VideoManager::slower()
{
    double speed = getSpeed();
    if(speed > 100)
        speed -= 10;
    else {
        if(speed > 60)
            speed -= 5;
        else if(speed > 5)
            speed -= 2;
        else
            return;
    }
    main_window->setSpeed(speed);
}

bool VideoManager::togglePause()
{
    if(video_player->state()==QMediaPlayer::PausedState)
        video_player->play();
    else
        video_player->pause();
    return video_player->state()==QMediaPlayer::PausedState;
}

void VideoManager::resize(QResizeEvent *qre)
{
    //TODO: what do when movie resize
}
