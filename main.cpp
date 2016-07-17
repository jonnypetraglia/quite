#include "project.h"
#include "main_window.h"

#include <QDebug>

#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QtAVWidgets>

class QuiteApp : public QApplication {
public:
    QuiteApp(int &argc, char ** argv) : QApplication(argc, argv) {}
    QString m_macFileOpenOnStart;
    MainWindow *w;

private:
     bool event(QEvent *event)
     {
        switch(event->type())
        {
            case QEvent::FileOpen:
            {
                QFileOpenEvent * fileOpenEvent = static_cast<QFileOpenEvent *>(event);
                if(fileOpenEvent)
                {
                    m_macFileOpenOnStart = fileOpenEvent->file();
                    if(!m_macFileOpenOnStart.isEmpty())
                    {
                        if(w)
                        {
                            QFileInfo fi(m_macFileOpenOnStart);
                            w->loadFolder(fi.dir().absolutePath(), fi.absoluteFilePath());
                        }
                        return true;
                    }
                }
            }
            default:
                return QApplication::event(event);
        }
     }
};


int main(int argc, char ** argv)
{
    QtAV::Widgets::registerRenderers();
    QuiteApp a(argc, argv);

    a.setApplicationName(APP_NAME);
    a.setApplicationDisplayName(APP_NAME);
    a.setOrganizationName(COPYRIGHT_HOLDER);
    a.setOrganizationDomain(COPYRIGHT_DOMAIN);
    a.setApplicationVersion(APP_VERSION);
    a.setWindowIcon(QIcon(":/logo/images/logo_512.png"));

    MainWindow w;
    a.w = &w;

    QString path;
    if(a.m_macFileOpenOnStart!="")
        path = a.m_macFileOpenOnStart;
    else if(argc>1)
        path = QString(argv[1]);
    else {
        QTextStream(stdout) << "Usage: "
                  << QFileInfo( QCoreApplication::applicationFilePath() ).fileName()
                  << " path_to_dir_or_file"
                  << endl;
    }

    if(path=="")
        path = QSettings().value("defaultPath", "").toString();

    if(path!="") {
        qDebug() << path;

        QFileInfo fileinfo(path);
        if(fileinfo.isDir())
            w.loadFolder(fileinfo.filePath());
        else
            w.loadFolder(fileinfo.absoluteDir().absolutePath(), fileinfo.fileName());
    }
    w.move(QApplication::desktop()->screenGeometry().center() - w.rect().center());
    w.show();
    return a.exec();
}
