#include "qweex_main_window.h"

QweexMainWindow::QweexMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    #ifdef __APPLE__
    setWindowIcon(QIcon());
    {
        QMenu* Window = new QMenu("Window");
        {
            QAction* minimize = new QAction(tr("Minimize"),Window);
            minimize->setShortcut(tr("Ctrl+M"));
            connect(minimize,SIGNAL(triggered()),this,SLOT(showMinimized());
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
            connect(about,SIGNAL(triggered()),this,SLOT(showAbout()));
            Help->addAction(about);
        }
        {
            QAction* license = new QAction(tr("License"), Help);
            connect(license,SIGNAL(triggered()),this,SLOT(showLicense()));
            Help->addAction(license);
        }
        this->menuBar()->addMenu(Help);
    }
    #endif
}



void QweexMainWindow::showAbout()
{
    QString title = tr("About Curmudgeon ");
    title.append(APP_VERSION);
    if (!version_dialog) {
        version_dialog = new version_dialog(this);
        connect(m_versionDialog, SIGNAL(finished(int)), this, SLOT(destroyVersionDialog()));
    }
    m_versionDialog->show();
}

void QweexMainWindow::showLicense()
{
    VersionDialog::License();
}


void QweexMainWindow::haveReceivedLicenseConfirmation()
{
    QAction *toRemove = menuBar()->actions().back();
    this->menuBar()->removeAction(toRemove);

    QMenu* Help = (QMenu*)this->menuBar()->actions().back()->parent();
    QAction* key = new QAction(tr("View License Info"),Help);
    connect(key, SIGNAL(triggered()), this, SLOT(licenseKey()));
    Help->addAction(key);
    qDebug() << "KEYEEEEE: " << Purchase::licenseKey;
}
