/********************************************************************************
** Form generated from reading UI file 'fwupdatestm.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FWUPDATESTM_H
#define UI_FWUPDATESTM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FWUpdate
{
public:
    QWidget *welcome;
    QPushButton *welcome_next;
    QLabel *welcome_intro_1;
    QLabel *welcome_pic;
    QLabel *welcome_intro_2;
    QWidget *updater;
    QLabel *updater_intro;
    QLabel *updater_curr;
    QLabel *updater_intro_curr;
    QLabel *updater_new;
    QLabel *updater_intro_new;
    QPushButton *updater_run;
    QCheckBox *updater_agree;
    QLabel *updater_run_text;
    QLabel *updater_agree_text;
    QPlainTextEdit *updater_info_text;
    QPushButton *updater_update;

    void setupUi(QDialog *FWUpdate)
    {
        if (FWUpdate->objectName().isEmpty())
            FWUpdate->setObjectName(QString::fromUtf8("FWUpdate"));
        FWUpdate->resize(801, 370);
        welcome = new QWidget(FWUpdate);
        welcome->setObjectName(QString::fromUtf8("welcome"));
        welcome->setGeometry(QRect(0, 0, 321, 371));
        welcome->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        welcome_next = new QPushButton(welcome);
        welcome_next->setObjectName(QString::fromUtf8("welcome_next"));
        welcome_next->setGeometry(QRect(120, 320, 191, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("RalewayBold"));
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        welcome_next->setFont(font);
        welcome_next->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);font: 12px \"RalewayBold\";"));
        welcome_intro_1 = new QLabel(welcome);
        welcome_intro_1->setObjectName(QString::fromUtf8("welcome_intro_1"));
        welcome_intro_1->setGeometry(QRect(10, 10, 301, 31));
        welcome_intro_1->setFont(font);
        welcome_intro_1->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayBold\";"));
        welcome_pic = new QLabel(welcome);
        welcome_pic->setObjectName(QString::fromUtf8("welcome_pic"));
        welcome_pic->setGeometry(QRect(10, 50, 301, 191));
        welcome_pic->setFrameShape(QFrame::Box);
        welcome_pic->setPixmap(QPixmap(QString::fromUtf8(":/Resources/fl2_usb.jpg")));
        welcome_intro_2 = new QLabel(welcome);
        welcome_intro_2->setObjectName(QString::fromUtf8("welcome_intro_2"));
        welcome_intro_2->setGeometry(QRect(10, 245, 301, 31));
        welcome_intro_2->setFont(font);
        welcome_intro_2->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayBold\";"));
        updater = new QWidget(FWUpdate);
        updater->setObjectName(QString::fromUtf8("updater"));
        updater->setGeometry(QRect(0, 0, 801, 371));
        updater->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        updater_intro = new QLabel(updater);
        updater_intro->setObjectName(QString::fromUtf8("updater_intro"));
        updater_intro->setGeometry(QRect(20, 20, 281, 31));
        updater_intro->setFont(font);
        updater_intro->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 18px \"RalewayBold\";"));
        updater_intro->setAlignment(Qt::AlignCenter);
        updater_curr = new QLabel(updater);
        updater_curr->setObjectName(QString::fromUtf8("updater_curr"));
        updater_curr->setGeometry(QRect(10, 80, 301, 31));
        updater_curr->setFont(font);
        updater_curr->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);font: 26px \"RalewayBold\";"));
        updater_curr->setAlignment(Qt::AlignCenter);
        updater_intro_curr = new QLabel(updater);
        updater_intro_curr->setObjectName(QString::fromUtf8("updater_intro_curr"));
        updater_intro_curr->setGeometry(QRect(10, 110, 301, 31));
        updater_intro_curr->setFont(font);
        updater_intro_curr->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 12px \"RalewayBold\";"));
        updater_intro_curr->setAlignment(Qt::AlignCenter);
        updater_new = new QLabel(updater);
        updater_new->setObjectName(QString::fromUtf8("updater_new"));
        updater_new->setGeometry(QRect(10, 180, 301, 31));
        updater_new->setFont(font);
        updater_new->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);font: 26px \"RalewayBold\";"));
        updater_new->setAlignment(Qt::AlignCenter);
        updater_intro_new = new QLabel(updater);
        updater_intro_new->setObjectName(QString::fromUtf8("updater_intro_new"));
        updater_intro_new->setGeometry(QRect(10, 210, 301, 31));
        updater_intro_new->setFont(font);
        updater_intro_new->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 12px \"RalewayBold\";"));
        updater_intro_new->setAlignment(Qt::AlignCenter);
        updater_run = new QPushButton(updater);
        updater_run->setObjectName(QString::fromUtf8("updater_run"));
        updater_run->setGeometry(QRect(20, 300, 281, 61));
        updater_run->setFont(font);
        updater_run->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));
        updater_agree = new QCheckBox(updater);
        updater_agree->setObjectName(QString::fromUtf8("updater_agree"));
        updater_agree->setGeometry(QRect(330, 220, 21, 21));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        updater_agree->setFont(font1);
        updater_agree->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);"));
        updater_run_text = new QLabel(updater);
        updater_run_text->setObjectName(QString::fromUtf8("updater_run_text"));
        updater_run_text->setGeometry(QRect(330, 10, 461, 16));
        updater_run_text->setFont(font);
        updater_run_text->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));
        updater_run_text->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        updater_run_text->setWordWrap(true);
        updater_agree_text = new QLabel(updater);
        updater_agree_text->setObjectName(QString::fromUtf8("updater_agree_text"));
        updater_agree_text->setGeometry(QRect(352, 222, 441, 81));
        updater_agree_text->setFont(font);
        updater_agree_text->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));
        updater_agree_text->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        updater_agree_text->setWordWrap(true);
        updater_info_text = new QPlainTextEdit(updater);
        updater_info_text->setObjectName(QString::fromUtf8("updater_info_text"));
        updater_info_text->setGeometry(QRect(330, 30, 461, 181));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Courier New"));
        updater_info_text->setFont(font2);
        updater_info_text->setReadOnly(true);
        updater_update = new QPushButton(updater);
        updater_update->setObjectName(QString::fromUtf8("updater_update"));
        updater_update->setEnabled(false);
        updater_update->setGeometry(QRect(320, 320, 471, 41));
        updater_update->setFont(font);
        updater_update->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);font: 12px \"RalewayBold\";"));
        updater->raise();
        welcome->raise();

        retranslateUi(FWUpdate);

        QMetaObject::connectSlotsByName(FWUpdate);
    } // setupUi

    void retranslateUi(QDialog *FWUpdate)
    {
        FWUpdate->setWindowTitle(QCoreApplication::translate("FWUpdate", "Dialog", nullptr));
        welcome_next->setText(QCoreApplication::translate("FWUpdate", "Sure. Next please!", nullptr));
        welcome_intro_1->setText(QCoreApplication::translate("FWUpdate", "Hey there. I'm here to help you updating the\n"
"firmware of your Fluorescence clock. Let's go!", nullptr));
        welcome_pic->setText(QString());
        welcome_intro_2->setText(QCoreApplication::translate("FWUpdate", "Is Fluorescence is powered on and connected?", nullptr));
        updater_intro->setText(QCoreApplication::translate("FWUpdate", "Step 2:", nullptr));
        updater_curr->setText(QCoreApplication::translate("FWUpdate", "v9.99s", nullptr));
        updater_intro_curr->setText(QCoreApplication::translate("FWUpdate", "That's the current version of Fluorescence.", nullptr));
        updater_new->setText(QCoreApplication::translate("FWUpdate", "v11.11s", nullptr));
        updater_intro_new->setText(QCoreApplication::translate("FWUpdate", "That's the version that we'll update to.", nullptr));
        updater_run->setText(QCoreApplication::translate("FWUpdate", "Looks great. Next!", nullptr));
        updater_agree->setText(QString());
        updater_run_text->setText(QCoreApplication::translate("FWUpdate", "Information about the firmware update process", nullptr));
        updater_agree_text->setText(QCoreApplication::translate("FWUpdate", "Updater Agree Text", nullptr));
        updater_info_text->setPlainText(QCoreApplication::translate("FWUpdate", "Plain Text", nullptr));
        updater_update->setText(QCoreApplication::translate("FWUpdate", "Update", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FWUpdate: public Ui_FWUpdate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FWUPDATESTM_H
