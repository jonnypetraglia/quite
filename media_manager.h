#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <QDebug>

#include <QMainWindow>
#include <QStatusBar>
#include <QStringList>
#include <QCheckBox>
#include <QString>
#include <QResizeEvent>


class MainWindow;

class MediaManager : public QObject {
    Q_OBJECT
public:
    ~MediaManager() {
        qDebug() << "Destroying mediaManager";
        /*
        for(int i=0; i<FILETYPES.length(); i++)
            delete filetype_checks[i];
        delete filetype_checks;
        */
        qDebug() << "Destroyed mediaManager";
    }

    virtual void load(QString) = 0;
    virtual void setSpeed(double) = 0;
    virtual double getSpeed() = 0;
    virtual void clear() = 0;
    virtual void setVolume(double) = 0;
    virtual void resize(QResizeEvent*) = 0;


    void addCheckedFiletypesTo(QStringList &filetypes_want) {
        for(int i=0; i<FILETYPES.length(); i++)
            if(filetype_checks[i]->isChecked())
                filetypes_want.append(FILETYPES[i]);
    }

    bool hasExtension(QString ext) {
        return FILETYPES.contains(QString("*.").append(ext));
    }

    void checkExtension(QString ext) {
        int ind = FILETYPES.indexOf(QString("*.").append(ext));
        if(ind==0)
            throw 400; //TODO
        filetype_checks[ind]->setChecked(true);
    }

    void initChecks(QStatusBar* status_bar, const char* filetype_check_slot) {
        filetype_checks = new QCheckBox*[FILETYPES.length()];
        for(int i=0; i<FILETYPES.length(); i++)
        {
            status_bar->addWidget(filetype_checks[i] = new QCheckBox(FILETYPES[i].right(FILETYPES[i].length()-2)));
            filetype_checks[i]->setChecked(true);
            filetype_checks[i]->setFocusPolicy(Qt::NoFocus);
            QMainWindow::connect(filetype_checks[i], SIGNAL(clicked()), (QMainWindow*)main_window, filetype_check_slot);
        }
    }

protected:
    QStringList FILETYPES;
    QCheckBox **filetype_checks;
    MainWindow *main_window;
};

#endif // MEDIA_MANAGER_H
