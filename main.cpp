#include "mainwindow.h"
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QVideoWidget>
#include <QMediaPlayer>

#include <iostream>

/*TODO:
 * -empty folder crashes
 * -order by: name, type, shuffle
 */

int main(int argc, char *argv[])
{
    if(argc<2)
    {
        std::cout << "Usage: " << argv[0] << " (directory or file path)" << std::endl;
        return 0;
    }

    QString path;
    path = QString(argv[1]);

    QFileInfo fileinfo(path);
    if(!fileinfo.exists())
    {
        std::cerr << "File does not exist: " << path.toStdString() << std::endl;
        return 1;
    }

    QApplication a(argc, argv);
    MainWindow w;
    if(fileinfo.isDir())
        w.loadFolder(fileinfo.filePath());
    else
        w.loadFolder(fileinfo.absoluteDir().absolutePath(), fileinfo.fileName());
    w.show();
    return a.exec();
}
