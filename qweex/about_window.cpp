#include "qweex/about_window.h"

Qweex::AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent)
{
    //connect(about_window, SIGNAL(finished(int)), about_window, SLOT(close()));

    QString iconpath = ":/logo/images/logo_192.png";
    QString license_href = QString("<a href='").append(LICENSE_URL).append("'>").append(LICENSE_NAME).append("</a>");

    // We need to set the window icon explicitly here since for some reason the
    // application icon isn't used when the size of the dialog is fixed (at least not on X11/GNOME)
    setWindowIcon(QIcon(iconpath));

    setWindowTitle(tr("About %1").arg(APP_NAME));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QGridLayout *layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    QString description;
    description.append("<h2><br/>").append(APP_NAME).append(" ").append(APP_VERSION).append("</h2>").append("\n")
               .append("<p>").append(tr("Based on Qt")).append(" ").append(qVersion())
                .append("(").append(QString::number(QSysInfo::WordSize)).append("-bit").append(buildPlatform()).append(")")
               .append("</p><p>").append("\n")
               .append(tr("Built on")).append(" ").append(__DATE__).append(" ").append(QLatin1String(__TIME__))
               .append("</p><p>").append("\n")
               .append(tr("Copyright")).append(" ").append(COPYRIGHT_STARTYEAR)
            #ifdef COPYRIGHT_ENDYEAR
               .append("-").append(COPYRIGHT_ENDYEAR)
            #endif
               .append(" ").append(COPYRIGHT_HOLDER).append(". ")
               .append(tr("All rights reserved")).append(".")
                .append("</p><p>").append("\n")
               .append(
                    tr("This program is released under the %1").arg(license_href).append(".")
                )
               .append("</p>");

    QLabel *copyRightLabel = new QLabel(description);
    copyRightLabel->setWordWrap(true);
    copyRightLabel->setOpenExternalLinks(false); //true);
    copyRightLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    copyRightLabel->setAlignment(Qt::AlignTop);
    connect(copyRightLabel, SIGNAL(linkActivated(QString)), this, SLOT(showLicense()));

    QGridLayout *crypto = new QGridLayout();
#ifdef BTC
    QLabel* btcL = new QLabel("BTC");
    QFont btcLF = btcL->font();
    btcLF.setBold(true);
    btcL->setFont(btcLF);
    QLineEdit* btc = new QLineEdit();
    btc->setText(BTC);
    btc->setReadOnly(true);
    QPushButton* btcC = new QPushButton();
    btcC->setIcon(QIcon(":/qweex/images/clipboard.png"));
    btcL->setIndent(100);
    btcC->setFocusPolicy(Qt::NoFocus);
    crypto->addWidget(btcL,0, 0);
    crypto->addWidget(btc, 0, 1);
    crypto->addWidget(btcC,0, 2);
    connect(btcC, SIGNAL(clicked()), this, SLOT(copyBTC()));
#endif

#ifdef LTC
    QLabel* ltcL = new QLabel("LTC");
    QFont ltcLF = btcL->font();
    ltcLF.setBold(true);
    ltcL->setFont(ltcLF);
    QLineEdit* ltc = new QLineEdit();
    ltc->setText(LTC);
    ltc->setReadOnly(true);
    QPushButton* ltcC = new QPushButton();
    ltcC->setIcon(QIcon(":/qweex/images/clipboard.png"));
    ltcL->setIndent(100);
    ltcC->setFocusPolicy(Qt::NoFocus);
    crypto->addWidget(ltcL,1, 0);
    crypto->addWidget(ltc, 1, 1);
    crypto->addWidget(ltcC,1, 2);
    connect(ltcC, SIGNAL(clicked()), this, SLOT(copyLTC()));
#endif

    QLabel *logoLabel = new QLabel;
    logoLabel->setPixmap(QPixmap(iconpath));
    layout->addWidget(logoLabel ,       0, 0, 1, 1);
    layout->addWidget(copyRightLabel,   0, 1, 4, 6);
    if(crypto->rowCount()>0)
        layout->addLayout(crypto,       1, 0, 1, 6);
}


QString Qweex::AboutWindow::buildPlatform()
{
    return
    #ifdef _MSC_VER
         QString(" VS").append(
        #if _MSC_VER < 1400
             ""
          // older than VC++ 2005
        #elif _MSC_VER < 1500
             "2005"
        #elif _MSC_VER < 1600
             "2008"
        #elif _MSC_VER < 1700
             "2010"
        #else
             "9001"
          // Future versions
        #endif
         )
    #elif defined(__GNUC__)
        #ifdef _WIN32
         QString(" MingW ").append(
        #else
         QString(" GCC ").append(
        #endif
         QString::number(__GNUC__).append(".").append(QString::number(__GNUC_MINOR__)).append(".").append(QString::number(__GNUC_PATCHLEVEL__)))
    #elif defined(__clang__)
         QString(" Clang ").append(
             QString::number(__clang_major__).append(".").append(QString::number(__clang_minor__)).append(".").append(QString::number(__clang_patchlevel__))
             )
    #else
         ""
    #endif
    ;
}

void Qweex::AboutWindow::showLicense()
{
    QDialog* license = new QDialog();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSizeConstraint(QLayout::SetFixedSize);

    // Title
    QLabel* title = new QLabel(QString(LICENSE_NAME)
                           #ifdef LICENSE_VERSION
                               .append(" ").append(LICENSE_VERSION)
                           #endif
                               , license);
    QFont font = title->font();
    font.setPointSize(20);
    font.setBold(true);
    title->setFont(font);
    layout->addWidget(title);


    // License Content
    QTextEdit* text = new QTextEdit();
    text->setReadOnly(true);
    QFile license_file(QString(":/").append(LICENSE_FILENAME));
    if(license_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString file_contents = license_file.readAll();
        text->setText(file_contents);
        QFontMetrics qfm(text->font());

        QString longest_line;
        QStringList lines = file_contents.split("\n");
        for(QString line : lines)
            if(line.length() > longest_line.length())
                longest_line = line;

        if(longest_line.length() > 80) {
            longest_line = longest_line.left(80);
        }

        qDebug() << longest_line;
        text->setMinimumSize(
                    qfm.width(longest_line)+20,
                    qfm.height()*qMax(20, qMin(lines.length()+1, 30)));
    } else {
        text->setText(tr("Cannot read license file."));
    }
    layout->addWidget(text);

    // Web Link
    #ifdef LICENSE_URL
    QLabel* link = new QLabel();
    link->setText(tr("The license can also be found online at: ")
                .append("<br/><a href='").append(LICENSE_URL).append("'>").append(LICENSE_URL).append("</a>")
                );
    link->setOpenExternalLinks(true);
    layout->addWidget(link);
    #endif

    license->setLayout(layout);
    license->show();
}
