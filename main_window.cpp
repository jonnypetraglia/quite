#include "main_window.h"
#include "quca.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    sort_order(QDir::Name)
{
    qDebug() << "Main Window Started";

    /////////// Setup the layout & such ///////////
    layout()->setMargin(11);
    layout()->setSpacing(6);
    QSizePolicy pol(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pol.setHorizontalStretch(0);
    pol.setVerticalStretch(0);
    setSizePolicy(pol);
    setGeometry(QRect(0, 0, WIDTH, HEIGHT));
    setAcceptDrops(true);

    /////////// Misc variables ///////////
    slide_timer = new QTimer;
    connect(slide_timer, SIGNAL(timeout()), this, SLOT(nextItemWhenConvenient()));
    sorts.append(qMakePair(tr("Name"),   QDir::Name));
    sorts.append(qMakePair(tr("Date"),   QDir::Time));
    sorts.append(qMakePair(tr("Size"),   QDir::Size));
    sorts.append(qMakePair(tr("Type"),   QDir::Type));
    sorts.append(qMakePair(tr("Random"), QDir::Unsorted));

    /////////// Create the managers ///////////
    managers.append(new ImageManager(this));
    managers.append(new VideoManager(this));

    /////////// Setup the ToolBar ///////////
    QWidget* SEPARATOR_SENTINEL = new QWidget;

    // Init the toolbar
    QToolBar* tool_bar = new QToolBar(tr("&Toolbar"), this);
    this->addToolBar(Qt::BottomToolBarArea, tool_bar); //:OPTIONS
    tool_bar->setFloatable(false); //:OPTIONS
    tool_bar->setMovable(false); //:OPTIONS
    tool_bar->setVisible(true); //:OPTIONS

    // Sort Order
    QComboBox* sort_select = new QComboBox;
    for(int i=0; i<sorts.length(); i++) {
        sort_select->addItem(sorts.at(i).first);
        if(sorts.at(i).second==sort_order)
            sort_select->setCurrentIndex(i);
    }
    connect(sort_select, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSort(int)));
    tb_widgets.append(sort_select);

    // Reverse
    reverse_button = new QPushButton;
    reverse_button->setCheckable(true);
    reverse_button->setChecked(false); //:OPTIONS?
    reverse_button->setText("Я");
    reverse_button->setToolTip(tr("Reverse Sort"));
    connect(reverse_button, SIGNAL(clicked()), this, SLOT(reloadFolder()));
    tb_widgets.append(reverse_button);

    // Recurse
    recurse_button = new QPushButton;
    recurse_button->setCheckable(true);
    recurse_button->setChecked(false); //:OPTIONS?
    recurse_button->setText("R");
    recurse_button->setToolTip(tr("Recurse into subdirectories"));
    connect(recurse_button, SIGNAL(clicked()), this, SLOT(reloadFolder()));
    tb_widgets.append(recurse_button);

    // Filetypes
        // Get ALL the filetypes!
    QStringList all_filetypes;
    for(MediaManager* manager :  managers)
        for(QString s : manager->filetypes())
            all_filetypes.append(s);
    all_filetypes.sort(Qt::CaseInsensitive);
        // Build the menu
    filetypes_menu = new QMenu;
    connect(filetypes_menu, SIGNAL(aboutToHide()), this, SLOT(reloadFolder()));
    for(QString s : all_filetypes) {
        QCheckBox *checkBox = new QCheckBox(s, filetypes_menu);
        checkBox->setChecked(true);
        QWidgetAction *checkableAction = new QWidgetAction(filetypes_menu);
        checkableAction->setDefaultWidget(checkBox);
        filetypes_menu->addAction(checkableAction);
    }

        // Build the button
    filetypes = new QPushButton(tr("%n filetype(s)", "", all_filetypes.length()));
    filetypes->setMenu(filetypes_menu);
    tb_widgets.append(filetypes);

    //
    tb_widgets.append(SEPARATOR_SENTINEL);
    //

    // Slideshow button
    slideshow_button = new QPushButton(tr("Slide"));
    slideshow_button->setFocusPolicy(Qt::NoFocus);
    connect(slideshow_button, SIGNAL(clicked()), this, SLOT(slideshowButton()));
    tb_widgets.append(slideshow_button);

    // Slideshow time
    slideshow_time = new QSpinBox();
    slideshow_time->setValue(3);
    slideshow_time->setSuffix("s");
    slideshow_time->setRange(1, 60);
    slideshow_time->setFocusPolicy(Qt::NoFocus);
    connect(slideshow_time, SIGNAL(valueChanged(int)), this, SLOT(restartSlideshow(int)));
    tb_widgets.append(slideshow_time);

    // Volume
    volume_dial = new QDial();
    volume_dial->setFocusPolicy(Qt::NoFocus);
    volume_dial->setMinimum(0);
    volume_dial->setMaximum(100);
    volume_dial->setNotchesVisible(true);
    volume_dial->setSingleStep(5);
    volume_dial->setBaseSize(20, 20);
    volume_dial->setMaximumSize(QSize(30,30));
    volume_dial->setWrapping(false); //WTF does this do?
    connect(volume_dial, SIGNAL(valueChanged(int)), this, SLOT(volumeChange(int)));
    tb_widgets.append(volume_dial);


    /////////// Setup the StatusBar ///////////

    // Video Length
    sb_length = new QLabel();
    statusBar()->addWidget(sb_length);

    // Speed label
    sb_speed = new QLabel();
    statusBar()->addWidget(sb_speed);

    // Text
    sb_text = new QLabel();
    sb_text->setAlignment(Qt::AlignRight);
    statusBar()->addWidget(sb_text, 1);
    statusBar()->setVisible(true); //:OPTIONS


    /////////// Setup the Menu ///////////
    QMenu* browse_menu = new QMenu(tr("&Browse"));
    QAction *file = browse_menu->addAction(tr("F&ile")),
             *folder = browse_menu->addAction(tr("F&older"));
    connect(file, SIGNAL(triggered()), this, SLOT(browseForFile()));
    connect(folder, SIGNAL(triggered()), this, SLOT(browseForFolder()));
    menuBar()->addMenu(browse_menu);

    QMenu* view_menu = new QMenu(tr("&View"));
    view_menu->addActions(QList<QAction*>() << tool_bar->toggleViewAction());
    toggle_status = view_menu->addAction(tr("&Status Bar"));
    connect(toggle_status, SIGNAL(triggered()), this, SLOT(toggleStatusBar()));
    toggle_status->setCheckable(true);
    toggle_status->setChecked(!statusBar()->isHidden());
    menuBar()->addMenu(view_menu);


    /////////// Final Setup ///////////

    // Disable all the widgets until a folder is loaded
    for(QWidget* widget : tb_widgets) {
        widget->setDisabled(true);
        if(widget==SEPARATOR_SENTINEL)
            tool_bar->addSeparator();
        else
            tool_bar->addWidget(widget);
    }

    QLabel* dapper_image = new QLabel;
    dapper_image->setStyleSheet("background-color: #333333");
    dapper_image->setBackgroundRole(QPalette::Base);
    dapper_image->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    dapper_image->setPixmap(QPixmap(":logo/images/logo_white.png"));
    dapper_image->setAlignment(Qt::AlignCenter);

    // QStackedLayout
    widget_stack = new QStackedWidget;
    widget_stack->addWidget(dapper_image);
    for(MediaManager* manager : managers) {
        widget_stack->addWidget(manager->widget());
    }
    widget_stack->setCurrentWidget(dapper_image);
    setCentralWidget(widget_stack);


    Qweex::MainWindow::doPlatformSpecific();
    Qweex::MainWindow::addMenus();
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
        slideshow_button->setText(tr("Slide"));
    } else {
        startSlideshow();
        slideshow_button->setText(tr("Stop"));
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
void MainWindow::loadFolder(QString folder, QFileInfo file)
{
    QStringList filetypes_want;
    bool filetypes_want_includes_file = false;

    for(QAction* action : filetypes_menu->actions())
    {
        QCheckBox* checkbox = (QCheckBox*) ((QWidgetAction*)action)->defaultWidget();
        if(checkbox->isChecked()) {
            filetypes_want.append(QString("*.").append(checkbox->text()));
            if(checkbox->text().compare(file.suffix(), Qt::CaseInsensitive)==0)
                filetypes_want_includes_file = true;
        }
    }

    qDebug() << "Loading Folder " << folder << " with " << filetypes_want.join(", ");

    // If there are no checked filetypes
    //if(filetypes_want.size()==0) {
    //    clearItem(); return; }

    filetypes->setText(tr("%n filetype(s)", "", filetypes_want.length()));


    // Check to see if the file we want is the same as is already loaded to avoid reloading it.
    bool sameFile = true; //Guilty until proven innocent
    {
        //Same folder
        sameFile &= this->folder==folder;
        // Same file
        sameFile &= list_index>0 && list.length()>list_index && file==list.at(list_index);
        // Extension checkbox for this file is checked
        sameFile &= filetypes_want_includes_file;
    }

    // Set the folder & load the files
    this->folder = folder;
    list.clear();
    QDirIterator it(folder, filetypes_want, QDir::Files,
                    recurse_button->isChecked() ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags);
    while(it.hasNext()) {
        it.next();
        list.append(it.fileInfo());
    }

    switch(sort_order) {
        case QDir::Name:
            qSort(list.begin(), list.end(), sortByName); break;
            //TODO: QUnicodeCollationAlgorithm::collatorKeySort
        case QDir::Time:
            qSort(list.begin(), list.end(), sortByDate); break;
        case QDir::Type:
            qSort(list.begin(), list.end(), sortByType); break;
        case QDir::Size:
            qSort(list.begin(), list.end(), sortBySize); break;
        case QDir::Unsorted:
            std::random_shuffle(list.begin(), list.end()); break;
    }



    // If there are no files
    if(list.length()==0) {
        clearItem(); return; }

    qDebug() << "Files: " << list.size();

    // Post-Sort (if necessary)
    /*
    switch(sort_order) {
        case QDir::Name:
            QUnicodeCollationAlgorithm::collatorKeySort(list, reverse_button->isChecked());
            break;
        case QDir::Unsorted: //Random
            std::random_shuffle(list.begin(), list.end());
            break;
        default:
            break;
    }
    */

    // Set the current file's index in the list
    if(file==QFileInfo() || (list_index = list.indexOf(file,0)) < 0) {
        list_index = 0;
        showError("File was not found in folder list", file.absoluteFilePath());
    }

    // Determine if this was a reload, in which case we should not reload the current item
    // Note: we still have to call 'indexOf' up there cause even if it is the same file, the order of the files
    //       may have changed, causing list_index to be incorrect.
    if(sameFile) {
        qDebug() << "Reloading same file, so not re-starting it";
        return;
    }

    for(QWidget* widget : tb_widgets)
        widget->setDisabled(false);

    loadItem();
}

bool sortByName(const QFileInfo &s1, const QFileInfo &s2) { return s1.fileName() < s2.fileName(); }
bool sortByDate(const QFileInfo &s1, const QFileInfo &s2) { return s1.lastModified() > s2.lastModified(); }
bool sortBySize(const QFileInfo &s1, const QFileInfo &s2) { return s1.size() < s2.size(); }
bool sortByType(const QFileInfo &s1, const QFileInfo &s2) {
    return s1.fileName().right(s1.fileName().length()-s1.baseName().length())
            <
           s2.fileName().right(s2.fileName().length()-s2.baseName().length())
            ;
}

void MainWindow::browseForFile()
{
    QString file = QFileDialog::getOpenFileName(this); //TODO filetype filtering
    if(file.length()==0)
        return;
    QFileInfo fileinfo(file);
    if(fileinfo.isDir())
        loadFolder(fileinfo.filePath());
    else
        loadFolder(fileinfo.absoluteDir().absolutePath(), fileinfo.absoluteFilePath());
}

void MainWindow::browseForFolder()
{
    QString newFolder = QFileDialog::getExistingDirectory(this);
    if(newFolder.length()>0)
        loadFolder(newFolder);
}


void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(folder=="")
        return;
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
    if(folder=="")
        return;
    e->accept();
    bool shift = e->modifiers().testFlag(Qt::ShiftModifier);
    int change;

    switch (e->key()) {
        case Qt::Key_Right:
            nextItem();
            break;
        case Qt::Key_Left:
            prevItem();
            break;
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            current_manager->faster();
            if(!shift)
                current_manager->faster();
            break;
        case Qt::Key_Minus:
        case Qt::Key_Underscore:
            current_manager->slower();
            if(!shift)
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
        case Qt::Key_F:
            if(isFullScreen())
                showNormal();
            else
                showFullScreen();
            return;
        case Qt::Key_Up:
        case Qt::Key_Down:
            change = shift ? 1 : 5;
            if(e->key()==Qt::Key_Down)
                change *= -1;
            volumeChange(volume_dial->value() + change);
            return;
        case Qt::Key_Greater:
        case Qt::Key_Period:
            if(current_manager->hasSeek()) {
                current_manager->forward();
                if(!shift)
                    current_manager->forward();
            }
            return;
        case Qt::Key_Less:
        case Qt::Key_Comma:
            if(current_manager->hasSeek()) {
                current_manager->back();
                if(!shift)
                    current_manager->back();
            }
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
    sb_speed->setText(QString("%1x").arg(d/100.0));
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
    if(current_manager!=NULL)
        current_manager->unload();
    list_index = 0;
    this->setWindowTitle(QString("<").append(tr("None")).append(">"));
    sb_text->setText(QString("<").append(tr("Folder empty")).append(">"));
    sb_speed->setText("");
}

void MainWindow::unloadItem()
{
    if(current_manager==NULL || list_index>=list.length())
        return;

    qDebug() << "Unloading File " << list.at(list_index).filePath();

    current_manager->unload();
}

// Load the file that is marked as the current file
void MainWindow::loadItem()
{
    unloadItem();

    qDebug() << "Loading Item at " << list_index;
    QString file = folder + "/" + list.at(list_index).fileName();
    QFileInfo fileInfo = QFileInfo(file);
    this->setWindowTitle(fileInfo.fileName());
    sb_text->setText(list.at(list_index).filePath());
    sb_speed->setText("");
    sb_length->setText("");

    qDebug() << "Loading File " << file;

    goto_next_slide = false;
    current_manager = NULL;
    for(MediaManager* manager : managers) {
        if(manager->filetypes().contains(fileInfo.suffix(), Qt::CaseInsensitive)) {
            current_manager = manager;
            widget_stack->setCurrentWidget(current_manager->widget());
            if(current_manager->hasVolume()) {
                qDebug() << "Manager has volume";
//                volume_dial->setEnabled(true);
//                volume_dial->setValue(current_manager->getVolume());
            } //else
                //volume_dial->setEnabled(false);

            current_manager->load(file, this, SLOT(started()));
            return;
        }
    }
    // No manager was found for this filetype
    showError(tr("Filetype not supported"), fileInfo.suffix());
    clearItem();
}

void MainWindow::volumeChange(int newVol)
{
    if(current_manager==NULL || newVol<0 || newVol>100)
        return;

    if(volume_dial->value()!=newVol)
        volume_dial->setValue(newVol);

    if(current_manager->hasVolume())
        current_manager->setVolume(newVol);
}

void MainWindow::resizeEvent(QResizeEvent * qre)
{
    if(current_manager!= NULL)
        current_manager->resize(qre);

    qre->accept();
    qDebug() << "resizing";
}

#ifdef Q_OS_MAC
#include "platform/mac.h"
#endif

void MainWindow::dropEvent(QDropEvent *de)
{
    qDebug() << "dropping";
    if(de->mimeData()->hasUrls())
    {
        QUrl fileUrl(de->mimeData()->urls().first());

        qDebug() << fileUrl;

        #ifdef Q_OS_MAC
            // Mac gives this retarded format back:
            //      file:///file/id=6571367.8312154
            // So we have to call some native Obj-C code to convert to a usable string.
        if(fileUrl.toString().startsWith("file:///file/")) {
            const char* derp = Platform::fileIdToPath(fileUrl.toString().toStdString().c_str());
            qDebug() << derp;
            fileUrl = QUrl(QString::fromUtf8(derp));
        } else if(fileUrl.toString().startsWith("file:///"))
            fileUrl = QUrl(fileUrl.toString().mid(QString("file://").length()));
        #endif


        if(fileUrl.scheme()=="file")
            fileUrl.setScheme("");
        QString path = fileUrl.toString();
        #ifdef Q_OS_WIN
            // For some brilliant reason, Qt also starts paths on Windows with a leading '/'
            path.remove(0, 1);
        #endif

        qDebug() << fileUrl << path;
;
        QFileInfo file_info(path);
        if(file_info.isFile()) {
            for(QAction* action : filetypes_menu->actions()) {
                QCheckBox* checkbox = (QCheckBox*) ((QWidgetAction*)action)->defaultWidget();
                if(checkbox->text().compare(file_info.suffix(), Qt::CaseInsensitive)==0) {
                    checkbox->setChecked(true);
                    break;
                }
            }
            loadFolder(file_info.dir().absolutePath(), file_info.absoluteFilePath());
            de->accept();
        } else if(file_info.isDir()) {
            loadFolder(path);
            de->accept();
        } else
            showError(tr("Not a file or folder"), path);
    }
}


void MainWindow::showError(QString str, QString cause) {
    //TODO: Eventually make this graphical
    qDebug() << QString(str.append(": ").append(cause));
}
