#include "main_window.h"
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

    // Setup the managers
    image_manager = new ImageManager(this);
    video_manager = new VideoManager(this);

    // Setup the Menu
    //this->menuBar()->setGeometry(0, 0, WIDTH, 22);
    QMenu* browse_menu = menuBar()->addMenu(tr("&Browse"));
    QAction *file = browse_menu->addAction(tr("F&ile")),
             *folder = browse_menu->addAction(tr("F&older"));
    connect(file, SIGNAL(triggered()), this, SLOT(browseForFile()));
    connect(folder, SIGNAL(triggered()), this, SLOT(browseForFolder()));
    //*/

    // Setup the StatusBar
    for(QCheckBox* check : image_manager->getChecks()) {
        QMainWindow::connect(check, SIGNAL(clicked()), this, SLOT(reloadFolder()));
        statusBar()->addWidget(check);
        status_bar_widgets.append(check);
    }
    for(QCheckBox* check : video_manager->getChecks()) {
        QMainWindow::connect(check, SIGNAL(clicked()), this, SLOT(reloadFolder()));
        statusBar()->addWidget(check);
        status_bar_widgets.append(check);
    }

    status_bar_speed = new QLabel();
    statusBar()->addWidget(status_bar_speed);
    status_bar_widgets.append(status_bar_speed);

    slideshow_button = new QPushButton(tr("Slide"));
    slideshow_button->setFocusPolicy(Qt::NoFocus);
    statusBar()->addWidget(slideshow_button);
    status_bar_widgets.append(slideshow_button);

    slideshow_time = new QSpinBox();
    slideshow_time->setValue(3);
    slideshow_time->setSuffix("s");
    slideshow_time->setRange(1, 60);
    slideshow_time->setFocusPolicy(Qt::NoFocus);
    statusBar()->addWidget(slideshow_time);
    status_bar_widgets.append(slideshow_time);

    random_order = new QCheckBox(tr("Random"));
    statusBar()->addWidget(random_order);
    status_bar_widgets.append(random_order);

    volume_dial = new QDial();
    volume_dial->setFocusPolicy(Qt::NoFocus);
    volume_dial->setMinimum(0);
    volume_dial->setMaximum(100);
    volume_dial->setNotchesVisible(true);
    volume_dial->setSingleStep(5);
    volume_dial->setBaseSize(20, 20);
    volume_dial->setMaximumSize(QSize(30,30));
    volume_dial->setWrapping(false); //WTF does this do?
    statusBar()->addWidget(volume_dial);
    status_bar_widgets.append(volume_dial);

    status_bar_text = new QLabel();
    status_bar_text->setAlignment(Qt::AlignRight);
    statusBar()->addWidget(status_bar_text);
    status_bar_widgets.append(status_bar_text);

    connect(random_order, SIGNAL(clicked()), this, SLOT(reloadFolder()));
    connect(slideshow_button, SIGNAL(clicked()), this, SLOT(slideshowButton()));
    connect(slide_timer, SIGNAL(timeout()), this, SLOT(nextItemWhenConvenient()));
    connect(slideshow_time, SIGNAL(valueChanged(int)), this, SLOT(restartSlideshow(int)));
    connect(volume_dial, SIGNAL(valueChanged(int)), this, SLOT(volumeChange(int)));


    for(QWidget* widget : status_bar_widgets)
        widget->setDisabled(true);

    qDebug() << "mainWindow created";
}

// Destructor
MainWindow::~MainWindow()
{
    qDebug() << "Destroying mainWindow";

    delete slide_timer;

    qDebug() << "Destroyed mainWindow";
}


// Start or stop the slideshow
void MainWindow::slideshowButton()
{
    qDebug() << "Slideshow Button";
    if(slide_timer->isActive()) {
        stopSlideshow();
        slideshow_button->setText("Slide");
    } else {
        startSlideshow();
        slideshow_button->setText("Stop");
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

    // Check to see if the file we want is the same as is already loaded to avoid reloading it.
    bool sameFile =
            //Same folder
            this->folder==folder &&
            // Same file
            (
                list_index>0 &&
                list.length()>list_index &&
                file==list.at(list_index)
            ) &&
            // Extension checkbox for this file is checked
            filetypes_want.contains(QString("*.").append(QFileInfo(file).suffix()));

    // Set the folder & load the files
    this->folder = folder;
    list = QDir(folder).entryList(filetypes_want);

    // If there are no files
    if(list.length()==0) {
        clearItem(); return; }

    qDebug() << "Files: " << list.size();

    // Sort (or anti-sort) the list
    if(random_order->isChecked())
        std::random_shuffle(list.begin(), list.end());
    else
        QUnicodeCollationAlgorithm::collatorKeySort(list);

    // Determine if this was a reload, in which case we should not reload the current item
    if(sameFile) {
        qDebug() << "Reloading same file, so not re-starting it";
        return;
    }

    // Set the current file's index in the list
    if(file=="" || list.indexOf(file,0)<0)
        list_index = 0;
    else
        list_index = list.indexOf(file,0);

    for(QWidget* widget : status_bar_widgets)
        widget->setDisabled(false);

    loadItem();
}

void MainWindow::browseForFile()
{
    QString file = QFileDialog::getOpenFileName(this); //TODO filetype filtering
    QFileInfo fileinfo(file);
    if(fileinfo.isDir())
        loadFolder(fileinfo.filePath());
    else
        loadFolder(fileinfo.absoluteDir().absolutePath(), fileinfo.fileName());
}

void MainWindow::browseForFolder()
{
    loadFolder(QFileDialog::getExistingDirectory(this));
}


void MainWindow::wheelEvent(QWheelEvent *event)
{
    qDebug() << "WheelEvent";
    if (event->modifiers().testFlag(Qt::ControlModifier)) {
        //Zoom
    } else if (event->modifiers().testFlag(Qt::AltModifier)) {
        if(event->delta()>0)
            current_manager->faster();
        else
            current_manager->slower();
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
    e->accept();
    switch (e->key()) {
        case Qt::Key_Right:
            nextItem();
            break;
        case Qt::Key_Left:
            prevItem();
            break;
        case Qt::Key_Plus:
            current_manager->faster();
            break;
        case Qt::Key_Minus:
            current_manager->slower();
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
        case Qt::Key_P:
            if(current_manager->togglePause())
                stopSlideshow();
            else if(slide_timer->isActive())
                restartSlideshow();
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
    qDebug() << "Loading Item at " << list_index;
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
