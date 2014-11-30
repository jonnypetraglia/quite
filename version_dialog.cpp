/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "version_dialog.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include <iostream>

const QString VersionDialog::BTC = "16Y7xEtFY18YanhzzigGhnnoyNewZEZDBT";
const QString VersionDialog::LTC = "LcQp2ZqDw5ew4wGeauN57i8xME3UKrqore";

VersionDialog::VersionDialog(QWidget *parent) : QDialog(parent)
{
    QString iconpath = ":/logo/images/logo_192.png";

    // We need to set the window icon explicitly here since for some reason the
    // application icon isn't used when the size of the dialog is fixed (at least not on X11/GNOME)
    setWindowIcon(QIcon(iconpath));

    setWindowTitle(versionString);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QGridLayout *layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

     const QString description = tr(
        "<br/><h2>%1</h2>"
        "Based on Qt %2 (%3-bit%4)<br/>"
        "<br/>"
        "Built on %5 %6<br />"
        "<br/>"
        "Copyright 2013-%7 %8. All rights reserved.<br/>"
        "<br/>"
        "This program is released under the <a href='http://opensource.org/licenses/QPL-1.0'>Q Public License</a>.")
        .arg(versionString,
             QLatin1String(qVersion()),
             QString::number(QSysInfo::WordSize),
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
             ),
        #elif defined(__GNUC__)
            #ifdef _WIN32
             QString(" MingW ").append(
            #else
             QString(" GCC ").append(
            #endif
             QString::number(__GNUC__).append(".").append(QString::number(__GNUC_MINOR__)).append(".").append(QString::number(__GNUC_PATCHLEVEL__))),
        #elif defined(__clang__)
             QString(" Clang ").append(
                 QString::number(__clang_major__).append(".").append(QString::number(__clang_minor__)).append(".").append(QString::number(__clang_patchlevel__))
                 ),
        #else
             "",
        #endif
             QLatin1String(__DATE__), QLatin1String(__TIME__),
             copyrightYear,
             copyrightHolder);

    QLabel *copyRightLabel = new QLabel(description);
    copyRightLabel->setWordWrap(true);
    copyRightLabel->setOpenExternalLinks(true);
    copyRightLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    copyRightLabel->setAlignment(Qt::AlignTop);

    QGridLayout *crypto = new QGridLayout();
    //BTC
    QLabel* btcL = new QLabel("BTC");
    QFont btcLF = btcL->font();
    btcLF.setBold(true);
    btcL->setFont(btcLF);
    QLineEdit* btc = new QLineEdit();
    btc->setText(VersionDialog::BTC);
    btc->setReadOnly(true);
    QPushButton* btcC = new QPushButton();
    btcC->setIcon(QIcon(":/res/images/clipboard.png"));

    //LTC
    QLabel* ltcL = new QLabel("LTC");
    QFont ltcLF = btcL->font();
    ltcLF.setBold(true);
    ltcL->setFont(ltcLF);
    QLineEdit* ltc = new QLineEdit();
    ltc->setText(VersionDialog::LTC);
    ltc->setReadOnly(true);
    QPushButton* ltcC = new QPushButton();
    ltcC->setIcon(QIcon(":/res/images/clipboard.png"));

    btcL->setIndent(100);
    ltcL->setIndent(100);
    crypto->addWidget(btcL,0, 0);
    crypto->addWidget(btc, 0, 1);
    crypto->addWidget(btcC,0, 2);
    crypto->addWidget(ltcL,1, 0);
    crypto->addWidget(ltc, 1, 1);
    crypto->addWidget(ltcC,1, 2);

    connect(btcC, SIGNAL(clicked()), this, SLOT(copyBTC()));
    connect(ltcC, SIGNAL(clicked()), this, SLOT(copyLTC()));

    QLabel *logoLabel = new QLabel;
    logoLabel->setPixmap(QPixmap(iconpath));
    layout->addWidget(logoLabel ,       0, 0, 1, 1);
    layout->addWidget(copyRightLabel,   0, 1, 4, 6);
    layout->addLayout(crypto,           1, 0, 1, 6);
}

#include <QTextEdit>
#include <QFile>
#include <QVBoxLayout>
#include <QFontMetrics>

void VersionDialog::License()
{
    QDialog* license = new QDialog();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSizeConstraint(QLayout::SetFixedSize);

    //Title
    QLabel* title = new QLabel("Q Public License 1.0", license);
    QFont font = title->font();
    font.setPointSize(20);
    font.setBold(true);
    title->setFont(font);

    //Derp
    QTextEdit* text = new QTextEdit();
    QFile data(":/res/LICENSE.QPL");
    data.open(QIODevice::ReadOnly);
    text->setText(data.readAll());
    QFontMetrics qfm(text->font());
    text->setMinimumSize(
                qfm.width("     b. You must ensure that all recipients of the machine-executable forms")+50,
                qfm.height()*30);
    text->setReadOnly(true);

    //Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    QPushButton *closeButton = buttonBox->button(QDialogButtonBox::Close);
    buttonBox->addButton(closeButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::RejectRole | QDialogButtonBox::AcceptRole));
    connect(buttonBox , SIGNAL(rejected()), license, SLOT(reject()));

    layout->addWidget(title);
    layout->addWidget(text, 4);
    layout->addWidget(buttonBox, 0, Qt::AlignRight);

    license->setLayout(layout);
    license->show();
}
