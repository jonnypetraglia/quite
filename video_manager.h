#ifndef VIDEO_MANAGER_H
#define VIDEO_MANAGER_H

#include <QDebug>
#include <QMessageBox>

#include "media_manager.h"
#include <QMediaPlayer>
#include <QVideoWidget>

#include <QtAV/QtAV.h>

class VideoManager : public MediaManager
{
    Q_OBJECT
public:
    VideoManager(MainWindow*);
    ~VideoManager();
    QWidget* widget() { return video_output->widget(); }
    void load(QString, QObject* slotmate, char* slot);
    void unload();
    void setSpeed(double);
    void resize(QResizeEvent*);
    bool togglePause();
    void faster();
    void slower();
    void forward();
    void back();

    double getSpeed() {
        return video_player->speed() * 100;
    }
    void clear() {
        //TODO
    }
    double getVolume() {
        return video_player->audio()->volume() * 100;
    }

    void setVolume(double change) {
        video_player->audio()->setVolume(change / 100);
        qDebug() << change << video_player->audio()->volume();
    }

    bool hasVolume() { return true; }
    bool hasSeek() { return true; }

    qint64 length() {
        return video_player->duration();
    }

private:
    QtAV::VideoOutput *video_output;
    QtAV::AVPlayer *video_player;
};

#endif // VIDEO_MANAGER_H
