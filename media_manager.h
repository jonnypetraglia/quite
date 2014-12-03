#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <QDebug>

#include <QStatusBar>
#include <QStringList>
#include <QCheckBox>
#include <QString>
#include <QResizeEvent>
#include <QVector>


class MainWindow;

class MediaManager : public QObject {
    Q_OBJECT
public:
    MediaManager(QStringList filetypes) : FILETYPES(filetypes) {}
    virtual QWidget* widget() = 0;
    virtual void load(QString) = 0;
    virtual void unload() = 0;
    virtual void faster() = 0;
    virtual void slower() = 0;
    virtual void setSpeed(double) = 0;
    virtual double getSpeed() = 0;
    virtual void forward() = 0;
    virtual void back() = 0;
    virtual bool togglePause() = 0;
    virtual void setVolume(double) = 0;
    virtual double getVolume() = 0;
    virtual void resize(QResizeEvent*) = 0;
    const QStringList filetypes() {
        return FILETYPES;
    }
    virtual bool hasVolume() = 0;
    virtual bool hasSeek() = 0;

protected:
    const QStringList FILETYPES;
    MainWindow *main_window = NULL;
};

#endif // MEDIA_MANAGER_H
