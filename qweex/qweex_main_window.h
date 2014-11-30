#ifndef QWEEX_MAIN_WINDOW_H
#define QWEEX_MAIN_WINDOW_H

#include "qweex/about_window.h"

#ifdef SUPPORT_THE_DEV
#include "qweex/purchase/purchase.h"
#endif

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QDesktopServices>
#include <QMenuBar>

namespace Qweex {

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    #ifdef SUPPORT_THE_DEV
    void buy()
    {
        QDesktopServices::openUrl(QUrl(Qweex::Purchase::BUY_URL));
    }
    void licenseKey()
    {
        Qweex::Purchase::showLicenseDialog(this);
    }
    void haveReceivedLicenseConfirmation();
    #endif

private:
    AboutWindow* about_window;
};

}
#endif
