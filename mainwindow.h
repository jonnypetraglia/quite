#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include <QMainWindow>
#include <QPushButton>
#include <QSpinbox>
#include <QDial>
#include <QTimer>
#include <QMenuBar>

#include <QDir>
#include <QMimeData>
#include <QFileDialog>

#include "image_manager.h"
#include "video_manager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void resizeEvent(QResizeEvent * qre);
    ~MainWindow();

    void dragMoveEvent(QDragMoveEvent *de) { de->accept(); }
    void dragEnterEvent(QDragEnterEvent *event) { event->acceptProposedAction();}
    void dropEvent(QDropEvent*);


    void loadFolder(QString folder, QString file = "");
    
private:
    //Ui::MainWindow *ui;
    const int WIDTH = 731, HEIGHT = 518;
    QMenuBar* menu_bar;

    QStringList list;
    int list_index;
    QString folder;

    ImageManager* image_manager;
    VideoManager* video_manager;
    MediaManager* current_manager = NULL;

    QStatusBar* status_bar;
    QLabel* status_bar_text;
    QLabel* status_bar_speed;
    QPushButton* start_slideshow;
    QCheckBox* random_order;
    QSpinBox* slideshow_time;
    QDial* volume_dial;

    QTimer* slide_timer;
    bool goto_next_slide;

    // Item Related
    void loadItem();
    void clearItem();
    // Speed related
    void faster();
    void slower();
    void setSpeed(double d=100);
    // SlideshowRelated
    void startSlideshow();
    void stopSlideshow();

public slots:
    void browseForFolder(QAction*);
    void slideshowButton();
    void reloadFolder();
    void prevItem();
    void nextItem();
    void restartSlideshow(int na=0);
    void volumeChange(int);
    void nextItemWhenConvenient() {
        if(slide_timer->isActive() && goto_next_slide)
            nextItem();
        else
            goto_next_slide = true;
    }
};

#endif // MAINWINDOW_H
