/*-----------------------------------------------------------------------------
 This source file is a part of Hopsan

 Copyright (c) 2009 to present year, Hopsan Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 For license details and information about the Hopsan Group see the files
 GPLv3 and HOPSANGROUP in the Hopsan source code root directory

 For author and contributor information see the AUTHORS file
-----------------------------------------------------------------------------*/

//$Id$

#include "LicenseDialog.h"

#include <QVBoxLayout>
#include <QFont>
#include <QCheckBox>
#include <QPushButton>

#include "Utilities/GUIUtilities.h"
#include "HelpDialog.h"
#include "Configuration.h"
#include "DesktopHandler.h"
#include "global.h"

LicenseDialog::LicenseDialog(QWidget *pParent) :
    QDialog(pParent)
{
    setObjectName("LicenseDialog");
    resize(640,480);
    setWindowTitle(tr("Hopsan License"));
    setAttribute(Qt::WA_DeleteOnClose, true);
    setPalette(QPalette(QColor("gray"), QColor("whitesmoke")));

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    QLabel *pLicenseText = new QLabel(this);
    QLabel *pGplV3short = new QLabel(this);
    QLabel *pDepHeader = new QLabel(this);
    QLabel *pDependencies = new QLabel(this);
    QString deps;
    QString gplv3short;
    QTextStream ts(&gplv3short);
    QTextLineStream tls(ts);
    tls << "";
    tls << " Copyright (c) 2009 to present year, Hopsan Group";
    tls << "    This program is free software: you can redistribute it and/or modify";
    tls << "    it under the terms of the GNU General Public License as published by";
    tls << "    the Free Software Foundation, either version 3 of the License, or";
    tls << "    (at your option) any later version.";
    tls << "";
    tls << "    This program is distributed in the hope that it will be useful,";
    tls << "    but WITHOUT ANY WARRANTY; without even the implied warranty of";
    tls << "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the";
    tls << "    GNU General Public License for more details.";
    tls << "";
    tls << "    You should have received a copy of the GNU General Public License";
    tls << "    along with this program.  If not, see <http://www.gnu.org/licenses/>.";
    tls << "";
    tls << " For license details and information about the Hopsan Group see the files";
    tls << " GPLv3 and HOPSANGROUP in the Hopsan source code root directory";
    tls << "";
    tls << " For author and contributor information see the AUTHORS file";

    QFont bigFont = pLicenseText->font();
    bigFont.setPointSizeF(bigFont.pointSizeF()*2.0);

    pLicenseText->setText("This version of Hopsan is released under the GPLv3 license");
    pLicenseText->setFont(bigFont);

    //pLicenseText->setAlignment(Qt::AlignHCenter);

    pGplV3short->setText(gplv3short);

    pDepHeader->setText("Dependencies:");
    pDepHeader->setFont(bigFont);

    deps.append("Hopsan uses third-party dependencies released under various licenses such as:\n");
    deps.append("LGPL v2.1, LGPL v3.0, BSD, MIT, Apache License Version 2.0\n\n");
    deps.append("For details on third-party dependencies and their respective licenses, see the documentation!");
    pDependencies->setText(deps);

    pLayout->addWidget(pLicenseText, 1);
    pLayout->addWidget(pGplV3short, 1);
    pLayout->addWidget(pDepHeader, 1);
    pLayout->addWidget(pDependencies, 1);
    pLayout->addWidget(new QLabel(this), 4);

    QPushButton *pDocsButton = new QPushButton("Show license documentation",  this);
    connect(pDocsButton, SIGNAL(clicked(bool)), this, SLOT(showLicenseDocs()));
    pLayout->addWidget(pDocsButton, 1);

    QCheckBox *pAlwaysShow = new QCheckBox("Always show on startup", this);
    pAlwaysShow->setChecked(gpConfig->getBoolSetting(CFG_SHOWLICENSEONSTARTUP));
    connect(pAlwaysShow, SIGNAL(clicked(bool)), this, SLOT(toggleAlwaysShow(bool)));
    pLayout->addWidget(pAlwaysShow, 1);

    setLayout(pLayout);
}

void LicenseDialog::toggleAlwaysShow(bool tf)
{
    gpConfig->setBoolSetting(CFG_SHOWLICENSEONSTARTUP, tf);
}

void LicenseDialog::showLicenseDocs()
{
    gpHelpDialog->open("page_hopsandependencies.html");
}
