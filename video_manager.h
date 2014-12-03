#ifndef VIDEO_MANAGER_H
#define VIDEO_MANAGER_H

#include <QDebug>

#include "media_manager.h"
#include <QMediaPlayer>
#include <QVideoWidget>

class VideoManager : public MediaManager
{
    Q_OBJECT
public:
    VideoManager(MainWindow*);
    ~VideoManager();
    void load(QString);
    void unload();
    void setSpeed(double);
    void resize(QResizeEvent*);
    bool togglePause();
    void faster();
    void slower();

    double getSpeed() {
        return video_player->playbackRate() * 100;
    }
    void clear() {
        //TODO
    }
    double getVolume() {
        return video_player->volume();
    }

    void setVolume(double change) {
        video_player->setVolume(change);
    }

private:
    QMediaPlayer* video_player;
    QVideoWidget* video_widget;
};

#endif // VIDEO_MANAGER_H
