#include "video_manager.h"
#include "quite_window.h"

VideoManager::VideoManager(MainWindow* window)
    : MediaManager(QStringList() << "flv" << "gifv" << "mov" << "mp4" << "webm")
{
    main_window = window;

    video_player = new QtAV::AVPlayer(main_window);
    video_output = new QtAV::VideoOutput(main_window);
    if(!video_output->widget()) {
        QMessageBox::warning(0, "QtAV error", "Can not create video renderer");
        return;
    }
    video_player->setRepeat(-1);
    video_player->setRenderer(video_output);
}

VideoManager::~VideoManager()
{
    delete video_player;
    delete video_output;
}

void VideoManager::load(QString file, QObject* slotOwner, char* updateSlot)
{
    qDebug() << "Playing " << QUrl::fromLocalFile(file);
    //connect(video_player, SIGNAL(positionChanged(qint64)), slotOwner, updateSlot);
    connect(video_player, SIGNAL(started()), slotOwner, updateSlot);
    video_player->play(file);
}

void VideoManager::unload()
{
    video_player->stop();
//    video_player->unload();
}

void VideoManager::setSpeed(double speedFrom0To100)
{
    video_player->setSpeed(speedFrom0To100 / 100);
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
    //video_player->seekBackward();
    video_player->seek(qMax(video_player->position() - 5000, (long long)0));
}

void VideoManager::forward()
{
    //video_player->seekForward();
    video_player->seek(qMin(video_player->position() + 5000, video_player->duration()));
}

bool VideoManager::togglePause()
{
    video_player->togglePause();
    return video_player->isPaused();
}

void VideoManager::resize(QResizeEvent *qre)
{
    //TODO: what do when movie resize
}
