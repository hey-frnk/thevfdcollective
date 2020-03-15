/********************************************************************************
** Form generated from reading UI file 'fluorescenceapp.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLUORESCENCEAPP_H
#define UI_FLUORESCENCEAPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "QtColorWidgets/color_line_edit.hpp"
#include "QtColorWidgets/color_wheel.hpp"
#include "QtColorWidgets/gradient_slider.hpp"
#include "QtColorWidgets/hue_slider.hpp"
#include "clickablelabel.h"

QT_BEGIN_NAMESPACE

class Ui_FluorescenceApp
{
public:
    QWidget *centralwidget;
    QGroupBox *com_group;
    QPushButton *com_connect;
    QComboBox *com_select;
    QLabel *com_text;
    QLabel *com_label_connect;
    QWidget *panel_main_control;
    QPushButton *main_settings;
    QPushButton *main_timesync;
    QPushButton *main_custom;
    QPushButton *main_message;
    QPushButton *main_preset;
    QWidget *panel_welcome;
    QLabel *welcome_click2begin;
    QLabel *welcome_text;
    QWidget *panel_presets;
    QTabWidget *tab_presets_container;
    QWidget *tab_presets_static;
    QLabel *static_intro;
    ClickableLabel *static_rainbow;
    ClickableLabel *static_pastelrainbow;
    ClickableLabel *static_red2green;
    ClickableLabel *static_green2blue;
    ClickableLabel *static_red2blue;
    QWidget *tab_presets_dynamic;
    QWidget *panel_custom_colors;
    color_widgets::ColorWheel *custom_color_wheel;
    color_widgets::HueSlider *custom_slider_h;
    color_widgets::GradientSlider *custom_slider_s;
    color_widgets::GradientSlider *custom_slider_l;
    color_widgets::GradientSlider *custom_slider_r;
    color_widgets::GradientSlider *custom_slider_g;
    color_widgets::GradientSlider *custom_slider_b;
    QLabel *custom_label_r;
    QLabel *custom_label_g;
    QLabel *custom_label_b;
    QLabel *custom_label_h;
    QLabel *custom_label_s;
    QLabel *custom_label_l;
    QSpinBox *custom_value_r;
    QSpinBox *custom_value_g;
    QSpinBox *custom_value_b;
    QSpinBox *custom_value_h;
    QSpinBox *custom_value_s;
    QSpinBox *custom_value_l;
    QLabel *custom_label_hex;
    color_widgets::ColorLineEdit *custom_value_hex;
    QLabel *custom_label_w;
    color_widgets::GradientSlider *custom_slider_w;
    QSpinBox *custom_value_w;
    QCheckBox *custom_select_led_1;
    QCheckBox *custom_select_led_2;
    QCheckBox *custom_select_led_3;
    QCheckBox *custom_select_led_4;
    QCheckBox *custom_select_led_5;
    QCheckBox *custom_select_led_6;
    QLabel *custom_intro;
    QLabel *custom_label_r_2;
    QLabel *custom_label_r_3;
    QWidget *panel_timesync;
    QLabel *timesync_time_label;
    QLabel *timesync_date_label;
    QLabel *timesync_intro;
    QPushButton *timesync_button;
    QLabel *timesync_intro_2;
    QWidget *panel_message;
    QPlainTextEdit *message_text;
    QLabel *message_intro;
    QLabel *message_intro_2;
    QLabel *message_intro_3;
    QLabel *message_intro_4;
    QLabel *message_intro_5;
    QLabel *message_intro_6;
    QCheckBox *checkBox;
    QPlainTextEdit *message_text_2;
    QLabel *message_intro_7;
    QPushButton *timesync_button_2;

    void setupUi(QMainWindow *FluorescenceApp)
    {
        if (FluorescenceApp->objectName().isEmpty())
            FluorescenceApp->setObjectName(QString::fromUtf8("FluorescenceApp"));
        FluorescenceApp->setWindowModality(Qt::NonModal);
        FluorescenceApp->resize(760, 500);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        FluorescenceApp->setFont(font);
        FluorescenceApp->setToolButtonStyle(Qt::ToolButtonIconOnly);
        FluorescenceApp->setDockNestingEnabled(false);
        centralwidget = new QWidget(FluorescenceApp);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        com_group = new QGroupBox(centralwidget);
        com_group->setObjectName(QString::fromUtf8("com_group"));
        com_group->setGeometry(QRect(10, 5, 241, 110));
        QFont font1;
        font1.setFamily(QString::fromUtf8(".AppleSystemUIFont"));
        font1.setBold(true);
        font1.setWeight(75);
        com_group->setFont(font1);
        com_group->setStyleSheet(QString::fromUtf8("QGroupBox::title {\n"
"	color: rgb(128, 0, 128);\n"
"}"));
        com_connect = new QPushButton(com_group);
        com_connect->setObjectName(QString::fromUtf8("com_connect"));
        com_connect->setGeometry(QRect(170, 25, 61, 61));
        com_connect->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);"));
        com_select = new QComboBox(com_group);
        com_select->setObjectName(QString::fromUtf8("com_select"));
        com_select->setGeometry(QRect(5, 50, 151, 32));
        QFont font2;
        font2.setFamily(QString::fromUtf8(".AppleSystemUIFont"));
        font2.setBold(false);
        font2.setWeight(50);
        com_select->setFont(font2);
        com_text = new QLabel(com_group);
        com_text->setObjectName(QString::fromUtf8("com_text"));
        com_text->setGeometry(QRect(10, 30, 151, 16));
        com_text->setFont(font1);
        com_text->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);"));
        com_label_connect = new QLabel(com_group);
        com_label_connect->setObjectName(QString::fromUtf8("com_label_connect"));
        com_label_connect->setGeometry(QRect(162, 85, 71, 16));
        com_label_connect->setFont(font1);
        com_label_connect->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128)"));
        com_label_connect->setAlignment(Qt::AlignCenter);
        panel_main_control = new QWidget(centralwidget);
        panel_main_control->setObjectName(QString::fromUtf8("panel_main_control"));
        panel_main_control->setEnabled(false);
        panel_main_control->setGeometry(QRect(250, 30, 501, 61));
        main_settings = new QPushButton(panel_main_control);
        main_settings->setObjectName(QString::fromUtf8("main_settings"));
        main_settings->setGeometry(QRect(410, 0, 81, 61));
        main_settings->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);"));
        main_timesync = new QPushButton(panel_main_control);
        main_timesync->setObjectName(QString::fromUtf8("main_timesync"));
        main_timesync->setGeometry(QRect(300, 0, 81, 61));
        main_timesync->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);"));
        main_custom = new QPushButton(panel_main_control);
        main_custom->setObjectName(QString::fromUtf8("main_custom"));
        main_custom->setGeometry(QRect(100, 0, 81, 61));
        main_custom->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);"));
        main_message = new QPushButton(panel_main_control);
        main_message->setObjectName(QString::fromUtf8("main_message"));
        main_message->setGeometry(QRect(210, 0, 81, 61));
        main_message->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);"));
        main_preset = new QPushButton(panel_main_control);
        main_preset->setObjectName(QString::fromUtf8("main_preset"));
        main_preset->setGeometry(QRect(10, 0, 81, 61));
        main_preset->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);"));
        panel_welcome = new QWidget(centralwidget);
        panel_welcome->setObjectName(QString::fromUtf8("panel_welcome"));
        panel_welcome->setEnabled(true);
        panel_welcome->setGeometry(QRect(10, 120, 741, 371));
        panel_welcome->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255)"));
        welcome_click2begin = new QLabel(panel_welcome);
        welcome_click2begin->setObjectName(QString::fromUtf8("welcome_click2begin"));
        welcome_click2begin->setGeometry(QRect(160, 110, 401, 81));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Arial Black"));
        font3.setPointSize(96);
        font3.setBold(true);
        font3.setWeight(75);
        welcome_click2begin->setFont(font3);
        welcome_click2begin->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);"));
        welcome_click2begin->setAlignment(Qt::AlignCenter);
        welcome_text = new QLabel(panel_welcome);
        welcome_text->setObjectName(QString::fromUtf8("welcome_text"));
        welcome_text->setGeometry(QRect(170, 210, 381, 16));
        welcome_text->setFont(font1);
        welcome_text->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128)"));
        welcome_text->setAlignment(Qt::AlignCenter);
        panel_presets = new QWidget(centralwidget);
        panel_presets->setObjectName(QString::fromUtf8("panel_presets"));
        panel_presets->setEnabled(true);
        panel_presets->setGeometry(QRect(10, 120, 741, 371));
        panel_presets->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255)"));
        tab_presets_container = new QTabWidget(panel_presets);
        tab_presets_container->setObjectName(QString::fromUtf8("tab_presets_container"));
        tab_presets_container->setGeometry(QRect(0, 0, 741, 371));
        tab_presets_container->setFont(font);
        tab_presets_static = new QWidget();
        tab_presets_static->setObjectName(QString::fromUtf8("tab_presets_static"));
        static_intro = new QLabel(tab_presets_static);
        static_intro->setObjectName(QString::fromUtf8("static_intro"));
        static_intro->setGeometry(QRect(10, 10, 411, 16));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Arial"));
        font4.setBold(true);
        font4.setWeight(75);
        static_intro->setFont(font4);
        static_rainbow = new ClickableLabel(tab_presets_static);
        static_rainbow->setObjectName(QString::fromUtf8("static_rainbow"));
        static_rainbow->setGeometry(QRect(220, 130, 281, 81));
        static_rainbow->setCursor(QCursor(Qt::OpenHandCursor));
        static_rainbow->setPixmap(QPixmap(QString::fromUtf8(":/Resources/rainbow_alt.png")));
        static_pastelrainbow = new ClickableLabel(tab_presets_static);
        static_pastelrainbow->setObjectName(QString::fromUtf8("static_pastelrainbow"));
        static_pastelrainbow->setGeometry(QRect(40, 90, 201, 31));
        static_pastelrainbow->setCursor(QCursor(Qt::OpenHandCursor));
        static_pastelrainbow->setPixmap(QPixmap(QString::fromUtf8(":/Resources/pastelrainbow.png")));
        static_red2green = new ClickableLabel(tab_presets_static);
        static_red2green->setObjectName(QString::fromUtf8("static_red2green"));
        static_red2green->setGeometry(QRect(300, 70, 161, 31));
        static_red2green->setCursor(QCursor(Qt::OpenHandCursor));
        static_red2green->setPixmap(QPixmap(QString::fromUtf8(":/Resources/red2green_alt.png")));
        static_green2blue = new ClickableLabel(tab_presets_static);
        static_green2blue->setObjectName(QString::fromUtf8("static_green2blue"));
        static_green2blue->setGeometry(QRect(470, 280, 171, 31));
        static_green2blue->setCursor(QCursor(Qt::OpenHandCursor));
        static_green2blue->setPixmap(QPixmap(QString::fromUtf8(":/Resources/green2blue.png")));
        static_red2blue = new ClickableLabel(tab_presets_static);
        static_red2blue->setObjectName(QString::fromUtf8("static_red2blue"));
        static_red2blue->setGeometry(QRect(530, 170, 141, 31));
        static_red2blue->setCursor(QCursor(Qt::OpenHandCursor));
        static_red2blue->setPixmap(QPixmap(QString::fromUtf8(":/Resources/red2blue1.png")));
        tab_presets_container->addTab(tab_presets_static, QString());
        tab_presets_dynamic = new QWidget();
        tab_presets_dynamic->setObjectName(QString::fromUtf8("tab_presets_dynamic"));
        tab_presets_container->addTab(tab_presets_dynamic, QString());
        panel_custom_colors = new QWidget(centralwidget);
        panel_custom_colors->setObjectName(QString::fromUtf8("panel_custom_colors"));
        panel_custom_colors->setEnabled(true);
        panel_custom_colors->setGeometry(QRect(10, 120, 741, 371));
        panel_custom_colors->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        custom_color_wheel = new color_widgets::ColorWheel(panel_custom_colors);
        custom_color_wheel->setObjectName(QString::fromUtf8("custom_color_wheel"));
        custom_color_wheel->setGeometry(QRect(170, 70, 251, 261));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(custom_color_wheel->sizePolicy().hasHeightForWidth());
        custom_color_wheel->setSizePolicy(sizePolicy);
        custom_color_wheel->setWheelWidth(20u);
        custom_slider_h = new color_widgets::HueSlider(panel_custom_colors);
        custom_slider_h->setObjectName(QString::fromUtf8("custom_slider_h"));
        custom_slider_h->setGeometry(QRect(480, 230, 191, 21));
        custom_slider_h->setMaximum(359);
        custom_slider_s = new color_widgets::GradientSlider(panel_custom_colors);
        custom_slider_s->setObjectName(QString::fromUtf8("custom_slider_s"));
        custom_slider_s->setGeometry(QRect(480, 260, 191, 21));
        custom_slider_s->setMaximum(255);
        custom_slider_l = new color_widgets::GradientSlider(panel_custom_colors);
        custom_slider_l->setObjectName(QString::fromUtf8("custom_slider_l"));
        custom_slider_l->setGeometry(QRect(480, 290, 191, 21));
        custom_slider_l->setMaximum(255);
        custom_slider_r = new color_widgets::GradientSlider(panel_custom_colors);
        custom_slider_r->setObjectName(QString::fromUtf8("custom_slider_r"));
        custom_slider_r->setGeometry(QRect(480, 70, 191, 21));
        custom_slider_r->setMaximum(255);
        custom_slider_g = new color_widgets::GradientSlider(panel_custom_colors);
        custom_slider_g->setObjectName(QString::fromUtf8("custom_slider_g"));
        custom_slider_g->setGeometry(QRect(480, 101, 191, 21));
        custom_slider_g->setMaximum(255);
        custom_slider_b = new color_widgets::GradientSlider(panel_custom_colors);
        custom_slider_b->setObjectName(QString::fromUtf8("custom_slider_b"));
        custom_slider_b->setGeometry(QRect(480, 130, 191, 21));
        custom_slider_b->setMaximum(255);
        custom_slider_b->setPageStep(10);
        custom_label_r = new QLabel(panel_custom_colors);
        custom_label_r->setObjectName(QString::fromUtf8("custom_label_r"));
        custom_label_r->setGeometry(QRect(450, 70, 21, 16));
        custom_label_r->setFont(font);
        custom_label_g = new QLabel(panel_custom_colors);
        custom_label_g->setObjectName(QString::fromUtf8("custom_label_g"));
        custom_label_g->setGeometry(QRect(450, 101, 21, 16));
        custom_label_g->setFont(font);
        custom_label_b = new QLabel(panel_custom_colors);
        custom_label_b->setObjectName(QString::fromUtf8("custom_label_b"));
        custom_label_b->setGeometry(QRect(450, 130, 21, 16));
        custom_label_b->setFont(font);
        custom_label_h = new QLabel(panel_custom_colors);
        custom_label_h->setObjectName(QString::fromUtf8("custom_label_h"));
        custom_label_h->setGeometry(QRect(450, 230, 21, 16));
        custom_label_h->setFont(font);
        custom_label_s = new QLabel(panel_custom_colors);
        custom_label_s->setObjectName(QString::fromUtf8("custom_label_s"));
        custom_label_s->setGeometry(QRect(450, 260, 21, 16));
        custom_label_s->setFont(font);
        custom_label_l = new QLabel(panel_custom_colors);
        custom_label_l->setObjectName(QString::fromUtf8("custom_label_l"));
        custom_label_l->setGeometry(QRect(450, 290, 21, 16));
        custom_label_l->setFont(font);
        custom_value_r = new QSpinBox(panel_custom_colors);
        custom_value_r->setObjectName(QString::fromUtf8("custom_value_r"));
        custom_value_r->setGeometry(QRect(680, 71, 51, 21));
        custom_value_r->setFont(font);
        custom_value_r->setLayoutDirection(Qt::LeftToRight);
        custom_value_r->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_r->setMaximum(255);
        custom_value_g = new QSpinBox(panel_custom_colors);
        custom_value_g->setObjectName(QString::fromUtf8("custom_value_g"));
        custom_value_g->setGeometry(QRect(680, 101, 51, 21));
        custom_value_g->setFont(font);
        custom_value_g->setLayoutDirection(Qt::LeftToRight);
        custom_value_g->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_g->setMaximum(255);
        custom_value_b = new QSpinBox(panel_custom_colors);
        custom_value_b->setObjectName(QString::fromUtf8("custom_value_b"));
        custom_value_b->setGeometry(QRect(680, 130, 51, 21));
        custom_value_b->setFont(font);
        custom_value_b->setLayoutDirection(Qt::LeftToRight);
        custom_value_b->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_b->setMaximum(255);
        custom_value_h = new QSpinBox(panel_custom_colors);
        custom_value_h->setObjectName(QString::fromUtf8("custom_value_h"));
        custom_value_h->setGeometry(QRect(680, 230, 51, 21));
        custom_value_h->setFont(font);
        custom_value_h->setLayoutDirection(Qt::LeftToRight);
        custom_value_h->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_h->setMaximum(359);
        custom_value_s = new QSpinBox(panel_custom_colors);
        custom_value_s->setObjectName(QString::fromUtf8("custom_value_s"));
        custom_value_s->setGeometry(QRect(680, 260, 51, 21));
        custom_value_s->setFont(font);
        custom_value_s->setLayoutDirection(Qt::LeftToRight);
        custom_value_s->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_s->setMaximum(255);
        custom_value_l = new QSpinBox(panel_custom_colors);
        custom_value_l->setObjectName(QString::fromUtf8("custom_value_l"));
        custom_value_l->setGeometry(QRect(680, 290, 51, 21));
        custom_value_l->setFont(font);
        custom_value_l->setLayoutDirection(Qt::LeftToRight);
        custom_value_l->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_l->setMaximum(255);
        custom_label_hex = new QLabel(panel_custom_colors);
        custom_label_hex->setObjectName(QString::fromUtf8("custom_label_hex"));
        custom_label_hex->setGeometry(QRect(450, 320, 101, 16));
        custom_label_hex->setFont(font4);
        custom_value_hex = new color_widgets::ColorLineEdit(panel_custom_colors);
        custom_value_hex->setObjectName(QString::fromUtf8("custom_value_hex"));
        custom_value_hex->setGeometry(QRect(670, 320, 61, 21));
        custom_value_hex->setFont(font);
        custom_label_w = new QLabel(panel_custom_colors);
        custom_label_w->setObjectName(QString::fromUtf8("custom_label_w"));
        custom_label_w->setGeometry(QRect(450, 160, 31, 16));
        custom_label_w->setFont(font);
        custom_slider_w = new color_widgets::GradientSlider(panel_custom_colors);
        custom_slider_w->setObjectName(QString::fromUtf8("custom_slider_w"));
        custom_slider_w->setGeometry(QRect(480, 160, 191, 21));
        custom_slider_w->setMaximum(255);
        custom_value_w = new QSpinBox(panel_custom_colors);
        custom_value_w->setObjectName(QString::fromUtf8("custom_value_w"));
        custom_value_w->setGeometry(QRect(680, 160, 51, 21));
        custom_value_w->setFont(font);
        custom_value_w->setLayoutDirection(Qt::LeftToRight);
        custom_value_w->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_w->setMaximum(255);
        custom_select_led_1 = new QCheckBox(panel_custom_colors);
        custom_select_led_1->setObjectName(QString::fromUtf8("custom_select_led_1"));
        custom_select_led_1->setGeometry(QRect(30, 240, 61, 20));
        custom_select_led_1->setFont(font);
        custom_select_led_1->setChecked(true);
        custom_select_led_2 = new QCheckBox(panel_custom_colors);
        custom_select_led_2->setObjectName(QString::fromUtf8("custom_select_led_2"));
        custom_select_led_2->setGeometry(QRect(30, 210, 61, 20));
        custom_select_led_2->setFont(font);
        custom_select_led_2->setChecked(true);
        custom_select_led_3 = new QCheckBox(panel_custom_colors);
        custom_select_led_3->setObjectName(QString::fromUtf8("custom_select_led_3"));
        custom_select_led_3->setGeometry(QRect(30, 180, 61, 20));
        custom_select_led_3->setFont(font);
        custom_select_led_3->setChecked(true);
        custom_select_led_4 = new QCheckBox(panel_custom_colors);
        custom_select_led_4->setObjectName(QString::fromUtf8("custom_select_led_4"));
        custom_select_led_4->setGeometry(QRect(30, 150, 61, 20));
        custom_select_led_4->setFont(font);
        custom_select_led_4->setChecked(true);
        custom_select_led_5 = new QCheckBox(panel_custom_colors);
        custom_select_led_5->setObjectName(QString::fromUtf8("custom_select_led_5"));
        custom_select_led_5->setGeometry(QRect(30, 120, 61, 20));
        custom_select_led_5->setFont(font);
        custom_select_led_5->setChecked(true);
        custom_select_led_6 = new QCheckBox(panel_custom_colors);
        custom_select_led_6->setObjectName(QString::fromUtf8("custom_select_led_6"));
        custom_select_led_6->setGeometry(QRect(30, 90, 61, 20));
        custom_select_led_6->setFont(font);
        custom_select_led_6->setChecked(true);
        custom_intro = new QLabel(panel_custom_colors);
        custom_intro->setObjectName(QString::fromUtf8("custom_intro"));
        custom_intro->setGeometry(QRect(30, 40, 301, 16));
        custom_intro->setFont(font4);
        custom_intro->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);"));
        custom_label_r_2 = new QLabel(panel_custom_colors);
        custom_label_r_2->setObjectName(QString::fromUtf8("custom_label_r_2"));
        custom_label_r_2->setGeometry(QRect(450, 41, 211, 16));
        custom_label_r_2->setFont(font4);
        custom_label_r_3 = new QLabel(panel_custom_colors);
        custom_label_r_3->setObjectName(QString::fromUtf8("custom_label_r_3"));
        custom_label_r_3->setGeometry(QRect(450, 200, 171, 16));
        custom_label_r_3->setFont(font4);
        panel_timesync = new QWidget(centralwidget);
        panel_timesync->setObjectName(QString::fromUtf8("panel_timesync"));
        panel_timesync->setGeometry(QRect(10, 120, 741, 371));
        panel_timesync->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        timesync_time_label = new QLabel(panel_timesync);
        timesync_time_label->setObjectName(QString::fromUtf8("timesync_time_label"));
        timesync_time_label->setGeometry(QRect(60, 120, 391, 91));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Arial"));
        font5.setPointSize(96);
        font5.setBold(true);
        font5.setWeight(75);
        timesync_time_label->setFont(font5);
        timesync_time_label->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);"));
        timesync_date_label = new QLabel(panel_timesync);
        timesync_date_label->setObjectName(QString::fromUtf8("timesync_date_label"));
        timesync_date_label->setGeometry(QRect(60, 220, 391, 31));
        QFont font6;
        font6.setFamily(QString::fromUtf8("Arial"));
        font6.setPointSize(24);
        font6.setBold(true);
        font6.setWeight(75);
        timesync_date_label->setFont(font6);
        timesync_intro = new QLabel(panel_timesync);
        timesync_intro->setObjectName(QString::fromUtf8("timesync_intro"));
        timesync_intro->setGeometry(QRect(510, 80, 211, 41));
        timesync_intro->setFont(font4);
        timesync_button = new QPushButton(panel_timesync);
        timesync_button->setObjectName(QString::fromUtf8("timesync_button"));
        timesync_button->setGeometry(QRect(510, 120, 211, 121));
        QFont font7;
        font7.setPointSize(36);
        font7.setBold(true);
        font7.setWeight(75);
        timesync_button->setFont(font7);
        timesync_button->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);"));
        timesync_button->setFlat(false);
        timesync_intro_2 = new QLabel(panel_timesync);
        timesync_intro_2->setObjectName(QString::fromUtf8("timesync_intro_2"));
        timesync_intro_2->setGeometry(QRect(510, 240, 211, 41));
        timesync_intro_2->setFont(font4);
        panel_message = new QWidget(centralwidget);
        panel_message->setObjectName(QString::fromUtf8("panel_message"));
        panel_message->setGeometry(QRect(10, 120, 741, 371));
        panel_message->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        message_text = new QPlainTextEdit(panel_message);
        message_text->setObjectName(QString::fromUtf8("message_text"));
        message_text->setGeometry(QRect(20, 50, 691, 71));
        QFont font8;
        font8.setFamily(QString::fromUtf8("Arial"));
        font8.setPointSize(48);
        font8.setBold(true);
        font8.setWeight(75);
        message_text->setFont(font8);
        message_text->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);"));
        message_text->setLineWrapMode(QPlainTextEdit::NoWrap);
        message_intro = new QLabel(panel_message);
        message_intro->setObjectName(QString::fromUtf8("message_intro"));
        message_intro->setGeometry(QRect(20, 30, 661, 16));
        message_intro->setFont(font);
        message_intro_2 = new QLabel(panel_message);
        message_intro_2->setObjectName(QString::fromUtf8("message_intro_2"));
        message_intro_2->setGeometry(QRect(20, 140, 311, 16));
        message_intro_2->setFont(font4);
        message_intro_3 = new QLabel(panel_message);
        message_intro_3->setObjectName(QString::fromUtf8("message_intro_3"));
        message_intro_3->setGeometry(QRect(30, 160, 601, 16));
        message_intro_3->setFont(font);
        message_intro_4 = new QLabel(panel_message);
        message_intro_4->setObjectName(QString::fromUtf8("message_intro_4"));
        message_intro_4->setGeometry(QRect(30, 180, 601, 16));
        message_intro_4->setFont(font);
        message_intro_5 = new QLabel(panel_message);
        message_intro_5->setObjectName(QString::fromUtf8("message_intro_5"));
        message_intro_5->setGeometry(QRect(30, 200, 601, 16));
        message_intro_5->setFont(font);
        message_intro_6 = new QLabel(panel_message);
        message_intro_6->setObjectName(QString::fromUtf8("message_intro_6"));
        message_intro_6->setGeometry(QRect(30, 220, 601, 16));
        message_intro_6->setFont(font);
        checkBox = new QCheckBox(panel_message);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(20, 240, 681, 20));
        checkBox->setFont(font);
        message_text_2 = new QPlainTextEdit(panel_message);
        message_text_2->setObjectName(QString::fromUtf8("message_text_2"));
        message_text_2->setGeometry(QRect(20, 280, 51, 71));
        message_text_2->setFont(font8);
        message_text_2->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);"));
        message_text_2->setLineWrapMode(QPlainTextEdit::NoWrap);
        message_intro_7 = new QLabel(panel_message);
        message_intro_7->setObjectName(QString::fromUtf8("message_intro_7"));
        message_intro_7->setGeometry(QRect(90, 280, 311, 61));
        message_intro_7->setFont(font8);
        message_intro_7->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);"));
        timesync_button_2 = new QPushButton(panel_message);
        timesync_button_2->setObjectName(QString::fromUtf8("timesync_button_2"));
        timesync_button_2->setGeometry(QRect(340, 280, 371, 71));
        timesync_button_2->setFont(font7);
        timesync_button_2->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);"));
        timesync_button_2->setFlat(false);
        FluorescenceApp->setCentralWidget(centralwidget);
        panel_custom_colors->raise();
        com_group->raise();
        panel_welcome->raise();
        panel_presets->raise();
        panel_timesync->raise();
        panel_main_control->raise();
        panel_message->raise();

        retranslateUi(FluorescenceApp);

        tab_presets_container->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FluorescenceApp);
    } // setupUi

    void retranslateUi(QMainWindow *FluorescenceApp)
    {
        FluorescenceApp->setWindowTitle(QCoreApplication::translate("FluorescenceApp", "FluorescenceApp", nullptr));
        com_group->setTitle(QCoreApplication::translate("FluorescenceApp", "Connect to Fluorescence", nullptr));
        com_connect->setText(QString());
        com_text->setText(QCoreApplication::translate("FluorescenceApp", "Where's Fluorescence?", nullptr));
        com_label_connect->setText(QCoreApplication::translate("FluorescenceApp", "Connect", nullptr));
        main_settings->setText(QCoreApplication::translate("FluorescenceApp", "Settings", nullptr));
        main_timesync->setText(QCoreApplication::translate("FluorescenceApp", "Time\n"
"Sync", nullptr));
        main_custom->setText(QCoreApplication::translate("FluorescenceApp", "Custom\n"
"Colors", nullptr));
        main_message->setText(QCoreApplication::translate("FluorescenceApp", "Send\n"
"Message", nullptr));
        main_preset->setText(QCoreApplication::translate("FluorescenceApp", "Color\n"
"Presets", nullptr));
        welcome_click2begin->setText(QCoreApplication::translate("FluorescenceApp", "CLICK", nullptr));
        welcome_text->setText(QCoreApplication::translate("FluorescenceApp", "on the controls above to connect fluorescence & get started", nullptr));
        static_intro->setText(QCoreApplication::translate("FluorescenceApp", "Some color presets that look fantastic on fluorescence.", nullptr));
        static_rainbow->setText(QString());
        static_pastelrainbow->setText(QString());
        static_red2green->setText(QString());
        static_green2blue->setText(QString());
        static_red2blue->setText(QString());
        tab_presets_container->setTabText(tab_presets_container->indexOf(tab_presets_static), QCoreApplication::translate("FluorescenceApp", "Static Presets", nullptr));
        tab_presets_container->setTabText(tab_presets_container->indexOf(tab_presets_dynamic), QCoreApplication::translate("FluorescenceApp", "Dynamic Presets", nullptr));
        custom_label_r->setText(QCoreApplication::translate("FluorescenceApp", "R:", nullptr));
        custom_label_g->setText(QCoreApplication::translate("FluorescenceApp", "G:", nullptr));
        custom_label_b->setText(QCoreApplication::translate("FluorescenceApp", "B:", nullptr));
        custom_label_h->setText(QCoreApplication::translate("FluorescenceApp", "H:", nullptr));
        custom_label_s->setText(QCoreApplication::translate("FluorescenceApp", "S:", nullptr));
        custom_label_l->setText(QCoreApplication::translate("FluorescenceApp", "L:", nullptr));
        custom_label_hex->setText(QCoreApplication::translate("FluorescenceApp", "hex color code:", nullptr));
        custom_label_w->setText(QCoreApplication::translate("FluorescenceApp", "W:", nullptr));
        custom_select_led_1->setText(QCoreApplication::translate("FluorescenceApp", "LED 1", nullptr));
        custom_select_led_2->setText(QCoreApplication::translate("FluorescenceApp", "LED 2", nullptr));
        custom_select_led_3->setText(QCoreApplication::translate("FluorescenceApp", "LED 3", nullptr));
        custom_select_led_4->setText(QCoreApplication::translate("FluorescenceApp", "LED 4", nullptr));
        custom_select_led_5->setText(QCoreApplication::translate("FluorescenceApp", "LED 5", nullptr));
        custom_select_led_6->setText(QCoreApplication::translate("FluorescenceApp", "LED 6", nullptr));
        custom_intro->setText(QCoreApplication::translate("FluorescenceApp", "Time to paint the night sky. Or some LEDs.", nullptr));
        custom_label_r_2->setText(QCoreApplication::translate("FluorescenceApp", "red, green, blue & white.", nullptr));
        custom_label_r_3->setText(QCoreApplication::translate("FluorescenceApp", "hue, saturation & lightness.", nullptr));
        timesync_time_label->setText(QCoreApplication::translate("FluorescenceApp", "20:15:16", nullptr));
        timesync_date_label->setText(QCoreApplication::translate("FluorescenceApp", "Friyay, March 7th 2032", nullptr));
        timesync_intro->setText(QCoreApplication::translate("FluorescenceApp", "Match the time of Fluorescence\n"
"to the time of your computer", nullptr));
        timesync_button->setText(QCoreApplication::translate("FluorescenceApp", "Sync", nullptr));
        timesync_intro_2->setText(QCoreApplication::translate("FluorescenceApp", "Ready to sync.", nullptr));
        message_text->setPlainText(QCoreApplication::translate("FluorescenceApp", "ABCDEFGHIJKLMNOPQRSTUVWX", nullptr));
        message_intro->setText(QCoreApplication::translate("FluorescenceApp", "Write a poem. Or say something lovely. We wrote this so that you can have a fun time :)", nullptr));
        message_intro_2->setText(QCoreApplication::translate("FluorescenceApp", "Rules of the game:", nullptr));
        message_intro_3->setText(QCoreApplication::translate("FluorescenceApp", "- 24 letters for you. The message will be split in max. three parts displayed separately", nullptr));
        message_intro_4->setText(QCoreApplication::translate("FluorescenceApp", "- Write in CAPITAL LETTERS, and only use those the display can display (see next point)", nullptr));
        message_intro_5->setText(QCoreApplication::translate("FluorescenceApp", "- Allowed: A B C D E F G H I J L N O P Q R S T U V Y, 0, spacebar and minus (-)", nullptr));
        message_intro_6->setText(QCoreApplication::translate("FluorescenceApp", "- It's a bit like SNAPCHAT. But you decide for how long each message part is on display:", nullptr));
        checkBox->setText(QCoreApplication::translate("FluorescenceApp", "Use this message as the new welcome message Fluorescence uses to greet you on startup", nullptr));
        message_text_2->setPlainText(QCoreApplication::translate("FluorescenceApp", "1", nullptr));
        message_intro_7->setText(QCoreApplication::translate("FluorescenceApp", "second(s)", nullptr));
        timesync_button_2->setText(QCoreApplication::translate("FluorescenceApp", "Send Message", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FluorescenceApp: public Ui_FluorescenceApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLUORESCENCEAPP_H
