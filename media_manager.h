#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <QDebug>

#include <QStatusBar>
#include <QStringList>
#include <QCheckBox>
#include <QString>
#include <QResizeEvent>
#include <QVector>


class MainWindow;

class MediaManager : public QObject {
    Q_OBJECT
public:
    virtual void load(QString) = 0;
    virtual void faster() = 0;
    virtual void slower() = 0;
    virtual void setSpeed(double) = 0;
    virtual double getSpeed() = 0;
    virtual bool togglePause() = 0;
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

    QVector<QCheckBox*> getChecks() {
        return filetype_checks;
    }

protected:
    void initChecks() {
        filetype_checks.reserve(FILETYPES.length());
        for(QString filetype : FILETYPES)
        {
            QCheckBox* c = new QCheckBox(filetype.right(filetype.length()-2));
            c->setChecked(true);
            c->setFocusPolicy(Qt::NoFocus);
            filetype_checks.append(c);
        }
    }

    QStringList FILETYPES;
    QVector<QCheckBox*> filetype_checks;
    MainWindow *main_window;
};

#endif // MEDIA_MANAGER_H
