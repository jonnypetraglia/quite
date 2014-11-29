#include <QDebug>

#include "mainwindow.h"
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QVideoWidget>
#include <QMediaPlayer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    qDebug() << "in Main";
    w.show();
    qDebug() << "window shown";
    return a.exec();

    QString path;
    if(argc<2) {
        QTextStream(stdout) << "Usage: "
                  << QFileInfo( QCoreApplication::applicationFilePath() ).fileName()
                  << " path_to_dir_or_file"
                  ; //<< endl;
        w.browseForFolder(NULL);
    }
    else
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
    return a.exec();
}
