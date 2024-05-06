/** \file AboutBox.cpp
\brief Implementation file for AboutBox.hpp.
*/

#include "AboutBox.hpp"

AboutBox::
AboutBox(QString& version, QWidget *parent) :
  QDialog(parent)
{
  {
    QString copyRightSign = QChar(0x0A9);

    //
    // Change the following strings to modify the texts shown
    // in the about box.
    //

    QString software_version = version;

    QString copyRightText = "<a href=\"https://www.cttc.cat/geomatics-gm/\">" + copyRightSign +
                            " 2023 Geomatics Research Unit, CTTC\nAll rights reserved</a>";

    // Interface set up.

    QHBoxLayout* mainLayout = new QHBoxLayout;

    QVBoxLayout* logoLayout = new QVBoxLayout;

    QLabel* iconLabel = new QLabel();
    QPixmap* pixmap = new QPixmap(":/resources/WFMEditor_about_logo.png");
    iconLabel->setPixmap(*pixmap);
    iconLabel->setMask(pixmap->mask());
    logoLayout->addWidget(iconLabel);
    logoLayout->addStretch();

    QLabel* blankLabel = new QLabel("  ");

    QVBoxLayout* textLayout = new QVBoxLayout;

    QLabel* companyLogoLabel = new QLabel();
    QPixmap* companyPixmap = new QPixmap(":/resources/CTTC_logo.png");
    companyLogoLabel->setPixmap(*companyPixmap);
    companyLogoLabel->setMask(companyPixmap->mask());
    textLayout->addWidget(companyLogoLabel);

    QFrame* line1 = new QFrame();
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    textLayout->addWidget(line1);

    QFont font;
    font.setPointSize(16);
    QLabel* titleText = new QLabel("WorkflowEditor");
    titleText->setFont(font);
    textLayout->addWidget(titleText);

    QLabel* versionLabel_ = new QLabel(software_version);
    textLayout->addWidget(versionLabel_);

    QLabel* copyRightLabel = new QLabel(copyRightText);
    copyRightLabel->setOpenExternalLinks(true);
    textLayout->addWidget(copyRightLabel);

    QLabel* vSpaceLabel = new QLabel(" ");
    textLayout->addWidget(vSpaceLabel);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;

    QPushButton* closeButton = new QPushButton(tr("Close"));
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(closeButton);
    textLayout->addLayout(buttonsLayout);

    mainLayout->addLayout(logoLayout);
    mainLayout->addWidget(blankLabel);
    mainLayout->addLayout(textLayout);

    setLayout(mainLayout);

    // Window flags and title.

    setWindowFlags(Qt::CustomizeWindowHint| Qt::WindowTitleHint);

    setWindowTitle(tr("About CTTC's WorkflowEditor"));

    // Connections

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
  }
}
