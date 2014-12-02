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

#include <QDir>
#include <QMimeData>
#include <QFileDialog>
#include <QLayout>
#include <QToolBar>
#include <QVector>
#include <QComboBox>
#include <QMetaEnum>

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
    void changeSort(int enumInd) {
        const QMetaObject &mo = MainWindow::staticMetaObject;
        QMetaEnum metaEnum = mo.enumerator(mo.indexOfEnumerator("SORT"));
        sort_order = (SORT)metaEnum.value(enumInd);
        reloadFolder();
    }
    void changeRandom(bool checked) {
        sort_reverse = checked ? Yes : No;
        reloadFolder();
    }

public:
    // Enums
    enum SORT {
        Name = QDir::Name,
        Random = QDir::NoSort
        //TODO: MOAR
        // e.g.
        //
    };
    enum REVERSE {
        Yes = QDir::Reversed,
        No = 0 //Note: This will be OR'ed with SORT, hence the 0
    };
    Q_ENUMS(SORT)
    Q_ENUMS(REVERSE)

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
    QVector<QWidget*> status_bar_widgets;
    QCheckBox* random_order;
    QSpinBox* slideshow_time;
    QDial* volume_dial;
    QPushButton* filetypes;
    QMenu* filetypes_menu;

    QLabel* status_bar_speed;
    QLabel* status_bar_text;
    QPushButton* slideshow_button;

    // Various Variables
    QTimer* slide_timer;
    bool goto_next_slide;
    SORT sort_order;
    REVERSE sort_reverse;

    // Non-Slot Functions
    void loadItem();
    void clearItem();
    void startSlideshow();
    void stopSlideshow();
};

#endif // MAINWINDOW_H
