#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDebug>

#include <QSettings>
#include <QApplication>
#include <QDir>

#define SETTINGS_FILE "config.ini"

class Options
{
public:
    static QSettings* settings;

    static void init() {
        settings = new QSettings(QApplication::applicationDirPath().append(QDir::separator()).append(SETTINGS_FILE) , QSettings::IniFormat);
        qDebug() << QApplication::applicationDirPath().append(QDir::separator()).append(SETTINGS_FILE);
    }
};

#endif // OPTIONS_H
