#ifndef QWEEX_ABOUT_WINDOW_H
#define QWEEX_ABOUT_WINDOW_H

#include "project.h"

#include <QDebug>

#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QFile>
#include <QVBoxLayout>
#include <QFontMetrics>
#include <QString>
#include <QClipboard>

namespace Qweex {

class AboutWindow : public QDialog
{
    Q_OBJECT
public:
    explicit AboutWindow(QWidget *parent);

private:
    QString buildPlatform();

public slots:
    void showLicense();
    #ifdef BTC
    void copyBTC() { QApplication::clipboard()->setText(BTC); }
    #endif
    #ifdef LTC
    void copyLTC() { QApplication::clipboard()->setText(LTC); }
    #endif
};

}

#endif
