/********************************************************************************
** Form generated from reading UI file 'infobox.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFOBOX_H
#define UI_INFOBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <clickablelabel.h>

QT_BEGIN_NAMESPACE

class Ui_infobox
{
public:
    ClickableLabel *info_screen;
    ClickableLabel *info_1;
    ClickableLabel *info_2;
    ClickableLabel *info_3;

    void setupUi(QDialog *infobox)
    {
        if (infobox->objectName().isEmpty())
            infobox->setObjectName(QString::fromUtf8("infobox"));
        infobox->resize(679, 480);
        infobox->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        info_screen = new ClickableLabel(infobox);
        info_screen->setObjectName(QString::fromUtf8("info_screen"));
        info_screen->setGeometry(QRect(0, 0, 681, 481));
        info_screen->setPixmap(QPixmap(QString::fromUtf8(":/Resources/about.jpg")));
        info_1 = new ClickableLabel(infobox);
        info_1->setObjectName(QString::fromUtf8("info_1"));
        info_1->setGeometry(QRect(28, 210, 631, 71));
        info_1->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"color: rgb(255, 255, 255);font: 12px \"RalewayMedium\";"));
        info_1->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        info_2 = new ClickableLabel(infobox);
        info_2->setObjectName(QString::fromUtf8("info_2"));
        info_2->setGeometry(QRect(28, 280, 631, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("RalewayMedium"));
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(true);
        font.setWeight(50);
        info_2->setFont(font);
        info_2->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"color: rgb(255, 255, 255);font: 12px \"RalewayMedium\";"));
        info_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        info_3 = new ClickableLabel(infobox);
        info_3->setObjectName(QString::fromUtf8("info_3"));
        info_3->setGeometry(QRect(28, 320, 631, 51));
        info_3->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"color: rgb(255, 255, 255);font: 12px \"RalewayMedium\";"));
        info_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        retranslateUi(infobox);

        QMetaObject::connectSlotsByName(infobox);
    } // setupUi

    void retranslateUi(QDialog *infobox)
    {
        infobox->setWindowTitle(QCoreApplication::translate("infobox", "Dialog", nullptr));
        info_screen->setText(QString());
        info_1->setText(QCoreApplication::translate("infobox", "With love by Frank from The VFD Collective.\n"
"Thank you for using FluorescenceApp. We've got you covered. \n"
"If there's anything you want to talk about with us,\n"
"please reach out to our customer service at", nullptr));
        info_2->setText(QCoreApplication::translate("infobox", "www.TheVFDCollective.com/support", nullptr));
        info_3->setText(QCoreApplication::translate("infobox", "PICTURE CREDITS (no copyright infringement intended):\n"
"Computer icon: http://icon-park.com/icon/imac/\n"
"Ambient light pictures: Taken from Instagram by @bryanadamc and Pinterest", nullptr));
    } // retranslateUi

};

namespace Ui {
    class infobox: public Ui_infobox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOBOX_H
