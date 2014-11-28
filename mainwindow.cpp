#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFile>
#include <QMovie>
#include <QKeyEvent>
#include <QDir>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setLayout(ui->horizontalLayout);
    img_durr = this->findChild<QLabel*>("img_durr");
    img_durr->setMovie(new QMovie);
    img_durr->movie()->setScaledSize(QSize(100,100));

    setAcceptDrops(true);

    FILETYPES << "*.jpg" << "*.png" << "*.gif";
    slideshowTime = 3000;

    status_bar = this->findChild<QStatusBar*>("statusBar");
    filetype_checks = new QCheckBox*[FILETYPES.length()];
    for(int i=0; i<FILETYPES.length(); i++)
    {
        status_bar->addWidget(filetype_checks[i] = new QCheckBox(FILETYPES[i].right(FILETYPES[i].length()-2)));
        filetype_checks[i]->setChecked(true);
        filetype_checks[i]->setFocusPolicy(Qt::NoFocus);
        connect(filetype_checks[i], SIGNAL(clicked()), this, SLOT(reloadFolder()));
    }
    status_bar->addWidget(status_bar_speed = new QLabel());
    status_bar->addWidget(start_slideshow = new QPushButton("Slide"));
    status_bar->addWidget(slideshow_time = new QSpinBox());
    slideshow_time->setValue(3);
    slideshow_time->setSuffix("s");
    slideshow_time->setRange(1, 60);
    slideshow_time->setFocusPolicy(Qt::NoFocus);
    start_slideshow->setFocusPolicy(Qt::NoFocus);
    status_bar->addWidget(status_bar_text = new QLabel(),1);
    status_bar_text->setAlignment(Qt::AlignRight);


    connect(start_slideshow, SIGNAL(clicked()), this, SLOT(slideshowButton()));
    connect(&slideshowTimer,SIGNAL(timeout()), this, SLOT(nextImage()));
    connect(slideshow_time, SIGNAL(valueChanged(int)), this, SLOT(restartSlideshow(int)));
}

void MainWindow::slideshowButton()
{
    if(slideshowTimer.isActive())
        stopSlideshow();
    else
        startSlideshow();
}

void MainWindow::reloadFolder()
{
    if(list_index>=0 && list_index<list.size())
        loadFolder(this->folder, list.at(list_index));
    else
        loadFolder(this->folder);
}

void MainWindow::loadFolder(QString folder, QString file)
{
    QStringList filetypes_want;
    for(int i=0; i<FILETYPES.length(); i++)
        if(filetype_checks[i]->isChecked())
            filetypes_want << FILETYPES[i];

    if(filetypes_want.size()==0)
    {
        clearImage();
        return;
    }

    this->folder = folder;
    QDir _folder(folder);
    std::cerr << filetypes_want.join(QChar(' ')).toStdString() << std::endl;
    list = _folder.entryList(filetypes_want);
    std::cout << "loadFolder " << folder.toStdString() << " " << file.toStdString() << std::endl;
    for(int i=0; i<list.size(); i++)
        std::cout << "  loadFolder " << list.at(i).toStdString() << std::endl;

    if(list.size()==0)
    {
        clearImage();
        return;
    }
    if(file=="" || list.indexOf(file,0)<0)
        list_index = 0;
    else
        list_index = list.indexOf(file,0);
    std::cout << "loadFolder " << list_index << " of " << list.size() << std::endl;
    setImage();
}


void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier)) {
        //Zoom
    } else if (event->modifiers().testFlag(Qt::AltModifier)) {
        if(event->delta()>0)
            faster();
        else
            slower();
    } else {
        if(event->delta()<0)
            nextImage();
        else
            prevImage();
    }

}

void MainWindow::mousePressEvent(QMouseEvent * me)
{
    return;
    if(me->button()==Qt::LeftButton)
        nextImage();
    else if(me->button()==Qt::RightButton)
        prevImage();
}

void MainWindow::keyPressEvent(QKeyEvent * e)
{
    e->accept();
    switch (e->key()) {
        case Qt::Key_Right:
            nextImage();
            return;
        case Qt::Key_Left:
            prevImage();
            return;
        case Qt::Key_Plus:
            faster();
            return;
        case Qt::Key_Minus:
            slower();
            return;
        case Qt::Key_0:
            resetSpeed();
            return;
        case Qt::Key_AsciiTilde:
            startSlideshow();
            return;
        case Qt::Key_Escape:
            stopSlideshow();
            return;
        default:
            return;
    }
}


void MainWindow::nextImage()
{
    ++list_index;
    if(list_index==list.size())
        list_index = 0;
    setImage();
}

void MainWindow::prevImage()
{
    if(list_index==0)
        list_index = list.size();
    --list_index;
    setImage();
}
void MainWindow::faster()
{
    if(img_durr->movie()->speed() >= 100)
    {
        if(img_durr->movie()->speed() >= 1000)
        {
            if(img_durr->movie()->speed()+1000 < 0)
                return;
            img_durr->movie()->setSpeed(img_durr->movie()->speed()+1000);
        } else {
            img_durr->movie()->setSpeed(img_durr->movie()->speed()+100);
        }
    } else {
        if(img_durr->movie()->speed() >= 60)
            img_durr->movie()->setSpeed(img_durr->movie()->speed()+10);
        else
            img_durr->movie()->setSpeed(img_durr->movie()->speed()+5);
    }
    img_durr->movie()->start();
    std::cout << "speed " << img_durr->movie()->speed() << std::endl;
    status_bar_speed->setText(QString("%1x").arg(img_durr->movie()->speed()/100.0));
}

void MainWindow::slower()
{
    if(img_durr->movie()->speed() > 100)
        img_durr->movie()->setSpeed(img_durr->movie()->speed()-50);
    else {
        if(img_durr->movie()->speed() > 60)
            img_durr->movie()->setSpeed(img_durr->movie()->speed()-10);
        else if(img_durr->movie()->speed() > 5)
            img_durr->movie()->setSpeed(img_durr->movie()->speed()-5);
        else
            return;

    }
    img_durr->movie()->start();
    std::cout << "speed " << img_durr->movie()->speed() << std::endl;
    status_bar_speed->setText(QString("%1x").arg(img_durr->movie()->speed()/100.0));
}

void MainWindow::resetSpeed()
{
    img_durr->movie()->setSpeed(100);
    img_durr->movie()->start();
    std::cout << "speed " << img_durr->movie()->speed() << std::endl;
    status_bar_speed->setText(QString("%1x").arg(img_durr->movie()->speed()/100.0));
}

void MainWindow::startSlideshow()
{
    slideshowTimer.start(slideshowTime);
    std::cout << "starting slideshow" << std::endl;
    //this->showFullScreen();
}

void MainWindow::stopSlideshow()
{
    slideshowTimer.stop();
    std::cout << "stahping slideshow" << std::endl;
    //this->showNormal();
}

void MainWindow::clearImage()
{
    std::cout << "clearing img" << std::endl;
    if(img_durr->movie()==NULL)
    {
        img_durr->setMovie(new QMovie);
        img_durr->movie()->setScaledSize(QSize(100,100));
    }
    list_index = 0;
    img_durr->movie()->stop();
    this->setWindowTitle("<None>");
    status_bar_text->setText("<Folder empty>");
    status_bar_speed->setText("");

    img_durr->setText("No image");
}

void MainWindow::setImage()
{
    if(img_durr->movie()==NULL)
    {
        img_durr->setMovie(new QMovie);
        img_durr->movie()->setScaledSize(QSize(100,100));
    }
    img_durr->movie()->stop();

    QString file = folder + "/" + list.at(list_index);
    this->setWindowTitle(list.at(list_index));

    img_durr->movie()->setFileName(file);
    status_bar_text->setText(QFileInfo(file).fileName());

    QList<QByteArray> test = img_durr->movie()->supportedFormats();

    img_size = QPixmap(file).size();
    QSize newsize = img_size;
    newsize.scale(size().width()-100, size().height()-100, Qt::KeepAspectRatio);
    img_durr->movie()->setScaledSize(newsize);

    resetSpeed();
    img_durr->movie()->start();
}

void MainWindow::resizeEvent(QResizeEvent * qre)
{
    QSize newsize = img_size;
    newsize.scale(qre->size().width(), qre->size().height(), Qt::KeepAspectRatio);
    img_durr->movie()->setScaledSize(newsize);
    img_durr->movie()->start();

    qre->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
}
