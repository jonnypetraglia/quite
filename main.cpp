#include "project.h"
#include "main_window.h"

#include <QDebug>

#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QVideoWidget>
#include <QMediaPlayer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName(APP_NAME);
    a.setApplicationDisplayName(APP_NAME);
    a.setOrganizationName(COPYRIGHT_HOLDER);
    a.setOrganizationDomain(COPYRIGHT_DOMAIN);

    a.setWindowIcon(QIcon(":/logo/images/logo_512.png"));

    MainWindow w;

    QString path;
    if(argc<2) {
        QTextStream(stdout) << "Usage: "
                  << QFileInfo( QCoreApplication::applicationFilePath() ).fileName()
                  << " path_to_dir_or_file"
                  << endl;
    }
    else {
        path = QString(argv[1]);
        QFileInfo fileinfo(path);
        if(!fileinfo.exists())
        {
            qDebug() << "File does not exist: " << path;
            return 1;
        }

        qDebug() << path;
        if(fileinfo.isDir())
            w.loadFolder(fileinfo.filePath());
        else
            w.loadFolder(fileinfo.absoluteDir().absolutePath(), fileinfo.fileName());
    }
    w.show();
    return a.exec();
}
