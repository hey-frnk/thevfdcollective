/********************************************************************************
** Form generated from reading UI file 'fwupdatestm.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FWUPDATESTM_H
#define UI_FWUPDATESTM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "clickablelabel.h"

QT_BEGIN_NAMESPACE

class Ui_FWUpdateSTM
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
    QCheckBox *updater_geekmode;
    QPlainTextEdit *plainTextEdit;
    QLabel *label;
    QLabel *label_2;
    QPlainTextEdit *plainTextEdit_2;
    QLabel *label_3;
    QPlainTextEdit *plainTextEdit_3;
    QGroupBox *groupBox;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QPushButton *pushButton_4;
    QPlainTextEdit *plainTextEdit_4;
    QGroupBox *groupBox_3;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    ClickableLabel *label_4;

    void setupUi(QDialog *FWUpdateSTM)
    {
        if (FWUpdateSTM->objectName().isEmpty())
            FWUpdateSTM->setObjectName(QString::fromUtf8("FWUpdateSTM"));
        FWUpdateSTM->resize(750, 370);
        welcome = new QWidget(FWUpdateSTM);
        welcome->setObjectName(QString::fromUtf8("welcome"));
        welcome->setGeometry(QRect(0, 0, 321, 371));
        welcome->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        welcome_next = new QPushButton(welcome);
        welcome_next->setObjectName(QString::fromUtf8("welcome_next"));
        welcome_next->setGeometry(QRect(120, 320, 191, 41));
        QFont font;
        font.setFamily(QString::fromUtf8(".AppleSystemUIFont"));
        welcome_next->setFont(font);
        welcome_next->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);"));
        welcome_intro_1 = new QLabel(welcome);
        welcome_intro_1->setObjectName(QString::fromUtf8("welcome_intro_1"));
        welcome_intro_1->setGeometry(QRect(10, 10, 301, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8(".AppleSystemUIFont"));
        font1.setBold(true);
        font1.setWeight(75);
        welcome_intro_1->setFont(font1);
        welcome_intro_1->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);"));
        welcome_pic = new QLabel(welcome);
        welcome_pic->setObjectName(QString::fromUtf8("welcome_pic"));
        welcome_pic->setGeometry(QRect(10, 50, 301, 191));
        welcome_pic->setPixmap(QPixmap(QString::fromUtf8(":/Resources/HXP_TN.png")));
        welcome_intro_2 = new QLabel(welcome);
        welcome_intro_2->setObjectName(QString::fromUtf8("welcome_intro_2"));
        welcome_intro_2->setGeometry(QRect(10, 240, 301, 31));
        welcome_intro_2->setFont(font1);
        welcome_intro_2->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);"));
        updater = new QWidget(FWUpdateSTM);
        updater->setObjectName(QString::fromUtf8("updater"));
        updater->setGeometry(QRect(0, 0, 751, 371));
        updater->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        updater_intro = new QLabel(updater);
        updater_intro->setObjectName(QString::fromUtf8("updater_intro"));
        updater_intro->setGeometry(QRect(20, 20, 281, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8(".AppleSystemUIFont"));
        font2.setPointSize(18);
        font2.setBold(true);
        font2.setWeight(75);
        updater_intro->setFont(font2);
        updater_intro->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);"));
        updater_intro->setAlignment(Qt::AlignCenter);
        updater_curr = new QLabel(updater);
        updater_curr->setObjectName(QString::fromUtf8("updater_curr"));
        updater_curr->setGeometry(QRect(10, 80, 301, 31));
        QFont font3;
        font3.setFamily(QString::fromUtf8(".AppleSystemUIFont"));
        font3.setPointSize(26);
        font3.setBold(true);
        font3.setWeight(75);
        updater_curr->setFont(font3);
        updater_curr->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);"));
        updater_curr->setAlignment(Qt::AlignCenter);
        updater_intro_curr = new QLabel(updater);
        updater_intro_curr->setObjectName(QString::fromUtf8("updater_intro_curr"));
        updater_intro_curr->setGeometry(QRect(10, 110, 301, 31));
        updater_intro_curr->setFont(font1);
        updater_intro_curr->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);"));
        updater_intro_curr->setAlignment(Qt::AlignCenter);
        updater_new = new QLabel(updater);
        updater_new->setObjectName(QString::fromUtf8("updater_new"));
        updater_new->setGeometry(QRect(10, 180, 301, 31));
        updater_new->setFont(font3);
        updater_new->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);"));
        updater_new->setAlignment(Qt::AlignCenter);
        updater_intro_new = new QLabel(updater);
        updater_intro_new->setObjectName(QString::fromUtf8("updater_intro_new"));
        updater_intro_new->setGeometry(QRect(10, 210, 301, 31));
        updater_intro_new->setFont(font1);
        updater_intro_new->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);"));
        updater_intro_new->setAlignment(Qt::AlignCenter);
        updater_run = new QPushButton(updater);
        updater_run->setObjectName(QString::fromUtf8("updater_run"));
        updater_run->setGeometry(QRect(20, 300, 281, 61));
        updater_run->setFont(font3);
        updater_run->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);"));
        updater_geekmode = new QCheckBox(updater);
        updater_geekmode->setObjectName(QString::fromUtf8("updater_geekmode"));
        updater_geekmode->setGeometry(QRect(110, 270, 101, 20));
        QFont font4;
        font4.setBold(true);
        font4.setWeight(75);
        updater_geekmode->setFont(font4);
        updater_geekmode->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);"));
        plainTextEdit = new QPlainTextEdit(updater);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(330, 30, 261, 21));
        plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit->setReadOnly(true);
        label = new QLabel(updater);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(330, 10, 101, 16));
        label_2 = new QLabel(updater);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(330, 60, 91, 16));
        plainTextEdit_2 = new QPlainTextEdit(updater);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));
        plainTextEdit_2->setGeometry(QRect(330, 80, 121, 21));
        plainTextEdit_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_2->setReadOnly(true);
        label_3 = new QLabel(updater);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(470, 60, 91, 16));
        plainTextEdit_3 = new QPlainTextEdit(updater);
        plainTextEdit_3->setObjectName(QString::fromUtf8("plainTextEdit_3"));
        plainTextEdit_3->setGeometry(QRect(470, 80, 71, 21));
        plainTextEdit_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_3->setReadOnly(true);
        groupBox = new QGroupBox(updater);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(330, 110, 261, 101));
        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(10, 30, 241, 20));
        checkBox->setChecked(true);
        checkBox_2 = new QCheckBox(groupBox);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setGeometry(QRect(10, 50, 241, 20));
        checkBox_3 = new QCheckBox(groupBox);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setGeometry(QRect(10, 70, 241, 20));
        pushButton_4 = new QPushButton(updater);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(320, 210, 271, 32));
        pushButton_4->setFont(font4);
        plainTextEdit_4 = new QPlainTextEdit(updater);
        plainTextEdit_4->setObjectName(QString::fromUtf8("plainTextEdit_4"));
        plainTextEdit_4->setGeometry(QRect(330, 250, 261, 111));
        plainTextEdit_4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_4->setReadOnly(true);
        groupBox_3 = new QGroupBox(updater);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(600, 10, 141, 171));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 25, 121, 31));
        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 60, 121, 31));
        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 95, 121, 31));
        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 130, 121, 31));
        label_4 = new ClickableLabel(updater);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(530, 10, 61, 16));
        QFont font5;
        font5.setUnderline(true);
        label_4->setFont(font5);
        label_4->setCursor(QCursor(Qt::OpenHandCursor));
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(7, 64, 128);"));

        retranslateUi(FWUpdateSTM);

        QMetaObject::connectSlotsByName(FWUpdateSTM);
    } // setupUi

    void retranslateUi(QDialog *FWUpdateSTM)
    {
        FWUpdateSTM->setWindowTitle(QCoreApplication::translate("FWUpdateSTM", "Dialog", nullptr));
        welcome_next->setText(QCoreApplication::translate("FWUpdateSTM", "Sure. Next please!", nullptr));
        welcome_intro_1->setText(QCoreApplication::translate("FWUpdateSTM", "Hey there. I'm here to help you updating the\n"
"firmware of your Fluorescence clock. Let's go!", nullptr));
        welcome_pic->setText(QString());
        welcome_intro_2->setText(QCoreApplication::translate("FWUpdateSTM", "Is Fluorescence is powered on and connected?", nullptr));
        updater_intro->setText(QCoreApplication::translate("FWUpdateSTM", "Step 2:", nullptr));
        updater_curr->setText(QCoreApplication::translate("FWUpdateSTM", "v9.99s", nullptr));
        updater_intro_curr->setText(QCoreApplication::translate("FWUpdateSTM", "That's the current version of Fluorescence.", nullptr));
        updater_new->setText(QCoreApplication::translate("FWUpdateSTM", "v11.11s", nullptr));
        updater_intro_new->setText(QCoreApplication::translate("FWUpdateSTM", "That's the version that we'll update to.", nullptr));
        updater_run->setText(QCoreApplication::translate("FWUpdateSTM", "Looks great. Go!", nullptr));
        updater_geekmode->setText(QCoreApplication::translate("FWUpdateSTM", "Geek Mode", nullptr));
        label->setText(QCoreApplication::translate("FWUpdateSTM", "Firmware path:", nullptr));
        label_2->setText(QCoreApplication::translate("FWUpdateSTM", "Start Address", nullptr));
        label_3->setText(QCoreApplication::translate("FWUpdateSTM", "Size", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FWUpdateSTM", "Programming options:", nullptr));
        checkBox->setText(QCoreApplication::translate("FWUpdateSTM", "Verify programming", nullptr));
        checkBox_2->setText(QCoreApplication::translate("FWUpdateSTM", "Skip flash erase before programming", nullptr));
        checkBox_3->setText(QCoreApplication::translate("FWUpdateSTM", "Run after programming", nullptr));
        pushButton_4->setText(QCoreApplication::translate("FWUpdateSTM", "Write/Program", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("FWUpdateSTM", "Device Info", nullptr));
        label_11->setText(QCoreApplication::translate("FWUpdateSTM", "Device:\n"
"STM32", nullptr));
        label_12->setText(QCoreApplication::translate("FWUpdateSTM", "Type:\n"
"Fluorescence", nullptr));
        label_13->setText(QCoreApplication::translate("FWUpdateSTM", "Device ID:\n"
"12345", nullptr));
        label_14->setText(QCoreApplication::translate("FWUpdateSTM", "CPU:\n"
"STM32F042K6", nullptr));
        label_4->setText(QCoreApplication::translate("FWUpdateSTM", "Change...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FWUpdateSTM: public Ui_FWUpdateSTM {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FWUPDATESTM_H
