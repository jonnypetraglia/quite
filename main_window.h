#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "image_manager.h"
#include "video_manager.h"
#include "qwmainwindow/qweex_main_window.h"

#include <QDebug>

#include <QMainWindow>
#include <QPushButton>
#include <QSpinbox>
#include <QDial>
#include <QTimer>
#include <QMenuBar>
#include <QWidgetList>
#include <QPair>

#include <QDir>
#include <QMimeData>
#include <QFileDialog>
#include <QLayout>
#include <QToolBar>
#include <QVector>
#include <QComboBox>

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

    void showError(QString str, QString cause);
    
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
    void changeSort(int index) {
        qDebug() << "changeSort: " << sorts.at(index).first;
        sort_order = sorts.at(index).second;
        reverse_button->setEnabled(sort_order!=QDir::Unsorted);
        reloadFolder();
    }

private:
    // GUI stuffs
    const int WIDTH = 731, HEIGHT = 518;

    // Current file & folder
    QStringList list;
    int list_index;
    QString folder;

    // Managers
    QVector<MediaManager*> managers;
    MediaManager* current_manager = NULL;

    // Widgets
    QVector<QWidget*> tb_widgets;
    QPushButton* reverse_button;
    QSpinBox* slideshow_time;
    QDial* volume_dial;
    QPushButton* filetypes;
    QMenu* filetypes_menu;

    QLabel* tb_speed;
    QLabel* tb_text;
    QPushButton* slideshow_button;

    // Various Variables
    QTimer* slide_timer;
    bool goto_next_slide;
    QVector< QPair<QString, QDir::SortFlag> > sorts;
    QDir::SortFlag sort_order;


    // Non-Slot Functions
    void loadItem();
    void clearItem();
    void startSlideshow();
    void stopSlideshow();
};

#endif // MAINWINDOW_H
