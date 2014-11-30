#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "image_manager.h"
#include "video_manager.h"
#include "qweex/qweex_main_window.h"

#include <QDebug>

#include <QMainWindow>
#include <QPushButton>
#include <QSpinbox>
#include <QDial>
#include <QTimer>
#include <QMenuBar>
#include <QWidgetList>

#include <QDir>
#include <QMimeData>
#include <QFileDialog>
#include <QLayout>
#include <QToolBar>

class MainWindow : public Qweex::MainWindow
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
    void setSpeed(double d=100);
    
public slots:
    void browseForFile();
    void browseForFolder();
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

private:
    const int WIDTH = 731, HEIGHT = 518;
    QStringList list;
    int list_index;
    QString folder;

    ImageManager* image_manager;
    VideoManager* video_manager;
    MediaManager* current_manager = NULL;

    QVector<QWidget*> status_bar_widgets;
    QLabel* status_bar_text;
    QLabel* status_bar_speed;
    QPushButton* slideshow_button;
    QCheckBox* random_order;
    QSpinBox* slideshow_time;
    QDial* volume_dial;

    QTimer* slide_timer;
    bool goto_next_slide;

    // Item Related
    void loadItem();
    void clearItem();
    // SlideshowRelated
    void startSlideshow();
    void stopSlideshow();
};

#endif // MAINWINDOW_H
