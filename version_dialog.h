#ifndef VERSIONDIALOG_H
#define VERSIONDIALOG_H

#include <QDialog>
#include <QString>
#include <QClipboard>
#include <QApplication>

static const QString versionString = "Quite 0.1.0",
        buildDate = "November 29, 2014",
        copyrightYear = "2014",
        copyrightHolder = "Qweex";

class VersionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VersionDialog(QWidget *parent);
    static void License();

private:
    static const QString BTC;
    static const QString LTC;

public slots:
    void copyBTC() { QApplication::clipboard()->setText(BTC); }

    void copyLTC() { QApplication::clipboard()->setText(LTC); }
};


#endif // VERSIONDIALOG_H
