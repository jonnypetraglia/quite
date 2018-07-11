#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "image_manager.h"
#include "video_manager.h"

#include <QDebug>

#include <QMainWindow>
#include <QPushButton>
#include <QSpinbox>
#include <QDial>
#include <QTimer>
#include <QMenuBar>
#include <QWidgetList>
#include <QPair>
#include <QStackedWidget>

#include <QDir>
#include <QMimeData>
#include <QFileDialog>
#include <QLayout>
#include <QToolBar>
#include <QVector>
#include <QComboBox>

#include <QWidgetAction>

#include <QDirIterator>
#include <QFileInfoList>

class MainWindow
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

    // Don't want a context menu on the toolbar
    QMenu* createPopupMenu() { return NULL; }


    void loadFolder(QString folder, QFileInfo file = QFileInfo());
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
    void toggleStatusBar() {
        qDebug() << statusBar()->isVisible();
        if(statusBar()->isVisible())
            statusBar()->hide();
        else
            statusBar()->show();
        toggle_status->setChecked(statusBar()->isVisible());
    }
    void started() {
        volumeChange(volume_dial->value());
    }

    void position(qint64 position) {
        QTime now = QTime::fromMSecsSinceStartOfDay(position),
              end = QTime::fromMSecsSinceStartOfDay(current_manager->length());
        QString tForm("m:ss");
        if(end.hour()>0)
            tForm.prepend("h:m");
        sb_length->setText(now.toString(tForm) + "/" + end.toString(tForm));

    }

private:
    // GUI stuffs
    const int WIDTH = 731, HEIGHT = 518;

    // Current file & folder
    QFileInfoList list;
    int list_index;
    QString folder;

    // Menu items
    QAction* toggle_status;

    // Managers
    QVector<MediaManager*> managers;
    MediaManager* current_manager = NULL;

    // Misc widgets
    QStackedWidget* widget_stack;

    // Toolbar Widgets
    QVector<QWidget*> tb_widgets;
    QPushButton* reverse_button, *recurse_button;
    QSpinBox* slideshow_time;
    QDial* volume_dial;
    QPushButton* filetypes;
    QMenu* filetypes_menu;
    QPushButton* slideshow_button;

    // Statusbar Widgets
    QLabel* sb_length, *sb_speed, *sb_text;

    // Various Variables
    QTimer* slide_timer;
    bool goto_next_slide;
    QVector< QPair<QString, QDir::SortFlag> > sorts;
    QDir::SortFlag sort_order;


    // Non-Slot Functions
    void loadItem();
    void unloadItem();
    void clearItem();
    void startSlideshow();
    void stopSlideshow();
};

bool sortByName(const QFileInfo &s1, const QFileInfo &s2);
bool sortByDate(const QFileInfo &s1, const QFileInfo &s2);
bool sortBySize(const QFileInfo &s1, const QFileInfo &s2);
bool sortByType(const QFileInfo &s1, const QFileInfo &s2);

#endif // MAINWINDOW_H
