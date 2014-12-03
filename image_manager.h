#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <QDebug>

#include "media_manager.h"
#include <QLabel>
#include <QStatusBar>
#include <QMovie>

class ImageManager : public MediaManager
{
    Q_OBJECT
public:
    ImageManager(MainWindow*);
    void load(QString);
    void unload();
    void setSpeed(double);
    void resize(QResizeEvent*);
    bool togglePause();
    void faster();
    void slower();

    double getSpeed() {
        return image_widget->movie()->speed();
    }
    void setVolume(double d) {
        do{d=0;}while(0); //Noop
        qCritical() << "ImageManager does not have volume.";
        throw "ImageManager does not have volume.";
    }
    double getVolume() {
        qCritical() << "ImageManager does not have volume.";
        throw "ImageManager does not have volume.";
        return 0;
    }

public slots:
    void movieFinished(int);
    void next();

private:
    QLabel* image_widget;
    QSize img_size;

    bool movie_is_finished;

};

#endif // IMAGE_MANAGER_H
