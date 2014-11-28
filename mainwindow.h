#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QStringList>
#include <QResizeEvent>
#include <QSize>
#include <QTimer>
#include <QStatusBar>
#include <QCheckBox>
#include <QLabel>
#include <QMimeData>
#include <QPushButton>
#include <QSpinbox>

#include <QMediaPlayer>
#include <QVideoWidget>
#include <iostream>
#include <QFileInfo>
#include <QDir>

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
    void loadFolder(QString folder, QString file = "");

    void dragMoveEvent(QDragMoveEvent *de) { de->accept(); }
    void dragEnterEvent(QDragEnterEvent *event) { event->acceptProposedAction();}

    void dropEvent(QDropEvent *de)
    {
        if(de->mimeData()->hasUrls())
        {
            QString path = de->mimeData()->urls().first().toString().remove(0, QString("file:///").length());
            QFileInfo pathInfo = QFileInfo(path);
            std::cout << "dropped: " << path.toStdString() << std::endl;
            int ind = FILETYPES.indexOf(QString("*.").append(pathInfo.suffix()));
            if(pathInfo.isDir() || ind>=0)
            {
                if(ind>=0)
                    filetype_checks[ind]->setChecked(true);
                if(pathInfo.isDir())
                    loadFolder(path);
                else
                    loadFolder(pathInfo.dir().absolutePath(), pathInfo.fileName());
            } else {
                std::cerr << "ERROR Invalid filetype: " << pathInfo.suffix().toStdString() << std::endl;
            }
        }
    }
    
private:
    Ui::MainWindow *ui;
    QLabel* img_durr;
    QStringList list;
    int list_index;
    QString folder;
    QSize img_size;
    bool movie_finished, next_slide;

    QStringList FILETYPES;
    QStatusBar* status_bar;
    QLabel* status_bar_text;
    QLabel* status_bar_speed;
    QCheckBox** filetype_checks;
    QPushButton* start_slideshow;
    QCheckBox* random_order;
    QSpinBox* slideshow_time;

    QTimer slideshowTimer;

    void setImage();
    void clearImage();
    void faster();
    void slower();
    void resetSpeed();
    void startSlideshow();
    void stopSlideshow();

public slots:
    void slideshowButton();
    void reloadFolder();
    void prevImage();
    void nextImage();
    void nextSlide();
    void restartSlideshow(int na=0);
    void movieFinished(int);
};

#endif // MAINWINDOW_H
