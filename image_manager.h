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
    void setSpeed(double);
    void resize(QResizeEvent*);
    bool togglePause();
    void faster();
    void slower();

    double getSpeed() {
        return image_widget->movie()->speed();
    }
    void clear() {
        if(image_widget->movie()==NULL)
        {
            image_widget->setMovie(new QMovie);
            image_widget->movie()->setScaledSize(QSize(100,100));
        }
        image_widget->movie()->stop();
        image_widget->setText("No item");
    }
    void setVolume(double d) {
        do{d=0;}while(0); //Noop
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
