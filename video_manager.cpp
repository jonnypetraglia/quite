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
    qDebug() << "Playing " << QUrl::fromLocalFile(file);

    video_player->setMedia(QUrl::fromLocalFile(file));
    video_player->setPosition(0);
    video_player->setMuted(true); //:OPTIONS
    //setSpeed(100);
    video_player->play();
}

void VideoManager::unload()
{
    video_player->stop();
    video_player->setMedia(NULL);
}

void VideoManager::setSpeed(double speedFrom0To100)
{
    video_player->setPlaybackRate(speedFrom0To100 / 100);
    qDebug() << "speed " << speedFrom0To100;
}

void VideoManager::faster()
{
    double speed = getSpeed();
    if(speed >= 60)
        speed += 5;
    else if(speed >= 20)
        speed += 2;
    else
        speed += 1;

    if(speed >= 250)
        return;
    main_window->setSpeed(speed);
}

void VideoManager::slower()
{
    double speed = getSpeed();
    if(speed >= 60)
        speed -= 5;
    else if(speed >= 20)
        speed -= 2;
    else if(speed > 0)
        speed -= 1;

    if(speed<0)
        return;
    qDebug() << speed;
    main_window->setSpeed(speed);
}

void VideoManager::back()
{
    video_player->setPosition(qMax(video_player->position() - 5000, (long long)0));
}

void VideoManager::forward()
{
    video_player->setPosition(qMin(video_player->position() + 5000, video_player->duration()));
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
