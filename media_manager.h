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
    virtual void load(QString) = 0;
    virtual void unload() = 0;
    virtual void faster() = 0;
    virtual void slower() = 0;
    virtual void setSpeed(double) = 0;
    virtual double getSpeed() = 0;
    virtual bool togglePause() = 0;
    virtual void clear() = 0;
    virtual void setVolume(double) = 0;
    virtual void resize(QResizeEvent*) = 0;
    const QStringList filetypes() {
        return FILETYPES;
    }

protected:
    const QStringList FILETYPES;
    MainWindow *main_window;
};

#endif // MEDIA_MANAGER_H
