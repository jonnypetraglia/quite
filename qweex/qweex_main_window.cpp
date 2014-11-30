#include "qweex/qweex_main_window.h"

Qweex::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    about_window = new Qweex::AboutWindow(this);
    //connect(about_window, SIGNAL(finished(int)), about_window, SLOT(close()));

    #ifdef SUPPORT_THE_DEV
    Purchase::readLicenseInfo();
    if(Purchase::firstTime && !Purchase::keyIsValid())
        Purchase::beg();
    #endif


    #ifdef __APPLE__
    setWindowIcon(QIcon());
    {
        QMenu* Window = new QMenu("Window");
        {
            QAction* minimize = new QAction(tr("Minimize"),Window);
            minimize->setShortcut(tr("Ctrl+M"));
            connect(minimize,SIGNAL(triggered()),this,SLOT(showMinimized()));
            Window->addAction(minimize);
        }
        {
            QAction* zoom = new QAction(tr("Zoom"), Window);
            //previous->setShortcut(tr("Ctrl+Shift+Tab"));
            //connect(zoom,SIGNAL(triggered()),this,SLOT(nextTab()));
            Window->addAction(zoom);
        }
        this->menuBar()->addMenu(Window);
    }
    {
        QMenu* Help = new QMenu(tr("Help"));
        {
            QAction* about = new QAction(tr("About"), Help);
            connect(about, SIGNAL(triggered()), about_window,SLOT(show()));
            Help->addAction(about);
        }
        {
            QAction* license = new QAction(tr("License"), Help);
            connect(license, SIGNAL(triggered()), about_window, SLOT(showLicense()));
            Help->addAction(license);
        }
        this->menuBar()->addMenu(Help);
    }
    #endif
}

#ifdef SUPPORT_THE_DEV
void Qweex::MainWindow::haveReceivedLicenseConfirmation()
{
    QAction *toRemove = menuBar()->actions().back();
    this->menuBar()->removeAction(toRemove);

    QMenu* Help = (QMenu*)this->menuBar()->actions().back()->parent();
    QAction* key = new QAction(tr("View License Info"),Help);
    connect(key, SIGNAL(triggered()), this, SLOT(licenseKey()));
    Help->addAction(key);
    //TODO
    //qDebug() << "KEYEEEEE: " << Qweex::Purchase::license.key;
}
#endif
