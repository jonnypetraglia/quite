#ifndef QWEEX_MAIN_WINDOW_H
#define QWEEX_MAIN_WINDOW_H

#ifdef SUPPORT_THE_DEVS
#include "Purchase/Purchase.h"
#endif

#include "version_dialog.h"

#include <QMainWindow>
#include <QMenu>
#include <QAction>

class QweexMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QweexMainWindow(QWidget *parent = 0);

signals:

public slots:
    void showAbout();
    void showLicense();
    void haveReceivedLicenseConfirmation();

private:
    VersionDialog* version_dialog;
};

#endif // QWEEX_MAIN_WINDOW_H
