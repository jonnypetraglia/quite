#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "quca.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    //, ui(new Ui::MainWindow)
{
    setWindowTitle("Quite");
    slide_timer = new QTimer();

    qDebug() << "Main Window Started";
    setGeometry(QRect(0, 0, WIDTH, HEIGHT));
    QSizePolicy pol(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pol.setHorizontalStretch(0);
    pol.setVerticalStretch(0);
    setSizePolicy(pol);
    layout()->setMargin(11);
    layout()->setSpacing(6);
    qDebug() << "X";

    menu_bar = new QMenuBar(this);
    menu_bar->setGeometry(0, 0, WIDTH, 22);
    status_bar = new QStatusBar(this);

    qDebug() << "Menu Bar Created";

    image_manager = new ImageManager(this, status_bar, SLOT(reloadFolder()));
    video_manager = new VideoManager(this, status_bar, SLOT(reloadFolder()));

    qDebug() << "Managers created";

    QWidget* central = new QWidget(this);
    this->setCentralWidget(central);

    qDebug() << "Layout init";

    // Add sum menu items
    QMenu* browse = menu_bar->addMenu("Browse");
    connect(browse, SIGNAL(triggered(QAction*)), this, SLOT(browseForFolder(QAction*)));

    // Add sum widgets
    status_bar->addWidget(status_bar_speed = new QLabel());

    status_bar->addWidget(start_slideshow = new QPushButton("Slide"));
    start_slideshow->setFocusPolicy(Qt::NoFocus);

    status_bar->addWidget(slideshow_time = new QSpinBox());
    slideshow_time->setValue(3);
    slideshow_time->setSuffix("s");
    slideshow_time->setRange(1, 60);
    slideshow_time->setFocusPolicy(Qt::NoFocus);

    status_bar->addWidget(random_order = new QCheckBox("Random"));

    status_bar->addWidget(status_bar_text = new QLabel(), 1);
    status_bar_text->setAlignment(Qt::AlignRight);

    status_bar->addWidget(volume_dial = new QDial());
    volume_dial->setFocusPolicy(Qt::NoFocus);
    volume_dial->setMinimum(0);
    volume_dial->setMaximum(100);
    volume_dial->setNotchesVisible(true);
    volume_dial->setSingleStep(5);
    volume_dial->setWrapping(false); //WTF does this do?

    qDebug() << "Connecting signals to slots";

    connect(random_order, SIGNAL(clicked()), this, SLOT(reloadFolder()));
    connect(start_slideshow, SIGNAL(clicked()), this, SLOT(slideshowButton()));
    connect(slide_timer, SIGNAL(timeout()), this, SLOT(nextItemWhenConvenient()));
    connect(slideshow_time, SIGNAL(valueChanged(int)), this, SLOT(restartSlideshow(int)));
    connect(volume_dial, SIGNAL(valueChanged(int)), this, SLOT(volumeChange(int)));

    qDebug() << "Adding Widgets";


    //this->layout()->addWidget(central);
    this->layout()->addWidget(menu_bar);
    //this->layout()->addItem(tool_bar);
    //this->layout()->addWidget(status_bar);
    //*/
    qDebug() << "mainWindow created";
}

// Destructor
MainWindow::~MainWindow()
{
    qDebug() << "Destroying mainWindow";

    delete slide_timer;
    delete menu_bar;
    delete status_bar;
    delete image_manager;
    delete video_manager;
    delete status_bar_speed;
    delete start_slideshow;
    delete slideshow_time;
    delete random_order;
    delete status_bar_text;
    delete volume_dial;
    //delete ui;

    qDebug() << "Destroyed mainWindow";
}


// Start or stop the slideshow
void MainWindow::slideshowButton()
{
    qDebug() << "Slideshow Button";
    if(slide_timer->isActive()) {
        stopSlideshow();
        start_slideshow->setText("Slide");
    } else {
        startSlideshow();
        start_slideshow->setText("Stop");
    }
}

// Call 'loadFolder' again on the current folder (& current file)
void MainWindow::reloadFolder()
{
    qDebug() << "reloading folder";
    if(list_index>=0 && list_index<list.size())
        loadFolder(this->folder, list.at(list_index));
    else
        loadFolder(this->folder);
}

// Read a folder's files
void MainWindow::loadFolder(QString folder, QString file)
{
    qDebug() << "Loading Folder " << folder;
    QStringList filetypes_want;
    image_manager->addCheckedFiletypesTo(filetypes_want);
    video_manager->addCheckedFiletypesTo(filetypes_want);

    // If there are no checked filetypes
    if(filetypes_want.size()==0) {
        clearItem(); return; }

    // Set the folder & load the files
    this->folder = folder;
    QDir _folder(folder);
    list = _folder.entryList(filetypes_want);

    // If there are no files
    if(list.size()==0) {
        clearItem(); return; }

    qDebug() << "Files: " << list.size();

    // Sort (or anti-sort) the list
    if(random_order->isChecked())
        std::random_shuffle(list.begin(), list.end());
    else
        QUnicodeCollationAlgorithm::collatorKeySort(list);

    // Set the current file's index in the list
    if(file=="" || list.indexOf(file,0)<0)
        list_index = 0;
    else
        list_index = list.indexOf(file,0);

    loadItem();
}

void MainWindow::browseForFolder(QAction* action)
{
    loadFolder(QFileDialog::getExistingDirectory());
}


void MainWindow::wheelEvent(QWheelEvent *event)
{
    qDebug() << "WheelEvent";
    if (event->modifiers().testFlag(Qt::ControlModifier)) {
        //Zoom
    } else if (event->modifiers().testFlag(Qt::AltModifier)) {
        if(event->delta()>0)
            faster();
        else
            slower();
    } else {
        if(event->delta()<0)
            nextItem();
        else
            prevItem();
    }

}

void MainWindow::mousePressEvent(QMouseEvent * me)
{
    return;
    if(me->button()==Qt::LeftButton)
        nextItem();
    else if(me->button()==Qt::RightButton)
        prevItem();
}

void MainWindow::keyPressEvent(QKeyEvent * e)
{
    qDebug() << "keyPressEvent";
    e->accept();
    switch (e->key()) {
        case Qt::Key_Right:
            nextItem();
            break;
        case Qt::Key_Left:
            prevItem();
            break;
        case Qt::Key_Plus:
            faster();
            break;
        case Qt::Key_Minus:
            slower();
            break;
        case Qt::Key_0:
            setSpeed();
            break;
        case Qt::Key_AsciiTilde:
            startSlideshow();
            return;
        case Qt::Key_Escape:
            stopSlideshow();
            return;
        case Qt::Key_Space:
            slideshowButton();
            return;
        default:
            return;
    }
    if(slide_timer->isActive())
        restartSlideshow();
}


void MainWindow::nextItem()
{
    qDebug() << "nextItem";
    ++list_index;
    if(list_index==list.size())
        list_index = 0;
    loadItem();
}

void MainWindow::prevItem()
{
    qDebug() << "prevItem";
    if(list_index==0)
        list_index = list.size();
    --list_index;
    loadItem();
}
void MainWindow::faster()
{
    double speed = current_manager->getSpeed();
    if(speed >= 100)
    {
        if(speed >= 1000)
            speed += 1000;
        else
            speed += 100;
    } else {
        if(speed >= 60)
            speed += 10;
        else
            speed += 5;
    }
    setSpeed(speed);
}

void MainWindow::slower()
{
    qDebug() << "slower";
    double speed = current_manager->getSpeed();
    if(speed > 100)
        speed -= 50;
    else {
        if(speed > 60)
            speed -= 10;
        else if(speed > 5)
            speed -= 5;
        else
            return;
    }
    setSpeed(speed);
}

void MainWindow::setSpeed(double d)
{
    qDebug() << "setSpeed " << d;
    current_manager->setSpeed(d);
    status_bar_speed->setText(QString("%1x").arg(d/100.0));
}

void MainWindow::startSlideshow()
{
    qDebug() << "starting slideshow";
    slide_timer->start(slideshow_time->value() * 1000);
    //this->showFullScreen();
}

void MainWindow::stopSlideshow()
{
    qDebug() << "stahping slideshow";
    slide_timer->stop();
    //this->showNormal();
}

void MainWindow::restartSlideshow(int na)
{
    qDebug() << "restarting slideshow";
    na = 0; //-Wunused-parameter
    stopSlideshow();
    startSlideshow();
}


void MainWindow::clearItem()
{
    qDebug() << "clearing item";
    current_manager->clear();
    list_index = 0;
    this->setWindowTitle("<None>");
    status_bar_text->setText("<Folder empty>");
    status_bar_speed->setText("");
}

// Load the file that is marked as the current file
void MainWindow::loadItem()
{
    qDebug() << "Loading Item";
    QString file = folder + "/" + list.at(list_index);
    QFileInfo fileInfo = QFileInfo(file);
    this->setWindowTitle(fileInfo.fileName());
    status_bar_text->setText(fileInfo.fileName());

    qDebug() << "Loading File " << file;

    goto_next_slide = false;
    if(image_manager->hasExtension(fileInfo.suffix()))
        current_manager = image_manager;
    else if(video_manager->hasExtension(fileInfo.suffix()))
        current_manager = video_manager;
    else {
        clearItem();
        return;
    }
    current_manager->load(file);
}

void MainWindow::volumeChange(int change)
{
    //TODO: volume_dial->pageStep() ???
    qDebug() << "Volume = " << change;
    //if(volume_dial->value()!=change)
        //volume_dial->setValue(change); //TODO: This is supposed to sync up key shortcuts & the dial

    current_manager->setVolume(change);
}

void MainWindow::resizeEvent(QResizeEvent * qre)
{
    qDebug() << "resizing";
    if(current_manager!= NULL)
        current_manager->resize(qre);

    qre->accept();
}

void MainWindow::dropEvent(QDropEvent *de)
{
    qDebug() << "dropping";
    if(de->mimeData()->hasUrls())
    {
        QString path = de->mimeData()->urls().first().toString().remove(0, QString("file:///").length());
        QFileInfo pathInfo = QFileInfo(path);
        qDebug() << "dropped: " << path;
        if(pathInfo.isFile()) {
            if(image_manager->hasExtension(pathInfo.suffix()))
                image_manager->checkExtension(pathInfo.suffix());
            else if(video_manager->hasExtension(pathInfo.suffix()))
                video_manager->checkExtension(pathInfo.suffix());
            else {
                qDebug() << "ERROR Invalid filetype: " << pathInfo.suffix();
                return;
            }
        }
        if(pathInfo.isDir())
            loadFolder(path);
        else
            loadFolder(pathInfo.dir().absolutePath(), pathInfo.fileName());
    }
}
