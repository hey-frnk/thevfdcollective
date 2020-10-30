/********************************************************************************
** Form generated from reading UI file 'fluorescenceapp.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLUORESCENCEAPP_H
#define UI_FLUORESCENCEAPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <clickablelabel.h>
#include "movablelabel.h"

QT_BEGIN_NAMESPACE

class Ui_FluorescenceApp
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_18;
    QWidget *menu_layout;
    QHBoxLayout *menu_hide;
    ClickableLabel *menu_button;
    QLabel *menu_about;
    QLabel *menu_about_2;
    ClickableLabel *menu_button_balancer;
    QWidget *parent_menu_control;
    QHBoxLayout *horizontalLayout_9;
    QWidget *menu_controls_layout;
    QVBoxLayout *menu_vlayout;
    QPushButton *main_welcome;
    QPushButton *main_timesync;
    QPushButton *main_preset;
    QPushButton *main_custom;
    QPushButton *main_settings;
    QPushButton *main_message;
    QWidget *control_parent;
    QVBoxLayout *verticalLayout_17;
    QScrollArea *panel_presets;
    QWidget *panel_presets_content;
    QVBoxLayout *verticalLayout_4;
    QTabWidget *tab_presets_container;
    QWidget *tab_presets_static;
    QVBoxLayout *verticalLayout_8;
    QScrollArea *scroll_presets_static;
    QWidget *scroll_presets_static_content;
    QVBoxLayout *verticalLayout_19;
    QLabel *static_intro;
    ClickableLabel *static_off;
    ClickableLabel *static_white;
    ClickableLabel *static_warmwhite;
    ClickableLabel *static_rainbow;
    ClickableLabel *static_pastelrainbow;
    ClickableLabel *static_red2blue;
    ClickableLabel *static_red2green;
    ClickableLabel *static_green2blue;
    ClickableLabel *static_blue;
    ClickableLabel *static_cyan;
    ClickableLabel *static_green;
    ClickableLabel *static_yellow;
    ClickableLabel *static_orange;
    ClickableLabel *static_red;
    ClickableLabel *static_magenta;
    ClickableLabel *static_violet;
    QWidget *tab_presets_dynamic;
    QHBoxLayout *horizontalLayout_6;
    QScrollArea *scroll_presets_dynamic;
    QWidget *tab_presets_dynamic_content;
    QVBoxLayout *verticalLayout_6;
    QWidget *panel_presets_control_grid;
    QGridLayout *gridLayout_11;
    QPushButton *dynamic_bliss_dnc;
    QPushButton *dynamic_spectrum;
    QPushButton *dynamic_rainbow;
    QPushButton *dynamic_chase;
    QPushButton *dynamic_music;
    QPushButton *dynamic_timecode;
    QPushButton *dynamic_police;
    QWidget *panel_dyn_rnb;
    QGridLayout *gridLayout_10;
    QLabel *panel_dyn_rnb_ptext_1;
    QLabel *panel_dyn_rnb_p1_1;
    QSlider *panel_dyn_rnb_param1;
    QLabel *panel_dyn_rnb_p1_2;
    QLabel *panel_dyn_rnb_ptext_2;
    QComboBox *panel_dyn_rnb_param2;
    QPushButton *panel_dyn_rnb_set;
    QWidget *panel_dyn_spectrum;
    QGridLayout *gridLayout_6;
    QLabel *panel_dyn_spectrum_ptext1;
    QPushButton *panel_dyn_spectrum_set;
    QLabel *panel_dyn_spectrum_ptext2;
    QComboBox *panel_dyn_spectrum_param_2;
    QLabel *panel_dyn_spectrum_intro;
    QComboBox *panel_dyn_spectrum_param_1;
    QWidget *panel_dyn_chase;
    QGridLayout *gridLayout_9;
    QLabel *panel_dyn_chase_ptext_1;
    QLabel *panel_dyn_chase_p1;
    QSlider *panel_dyn_chase_param1;
    QLabel *panel_dyn_chase_p2;
    QLabel *panel_dyn_chase_ptext_2;
    QComboBox *panel_dyn_chase_param2;
    QPushButton *panel_dyn_chase_set;
    QWidget *panel_dyn_tcode;
    QGridLayout *gridLayout_4;
    QLabel *panel_dyn_tcode_intro;
    QLabel *panel_dyn_tcode_ptext;
    QComboBox *panel_dyn_tcode_param1;
    QPushButton *panel_dyn_tcode_set;
    QWidget *panel_dyn_music;
    QGridLayout *gridLayout_8;
    QLabel *panel_dyn_music_ptext_1;
    QLabel *panel_dyn_music_p2;
    QPushButton *panel_dyn_music_set;
    QLabel *panel_dyn_music_p1;
    QComboBox *panel_dyn_music_param2;
    QSlider *panel_dyn_music_param1;
    QLabel *panel_dyn_music_ptext_2;
    QWidget *panel_dyn_cop;
    QGridLayout *gridLayout_7;
    QLabel *panel_dyn_cop_intro;
    QLabel *panel_dyn_cop_ptext;
    QComboBox *panel_dyn_cop_param1;
    QPushButton *panel_dyn_cop_set;
    QWidget *panel_dyn_bliss;
    QGridLayout *gridLayout_5;
    QComboBox *panel_dyn_bliss_param;
    QLabel *panel_dyn_bliss_ptext;
    QPushButton *panel_dyn_bliss_set;
    QLabel *panel_dyn_bliss_description;
    QLabel *dynamic_intro;
    QWidget *dynamic_c_layout;
    QHBoxLayout *horizontalLayout_3;
    ClickableLabel *dynamic_c1;
    ClickableLabel *dynamic_c2;
    ClickableLabel *dynamic_c3;
    ClickableLabel *dynamic_c4;
    ClickableLabel *dynamic_c5;
    ClickableLabel *dynamic_c6;
    QLabel *dynamic_t_intro;
    QWidget *dynamic_t;
    QHBoxLayout *horizontalLayout_4;
    ClickableLabel *dynamic_t_1;
    ClickableLabel *dynamic_t_2;
    ClickableLabel *dynamic_t_3;
    ClickableLabel *dynamic_t_4;
    ClickableLabel *dynamic_t_5;
    ClickableLabel *dynamic_t_6;
    ClickableLabel *dynamic_t_7;
    ClickableLabel *dynamic_t_8;
    ClickableLabel *dynamic_t_9;
    ClickableLabel *dynamic_t_10;
    ClickableLabel *dynamic_t_11;
    ClickableLabel *dynamic_t_12;
    ClickableLabel *dynamic_t_13;
    ClickableLabel *dynamic_t_14;
    ClickableLabel *dynamic_t_15;
    ClickableLabel *dynamic_t_16;
    ClickableLabel *dynamic_t_17;
    ClickableLabel *dynamic_t_18;
    ClickableLabel *dynamic_t_19;
    ClickableLabel *dynamic_t_20;
    ClickableLabel *dynamic_t_21;
    QWidget *tab_moodsync;
    QHBoxLayout *horizontalLayout_7;
    QScrollArea *scroll_moodsync;
    QWidget *tab_moodsync_content;
    QVBoxLayout *verticalLayout_7;
    QLabel *lisync_intro;
    QWidget *lisync_computer_layout;
    QLabel *lisync_computer;
    ClickableLabel *lisync_sample;
    QWidget *lisync_layout;
    QHBoxLayout *horizontalLayout_5;
    ClickableLabel *lisync_c1;
    ClickableLabel *lisync_c2;
    ClickableLabel *lisync_c3;
    ClickableLabel *lisync_c4;
    ClickableLabel *lisync_c5;
    ClickableLabel *lisync_c6;
    QLabel *lisync_status;
    QWidget *tab_shuffle;
    QHBoxLayout *horizontalLayout_8;
    QScrollArea *scroll_shuffle;
    QWidget *tab_shuffle_content;
    QGridLayout *gridLayout_12;
    QLabel *shuffle_intro1;
    QLabel *shuffle_intro3;
    QCheckBox *shuffle_en_static;
    QCheckBox *shuffle_en_chase;
    QCheckBox *shuffle_en_bliss;
    QCheckBox *shuffle_en_tcode;
    QCheckBox *shuffle_en_spectrum;
    QCheckBox *shuffle_en_music;
    QCheckBox *shuffle_en_rnb;
    QCheckBox *shuffle_en_cop;
    QLabel *shuffle_intro2;
    QComboBox *shuffle_speed;
    QPushButton *shuffle_set;
    QScrollArea *panel_custom_colors;
    QWidget *panel_custom_colors_content;
    QVBoxLayout *verticalLayout_3;
    QLabel *custom_intro;
    QWidget *custom_color_wheel_parent;
    MovableLabel *custom_color_wheel_cursor;
    QFormLayout *formLayout;
    ClickableLabel *custom_color_wheel;
    QWidget *custom_led_grid;
    QGridLayout *gridLayout;
    QCheckBox *custom_select_led_6;
    QCheckBox *custom_select_led_5;
    QCheckBox *custom_select_led_4;
    QCheckBox *custom_select_led_3;
    QCheckBox *custom_select_led_2;
    QCheckBox *custom_select_led_1;
    QLabel *custom_label_r_2;
    QWidget *custom_grid_rgbw;
    QGridLayout *gridLayout_2;
    QLabel *custom_label_r;
    QSlider *custom_slider_r;
    QSpinBox *custom_value_r;
    QLabel *custom_label_g;
    QSlider *custom_slider_g;
    QSpinBox *custom_value_g;
    QLabel *custom_label_b;
    QSlider *custom_slider_b;
    QSpinBox *custom_value_b;
    QLabel *custom_label_w;
    QSlider *custom_slider_w;
    QSpinBox *custom_value_w;
    QLabel *custom_label_r_3;
    QWidget *custom_grid_hsl;
    QGridLayout *gridLayout_3;
    QLabel *custom_label_h;
    QSlider *custom_slider_h;
    QSpinBox *custom_value_h;
    QLabel *custom_label_s;
    QSlider *custom_slider_s;
    QSpinBox *custom_value_s;
    QLabel *custom_label_l;
    QSlider *custom_slider_l;
    QSpinBox *custom_value_l;
    QWidget *custom_grid_hex;
    QHBoxLayout *horizontalLayout_2;
    QLabel *custom_label_hex;
    QPlainTextEdit *custom_value_hex;
    QPushButton *custom_value_save;
    QPushButton *custom_value_load;
    QScrollArea *panel_lsettings;
    QWidget *panel_lsettings_content;
    QVBoxLayout *verticalLayout_16;
    QGroupBox *lsettings_td;
    QVBoxLayout *verticalLayout_14;
    QCheckBox *lsettings_td_12hr_enable;
    QCheckBox *lsettings_td_l0_enable;
    QCheckBox *lsettings_td_intldate_enable;
    QGroupBox *lsettings_nsh;
    QVBoxLayout *verticalLayout_15;
    QCheckBox *lsettings_nsh_enable;
    QCheckBox *lsettings_nsh_scheduled_enable;
    QWidget *lsettings_nsh_panel;
    QGridLayout *gridLayout_21;
    QLabel *lsettings_nsh_intro1;
    QTimeEdit *lsettings_nsh_start;
    QLabel *lsettings_nsh_intro2;
    QTimeEdit *lsettings_nsh_stop;
    QLabel *lsettings_nsh_intro3;
    QGroupBox *lsettings_settings;
    QVBoxLayout *verticalLayout_13;
    QPushButton *lsettings_settings_default;
    QPushButton *lsettings_settings_save;
    QGroupBox *lsettings_info;
    QGridLayout *gridLayout_20;
    QLabel *lsettings_info_hw_t;
    QLabel *lsettings_info_hw;
    QLabel *lsettings_info_fw_t;
    QLabel *lsettings_info_fw;
    QLabel *lsettings_info_app_t;
    QLabel *lsettings_info_app;
    QLabel *lsettings_info_notice;
    QPushButton *lsettings_info_download;
    QPushButton *lsettings_info_update;
    QScrollArea *panel_message;
    QWidget *panel_message_content;
    QVBoxLayout *verticalLayout;
    QLabel *message_intro;
    QPlainTextEdit *message_text;
    QLabel *message_intro_2;
    QLabel *message_intro_3;
    QLabel *message_intro_4;
    QLabel *message_intro_5;
    QLabel *message_intro_6;
    QCheckBox *message_set_custom_welcome;
    QLabel *message_intro_7;
    QPlainTextEdit *message_length;
    QPushButton *message_send;
    QScrollArea *panel_settings;
    QWidget *panel_settings_content;
    QVBoxLayout *verticalLayout_11;
    QGroupBox *settings_td;
    QGridLayout *gridLayout_17;
    QLabel *settings_td_timemode_t;
    QComboBox *settings_td_timemode;
    QLabel *settings_td_datemode_t;
    QComboBox *settings_td_datemode;
    QPushButton *settings_td_set;
    QGroupBox *settings_bri;
    QGridLayout *gridLayout_18;
    QLabel *settings_bri_t_disp;
    QComboBox *settings_bri_disp;
    QLabel *settings_bri_t_led;
    QComboBox *settings_bri_led;
    QPushButton *settings_bri_set;
    QGroupBox *settings_presets;
    QGridLayout *gridLayout_16;
    QCheckBox *settings_presets_en_static;
    QCheckBox *settings_presets_en_chase;
    QCheckBox *settings_presets_en_bliss;
    QCheckBox *settings_presets_en_tcode;
    QCheckBox *settings_presets_en_spectrum;
    QCheckBox *settings_presets_en_music;
    QCheckBox *settings_presets_en_rnb;
    QCheckBox *settings_presets_en_cop;
    QPushButton *settings_presets_en_set;
    QGroupBox *settings_nsh;
    QVBoxLayout *verticalLayout_5;
    QCheckBox *settings_nsh_enable;
    QWidget *settings_nsh_panel;
    QGridLayout *gridLayout_14;
    QLabel *settings_nsh_intro1;
    QTimeEdit *settings_nsh_start;
    QLabel *settings_nsh_intro2;
    QTimeEdit *settings_nsh_stop;
    QLabel *settings_nsh_intro3;
    QPushButton *settings_nsh_set;
    QGroupBox *settings_settings;
    QVBoxLayout *verticalLayout_10;
    QPushButton *settings_settings_default;
    QPushButton *settings_settings_save;
    QGroupBox *settings_info;
    QGridLayout *gridLayout_15;
    QLabel *settings_info_hw_t;
    QLabel *settings_info_hw;
    QLabel *settings_info_fw_t;
    QLabel *settings_info_fw;
    QLabel *settings_info_app_t;
    ClickableLabel *settings_info_app;
    QGroupBox *settings_update;
    QVBoxLayout *verticalLayout_9;
    QPushButton *settings_info_download;
    QPushButton *settings_info_update;
    QScrollArea *panel_timesync;
    QWidget *panel_timesync_content;
    QVBoxLayout *verticalLayout_12;
    ClickableLabel *timesync_time_label;
    QLabel *timesync_date_label;
    QWidget *timesync_layout;
    QGridLayout *gridLayout_19;
    QLabel *timesync_label_6;
    QSpinBox *timesync_p_y;
    QSpinBox *timesync_p_mo;
    QSpinBox *timesync_p_d;
    QSpinBox *timesync_p_h;
    QSpinBox *timesync_p_m;
    QLabel *timesync_label_7;
    QLabel *timesync_label;
    QLabel *timesync_label_2;
    QLabel *timesync_label_3;
    QLabel *timesync_label_4;
    QLabel *timesync_label_5;
    QLabel *timesync_intro;
    QPushButton *timesync_button;
    QLabel *timesync_intro_2;
    QScrollArea *panel_welcome;
    QWidget *panel_welcome_content;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_2;
    QLabel *com_intro;
    QComboBox *com_select;
    QWidget *com_control_dock;
    QHBoxLayout *horizontalLayout;
    ClickableLabel *com_reload;
    QWidget *widget;
    ClickableLabel *com_connect;
    QWidget *widget_3;
    QLabel *com_label_connect;

    void setupUi(QMainWindow *FluorescenceApp)
    {
        if (FluorescenceApp->objectName().isEmpty())
            FluorescenceApp->setObjectName(QString::fromUtf8("FluorescenceApp"));
        FluorescenceApp->resize(795, 2551);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        FluorescenceApp->setFont(font);
        FluorescenceApp->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        FluorescenceApp->setToolButtonStyle(Qt::ToolButtonIconOnly);
        FluorescenceApp->setDockNestingEnabled(false);
        centralWidget = new QWidget(FluorescenceApp);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_18 = new QVBoxLayout(centralWidget);
        verticalLayout_18->setSpacing(0);
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        verticalLayout_18->setContentsMargins(0, 0, 0, 0);
        menu_layout = new QWidget(centralWidget);
        menu_layout->setObjectName(QString::fromUtf8("menu_layout"));
        menu_hide = new QHBoxLayout(menu_layout);
        menu_hide->setSpacing(0);
        menu_hide->setObjectName(QString::fromUtf8("menu_hide"));
        menu_hide->setSizeConstraint(QLayout::SetNoConstraint);
        menu_hide->setContentsMargins(10, 0, 10, 0);
        menu_button = new ClickableLabel(menu_layout);
        menu_button->setObjectName(QString::fromUtf8("menu_button"));
        menu_button->setEnabled(false);
        menu_button->setMinimumSize(QSize(46, 40));
        menu_button->setMaximumSize(QSize(46, 40));
        menu_button->setPixmap(QPixmap(QString::fromUtf8(":/Resources/menu.png")));
        menu_button->setScaledContents(true);

        menu_hide->addWidget(menu_button);

        menu_about = new QLabel(menu_layout);
        menu_about->setObjectName(QString::fromUtf8("menu_about"));
        menu_about->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 18px \"RalewayBlack\";"));
        menu_about->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        menu_hide->addWidget(menu_about);

        menu_about_2 = new QLabel(menu_layout);
        menu_about_2->setObjectName(QString::fromUtf8("menu_about_2"));
        menu_about_2->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayMedium\";"));
        menu_about_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        menu_about_2->setMargin(0);

        menu_hide->addWidget(menu_about_2);

        menu_button_balancer = new ClickableLabel(menu_layout);
        menu_button_balancer->setObjectName(QString::fromUtf8("menu_button_balancer"));
        menu_button_balancer->setMinimumSize(QSize(39, 0));
        menu_button_balancer->setMaximumSize(QSize(40, 16777215));
        menu_button_balancer->setScaledContents(false);

        menu_hide->addWidget(menu_button_balancer);


        verticalLayout_18->addWidget(menu_layout);

        parent_menu_control = new QWidget(centralWidget);
        parent_menu_control->setObjectName(QString::fromUtf8("parent_menu_control"));
        horizontalLayout_9 = new QHBoxLayout(parent_menu_control);
        horizontalLayout_9->setSpacing(0);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        menu_controls_layout = new QWidget(parent_menu_control);
        menu_controls_layout->setObjectName(QString::fromUtf8("menu_controls_layout"));
        menu_controls_layout->setMinimumSize(QSize(0, 0));
        menu_controls_layout->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));
        menu_vlayout = new QVBoxLayout(menu_controls_layout);
        menu_vlayout->setSpacing(0);
        menu_vlayout->setObjectName(QString::fromUtf8("menu_vlayout"));
        menu_vlayout->setContentsMargins(10, 0, 10, 20);
        main_welcome = new QPushButton(menu_controls_layout);
        main_welcome->setObjectName(QString::fromUtf8("main_welcome"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("RalewayBold"));
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        main_welcome->setFont(font1);
        main_welcome->setAutoFillBackground(false);
        main_welcome->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 32px \"RalewayBold\";"));
        main_welcome->setAutoDefault(false);
        main_welcome->setFlat(true);

        menu_vlayout->addWidget(main_welcome);

        main_timesync = new QPushButton(menu_controls_layout);
        main_timesync->setObjectName(QString::fromUtf8("main_timesync"));
        main_timesync->setFont(font1);
        main_timesync->setAutoFillBackground(false);
        main_timesync->setStyleSheet(QString::fromUtf8("background-color:  rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 32px \"RalewayBold\";"));
        main_timesync->setAutoDefault(false);
        main_timesync->setFlat(true);

        menu_vlayout->addWidget(main_timesync);

        main_preset = new QPushButton(menu_controls_layout);
        main_preset->setObjectName(QString::fromUtf8("main_preset"));
        main_preset->setFont(font1);
        main_preset->setAutoFillBackground(false);
        main_preset->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 32px \"RalewayBold\";"));
        main_preset->setAutoDefault(false);
        main_preset->setFlat(true);

        menu_vlayout->addWidget(main_preset);

        main_custom = new QPushButton(menu_controls_layout);
        main_custom->setObjectName(QString::fromUtf8("main_custom"));
        main_custom->setFont(font1);
        main_custom->setAutoFillBackground(false);
        main_custom->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 32px \"RalewayBold\";"));
        main_custom->setFlat(true);

        menu_vlayout->addWidget(main_custom);

        main_settings = new QPushButton(menu_controls_layout);
        main_settings->setObjectName(QString::fromUtf8("main_settings"));
        main_settings->setFont(font1);
        main_settings->setAutoFillBackground(false);
        main_settings->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 32px \"RalewayBold\";"));
        main_settings->setAutoDefault(false);
        main_settings->setFlat(true);

        menu_vlayout->addWidget(main_settings);

        main_message = new QPushButton(menu_controls_layout);
        main_message->setObjectName(QString::fromUtf8("main_message"));
        main_message->setFont(font1);
        main_message->setAutoFillBackground(false);
        main_message->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 32px \"RalewayBold\";"));
        main_message->setAutoDefault(false);
        main_message->setFlat(true);

        menu_vlayout->addWidget(main_message);


        horizontalLayout_9->addWidget(menu_controls_layout);

        control_parent = new QWidget(parent_menu_control);
        control_parent->setObjectName(QString::fromUtf8("control_parent"));
        control_parent->setMinimumSize(QSize(450, 400));
        verticalLayout_17 = new QVBoxLayout(control_parent);
        verticalLayout_17->setSpacing(0);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        verticalLayout_17->setContentsMargins(0, 0, 0, 0);
        panel_presets = new QScrollArea(control_parent);
        panel_presets->setObjectName(QString::fromUtf8("panel_presets"));
        panel_presets->setMinimumSize(QSize(450, 0));
        panel_presets->setFrameShape(QFrame::NoFrame);
        panel_presets->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        panel_presets->setWidgetResizable(true);
        panel_presets_content = new QWidget();
        panel_presets_content->setObjectName(QString::fromUtf8("panel_presets_content"));
        panel_presets_content->setGeometry(QRect(0, 0, 545, 375));
        verticalLayout_4 = new QVBoxLayout(panel_presets_content);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        tab_presets_container = new QTabWidget(panel_presets_content);
        tab_presets_container->setObjectName(QString::fromUtf8("tab_presets_container"));
        tab_presets_container->setFont(font1);
        tab_presets_container->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        tab_presets_static = new QWidget();
        tab_presets_static->setObjectName(QString::fromUtf8("tab_presets_static"));
        verticalLayout_8 = new QVBoxLayout(tab_presets_static);
        verticalLayout_8->setSpacing(20);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(12, 12, 12, 12);
        scroll_presets_static = new QScrollArea(tab_presets_static);
        scroll_presets_static->setObjectName(QString::fromUtf8("scroll_presets_static"));
        scroll_presets_static->setFrameShape(QFrame::NoFrame);
        scroll_presets_static->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll_presets_static->setWidgetResizable(true);
        scroll_presets_static_content = new QWidget();
        scroll_presets_static_content->setObjectName(QString::fromUtf8("scroll_presets_static_content"));
        scroll_presets_static_content->setGeometry(QRect(0, 0, 515, 1059));
        verticalLayout_19 = new QVBoxLayout(scroll_presets_static_content);
        verticalLayout_19->setSpacing(30);
        verticalLayout_19->setObjectName(QString::fromUtf8("verticalLayout_19"));
        verticalLayout_19->setContentsMargins(0, 15, 0, 30);
        static_intro = new QLabel(scroll_presets_static_content);
        static_intro->setObjectName(QString::fromUtf8("static_intro"));
        static_intro->setFont(font1);
        static_intro->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        static_intro->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_intro);

        static_off = new ClickableLabel(scroll_presets_static_content);
        static_off->setObjectName(QString::fromUtf8("static_off"));
        static_off->setFont(font1);
        static_off->setCursor(QCursor(Qt::OpenHandCursor));
        static_off->setStyleSheet(QString::fromUtf8("color: rgb(51, 51, 51);\n"
"font: 26px \"RalewayBold\";"));
        static_off->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_off);

        static_white = new ClickableLabel(scroll_presets_static_content);
        static_white->setObjectName(QString::fromUtf8("static_white"));
        static_white->setFont(font1);
        static_white->setCursor(QCursor(Qt::OpenHandCursor));
        static_white->setStyleSheet(QString::fromUtf8("color: rgb(204, 204, 204);font: 26px \"RalewayBold\";"));
        static_white->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_white);

        static_warmwhite = new ClickableLabel(scroll_presets_static_content);
        static_warmwhite->setObjectName(QString::fromUtf8("static_warmwhite"));
        static_warmwhite->setFont(font1);
        static_warmwhite->setCursor(QCursor(Qt::OpenHandCursor));
        static_warmwhite->setStyleSheet(QString::fromUtf8("color: rgb(254, 204, 102);font: 26px \"RalewayBold\";"));
        static_warmwhite->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_warmwhite);

        static_rainbow = new ClickableLabel(scroll_presets_static_content);
        static_rainbow->setObjectName(QString::fromUtf8("static_rainbow"));
        static_rainbow->setCursor(QCursor(Qt::OpenHandCursor));
        static_rainbow->setPixmap(QPixmap(QString::fromUtf8(":/Resources/rainbow_alt.png")));
        static_rainbow->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_rainbow);

        static_pastelrainbow = new ClickableLabel(scroll_presets_static_content);
        static_pastelrainbow->setObjectName(QString::fromUtf8("static_pastelrainbow"));
        static_pastelrainbow->setCursor(QCursor(Qt::OpenHandCursor));
        static_pastelrainbow->setPixmap(QPixmap(QString::fromUtf8(":/Resources/pastelrainbow.png")));
        static_pastelrainbow->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_pastelrainbow);

        static_red2blue = new ClickableLabel(scroll_presets_static_content);
        static_red2blue->setObjectName(QString::fromUtf8("static_red2blue"));
        static_red2blue->setCursor(QCursor(Qt::OpenHandCursor));
        static_red2blue->setPixmap(QPixmap(QString::fromUtf8(":/Resources/red2blue1.png")));
        static_red2blue->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_red2blue);

        static_red2green = new ClickableLabel(scroll_presets_static_content);
        static_red2green->setObjectName(QString::fromUtf8("static_red2green"));
        static_red2green->setCursor(QCursor(Qt::OpenHandCursor));
        static_red2green->setPixmap(QPixmap(QString::fromUtf8(":/Resources/red2green_alt.png")));
        static_red2green->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_red2green);

        static_green2blue = new ClickableLabel(scroll_presets_static_content);
        static_green2blue->setObjectName(QString::fromUtf8("static_green2blue"));
        static_green2blue->setCursor(QCursor(Qt::OpenHandCursor));
        static_green2blue->setPixmap(QPixmap(QString::fromUtf8(":/Resources/green2blue.png")));
        static_green2blue->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_green2blue);

        static_blue = new ClickableLabel(scroll_presets_static_content);
        static_blue->setObjectName(QString::fromUtf8("static_blue"));
        static_blue->setFont(font1);
        static_blue->setCursor(QCursor(Qt::OpenHandCursor));
        static_blue->setStyleSheet(QString::fromUtf8("font: 26px \"RalewayBold\";\n"
"color: rgb(0, 84, 147);"));
        static_blue->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_blue);

        static_cyan = new ClickableLabel(scroll_presets_static_content);
        static_cyan->setObjectName(QString::fromUtf8("static_cyan"));
        static_cyan->setFont(font1);
        static_cyan->setCursor(QCursor(Qt::OpenHandCursor));
        static_cyan->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 26px \"RalewayBold\";"));
        static_cyan->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_cyan);

        static_green = new ClickableLabel(scroll_presets_static_content);
        static_green->setObjectName(QString::fromUtf8("static_green"));
        static_green->setFont(font1);
        static_green->setCursor(QCursor(Qt::OpenHandCursor));
        static_green->setStyleSheet(QString::fromUtf8("font: 26px \"RalewayBold\";\n"
"color: rgb(0, 143, 0);"));
        static_green->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_green);

        static_yellow = new ClickableLabel(scroll_presets_static_content);
        static_yellow->setObjectName(QString::fromUtf8("static_yellow"));
        static_yellow->setFont(font1);
        static_yellow->setCursor(QCursor(Qt::OpenHandCursor));
        static_yellow->setStyleSheet(QString::fromUtf8("color: rgb(185, 161, 29);font: 26px \"RalewayBold\";"));
        static_yellow->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_yellow);

        static_orange = new ClickableLabel(scroll_presets_static_content);
        static_orange->setObjectName(QString::fromUtf8("static_orange"));
        static_orange->setFont(font1);
        static_orange->setCursor(QCursor(Qt::OpenHandCursor));
        static_orange->setStyleSheet(QString::fromUtf8("font: 26px \"RalewayBold\";\n"
"color: rgb(206, 114, 0);"));
        static_orange->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_orange);

        static_red = new ClickableLabel(scroll_presets_static_content);
        static_red->setObjectName(QString::fromUtf8("static_red"));
        static_red->setFont(font1);
        static_red->setCursor(QCursor(Qt::OpenHandCursor));
        static_red->setStyleSheet(QString::fromUtf8("font: 26px \"RalewayBold\";\n"
"color: rgb(148, 17, 0);"));
        static_red->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_red);

        static_magenta = new ClickableLabel(scroll_presets_static_content);
        static_magenta->setObjectName(QString::fromUtf8("static_magenta"));
        static_magenta->setFont(font1);
        static_magenta->setCursor(QCursor(Qt::OpenHandCursor));
        static_magenta->setStyleSheet(QString::fromUtf8("font: 26px \"RalewayBold\";\n"
"color: rgb(148, 23, 81);"));
        static_magenta->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_magenta);

        static_violet = new ClickableLabel(scroll_presets_static_content);
        static_violet->setObjectName(QString::fromUtf8("static_violet"));
        static_violet->setFont(font1);
        static_violet->setCursor(QCursor(Qt::OpenHandCursor));
        static_violet->setStyleSheet(QString::fromUtf8("font: 26px \"RalewayBold\";\n"
"color: rgb(83, 27, 147);"));
        static_violet->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(static_violet);

        scroll_presets_static->setWidget(scroll_presets_static_content);

        verticalLayout_8->addWidget(scroll_presets_static);

        tab_presets_container->addTab(tab_presets_static, QString());
        tab_presets_dynamic = new QWidget();
        tab_presets_dynamic->setObjectName(QString::fromUtf8("tab_presets_dynamic"));
        horizontalLayout_6 = new QHBoxLayout(tab_presets_dynamic);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        scroll_presets_dynamic = new QScrollArea(tab_presets_dynamic);
        scroll_presets_dynamic->setObjectName(QString::fromUtf8("scroll_presets_dynamic"));
        scroll_presets_dynamic->setFrameShape(QFrame::NoFrame);
        scroll_presets_dynamic->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll_presets_dynamic->setWidgetResizable(true);
        scroll_presets_dynamic->setAlignment(Qt::AlignCenter);
        tab_presets_dynamic_content = new QWidget();
        tab_presets_dynamic_content->setObjectName(QString::fromUtf8("tab_presets_dynamic_content"));
        tab_presets_dynamic_content->setGeometry(QRect(0, 0, 539, 1848));
        verticalLayout_6 = new QVBoxLayout(tab_presets_dynamic_content);
        verticalLayout_6->setSpacing(10);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(20, 30, 20, 30);
        panel_presets_control_grid = new QWidget(tab_presets_dynamic_content);
        panel_presets_control_grid->setObjectName(QString::fromUtf8("panel_presets_control_grid"));
        gridLayout_11 = new QGridLayout(panel_presets_control_grid);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        gridLayout_11->setHorizontalSpacing(20);
        gridLayout_11->setVerticalSpacing(30);
        gridLayout_11->setContentsMargins(-1, -1, -1, 30);
        dynamic_bliss_dnc = new QPushButton(panel_presets_control_grid);
        dynamic_bliss_dnc->setObjectName(QString::fromUtf8("dynamic_bliss_dnc"));
        dynamic_bliss_dnc->setMinimumSize(QSize(0, 40));
        dynamic_bliss_dnc->setFont(font1);
        dynamic_bliss_dnc->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_11->addWidget(dynamic_bliss_dnc, 0, 0, 1, 3);

        dynamic_spectrum = new QPushButton(panel_presets_control_grid);
        dynamic_spectrum->setObjectName(QString::fromUtf8("dynamic_spectrum"));
        dynamic_spectrum->setMinimumSize(QSize(0, 40));
        dynamic_spectrum->setFont(font1);
        dynamic_spectrum->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_11->addWidget(dynamic_spectrum, 1, 0, 1, 1);

        dynamic_rainbow = new QPushButton(panel_presets_control_grid);
        dynamic_rainbow->setObjectName(QString::fromUtf8("dynamic_rainbow"));
        dynamic_rainbow->setMinimumSize(QSize(0, 40));
        dynamic_rainbow->setFont(font1);
        dynamic_rainbow->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_11->addWidget(dynamic_rainbow, 1, 1, 1, 1);

        dynamic_chase = new QPushButton(panel_presets_control_grid);
        dynamic_chase->setObjectName(QString::fromUtf8("dynamic_chase"));
        dynamic_chase->setMinimumSize(QSize(0, 40));
        dynamic_chase->setFont(font1);
        dynamic_chase->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_11->addWidget(dynamic_chase, 1, 2, 1, 1);

        dynamic_music = new QPushButton(panel_presets_control_grid);
        dynamic_music->setObjectName(QString::fromUtf8("dynamic_music"));
        dynamic_music->setMinimumSize(QSize(0, 40));
        dynamic_music->setFont(font1);
        dynamic_music->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_11->addWidget(dynamic_music, 2, 0, 1, 1);

        dynamic_timecode = new QPushButton(panel_presets_control_grid);
        dynamic_timecode->setObjectName(QString::fromUtf8("dynamic_timecode"));
        dynamic_timecode->setMinimumSize(QSize(0, 40));
        dynamic_timecode->setFont(font1);
        dynamic_timecode->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_11->addWidget(dynamic_timecode, 2, 1, 1, 1);

        dynamic_police = new QPushButton(panel_presets_control_grid);
        dynamic_police->setObjectName(QString::fromUtf8("dynamic_police"));
        dynamic_police->setMinimumSize(QSize(0, 40));
        dynamic_police->setFont(font1);
        dynamic_police->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_11->addWidget(dynamic_police, 2, 2, 1, 1);


        verticalLayout_6->addWidget(panel_presets_control_grid);

        panel_dyn_rnb = new QWidget(tab_presets_dynamic_content);
        panel_dyn_rnb->setObjectName(QString::fromUtf8("panel_dyn_rnb"));
        panel_dyn_rnb->setFont(font1);
        gridLayout_10 = new QGridLayout(panel_dyn_rnb);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_10->setVerticalSpacing(20);
        panel_dyn_rnb_ptext_1 = new QLabel(panel_dyn_rnb);
        panel_dyn_rnb_ptext_1->setObjectName(QString::fromUtf8("panel_dyn_rnb_ptext_1"));
        panel_dyn_rnb_ptext_1->setFont(font1);
        panel_dyn_rnb_ptext_1->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 26px \"RalewayBold\";"));

        gridLayout_10->addWidget(panel_dyn_rnb_ptext_1, 0, 0, 1, 1);

        panel_dyn_rnb_p1_1 = new QLabel(panel_dyn_rnb);
        panel_dyn_rnb_p1_1->setObjectName(QString::fromUtf8("panel_dyn_rnb_p1_1"));
        panel_dyn_rnb_p1_1->setFont(font1);
        panel_dyn_rnb_p1_1->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        panel_dyn_rnb_p1_1->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_10->addWidget(panel_dyn_rnb_p1_1, 0, 1, 1, 1);

        panel_dyn_rnb_param1 = new QSlider(panel_dyn_rnb);
        panel_dyn_rnb_param1->setObjectName(QString::fromUtf8("panel_dyn_rnb_param1"));
        panel_dyn_rnb_param1->setFont(font1);
        panel_dyn_rnb_param1->setMinimum(10);
        panel_dyn_rnb_param1->setMaximum(42);
        panel_dyn_rnb_param1->setValue(18);
        panel_dyn_rnb_param1->setTracking(true);
        panel_dyn_rnb_param1->setOrientation(Qt::Horizontal);
        panel_dyn_rnb_param1->setTickPosition(QSlider::TicksBelow);

        gridLayout_10->addWidget(panel_dyn_rnb_param1, 0, 2, 1, 1);

        panel_dyn_rnb_p1_2 = new QLabel(panel_dyn_rnb);
        panel_dyn_rnb_p1_2->setObjectName(QString::fromUtf8("panel_dyn_rnb_p1_2"));
        panel_dyn_rnb_p1_2->setFont(font1);
        panel_dyn_rnb_p1_2->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        panel_dyn_rnb_p1_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_10->addWidget(panel_dyn_rnb_p1_2, 0, 3, 1, 1);

        panel_dyn_rnb_ptext_2 = new QLabel(panel_dyn_rnb);
        panel_dyn_rnb_ptext_2->setObjectName(QString::fromUtf8("panel_dyn_rnb_ptext_2"));
        panel_dyn_rnb_ptext_2->setFont(font1);
        panel_dyn_rnb_ptext_2->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 26px \"RalewayBold\";"));

        gridLayout_10->addWidget(panel_dyn_rnb_ptext_2, 1, 0, 1, 1);

        panel_dyn_rnb_param2 = new QComboBox(panel_dyn_rnb);
        panel_dyn_rnb_param2->addItem(QString());
        panel_dyn_rnb_param2->addItem(QString());
        panel_dyn_rnb_param2->addItem(QString());
        panel_dyn_rnb_param2->setObjectName(QString::fromUtf8("panel_dyn_rnb_param2"));
        panel_dyn_rnb_param2->setMinimumSize(QSize(0, 45));
        panel_dyn_rnb_param2->setFont(font1);
        panel_dyn_rnb_param2->setStyleSheet(QString::fromUtf8("font: 18px \"RalewayBold\";"));
        panel_dyn_rnb_param2->setFrame(false);

        gridLayout_10->addWidget(panel_dyn_rnb_param2, 1, 1, 1, 3);

        panel_dyn_rnb_set = new QPushButton(panel_dyn_rnb);
        panel_dyn_rnb_set->setObjectName(QString::fromUtf8("panel_dyn_rnb_set"));
        panel_dyn_rnb_set->setMinimumSize(QSize(0, 60));
        QFont font2;
        font2.setFamily(QString::fromUtf8("RalewayBlack"));
        font2.setBold(false);
        font2.setItalic(false);
        font2.setWeight(50);
        panel_dyn_rnb_set->setFont(font2);
        panel_dyn_rnb_set->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 26px \"RalewayBlack\";"));

        gridLayout_10->addWidget(panel_dyn_rnb_set, 2, 0, 1, 4);


        verticalLayout_6->addWidget(panel_dyn_rnb);

        panel_dyn_spectrum = new QWidget(tab_presets_dynamic_content);
        panel_dyn_spectrum->setObjectName(QString::fromUtf8("panel_dyn_spectrum"));
        panel_dyn_spectrum->setFont(font1);
        gridLayout_6 = new QGridLayout(panel_dyn_spectrum);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setVerticalSpacing(20);
        panel_dyn_spectrum_ptext1 = new QLabel(panel_dyn_spectrum);
        panel_dyn_spectrum_ptext1->setObjectName(QString::fromUtf8("panel_dyn_spectrum_ptext1"));
        panel_dyn_spectrum_ptext1->setFont(font1);
        panel_dyn_spectrum_ptext1->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 26px \"RalewayBold\";"));

        gridLayout_6->addWidget(panel_dyn_spectrum_ptext1, 1, 0, 1, 1);

        panel_dyn_spectrum_set = new QPushButton(panel_dyn_spectrum);
        panel_dyn_spectrum_set->setObjectName(QString::fromUtf8("panel_dyn_spectrum_set"));
        panel_dyn_spectrum_set->setMinimumSize(QSize(0, 60));
        panel_dyn_spectrum_set->setFont(font2);
        panel_dyn_spectrum_set->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 26px \"RalewayBlack\";"));

        gridLayout_6->addWidget(panel_dyn_spectrum_set, 4, 0, 1, 2);

        panel_dyn_spectrum_ptext2 = new QLabel(panel_dyn_spectrum);
        panel_dyn_spectrum_ptext2->setObjectName(QString::fromUtf8("panel_dyn_spectrum_ptext2"));
        panel_dyn_spectrum_ptext2->setFont(font1);
        panel_dyn_spectrum_ptext2->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 26px \"RalewayBold\";"));

        gridLayout_6->addWidget(panel_dyn_spectrum_ptext2, 2, 0, 1, 1);

        panel_dyn_spectrum_param_2 = new QComboBox(panel_dyn_spectrum);
        panel_dyn_spectrum_param_2->addItem(QString());
        panel_dyn_spectrum_param_2->addItem(QString());
        panel_dyn_spectrum_param_2->addItem(QString());
        panel_dyn_spectrum_param_2->setObjectName(QString::fromUtf8("panel_dyn_spectrum_param_2"));
        panel_dyn_spectrum_param_2->setMinimumSize(QSize(0, 45));
        panel_dyn_spectrum_param_2->setFont(font1);
        panel_dyn_spectrum_param_2->setStyleSheet(QString::fromUtf8("font: 18px \"RalewayBold\";"));
        panel_dyn_spectrum_param_2->setFrame(false);

        gridLayout_6->addWidget(panel_dyn_spectrum_param_2, 2, 1, 1, 1);

        panel_dyn_spectrum_intro = new QLabel(panel_dyn_spectrum);
        panel_dyn_spectrum_intro->setObjectName(QString::fromUtf8("panel_dyn_spectrum_intro"));
        panel_dyn_spectrum_intro->setFont(font1);
        panel_dyn_spectrum_intro->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        panel_dyn_spectrum_intro->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        gridLayout_6->addWidget(panel_dyn_spectrum_intro, 0, 0, 1, 2);

        panel_dyn_spectrum_param_1 = new QComboBox(panel_dyn_spectrum);
        panel_dyn_spectrum_param_1->addItem(QString());
        panel_dyn_spectrum_param_1->addItem(QString());
        panel_dyn_spectrum_param_1->addItem(QString());
        panel_dyn_spectrum_param_1->setObjectName(QString::fromUtf8("panel_dyn_spectrum_param_1"));
        panel_dyn_spectrum_param_1->setMinimumSize(QSize(0, 45));
        panel_dyn_spectrum_param_1->setFont(font1);
        panel_dyn_spectrum_param_1->setStyleSheet(QString::fromUtf8("font: 18px \"RalewayBold\";"));
        panel_dyn_spectrum_param_1->setFrame(false);

        gridLayout_6->addWidget(panel_dyn_spectrum_param_1, 1, 1, 1, 1);


        verticalLayout_6->addWidget(panel_dyn_spectrum);

        panel_dyn_chase = new QWidget(tab_presets_dynamic_content);
        panel_dyn_chase->setObjectName(QString::fromUtf8("panel_dyn_chase"));
        panel_dyn_chase->setFont(font1);
        gridLayout_9 = new QGridLayout(panel_dyn_chase);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_9->setVerticalSpacing(20);
        panel_dyn_chase_ptext_1 = new QLabel(panel_dyn_chase);
        panel_dyn_chase_ptext_1->setObjectName(QString::fromUtf8("panel_dyn_chase_ptext_1"));
        panel_dyn_chase_ptext_1->setFont(font1);
        panel_dyn_chase_ptext_1->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 26px \"RalewayBold\";"));

        gridLayout_9->addWidget(panel_dyn_chase_ptext_1, 0, 0, 1, 1);

        panel_dyn_chase_p1 = new QLabel(panel_dyn_chase);
        panel_dyn_chase_p1->setObjectName(QString::fromUtf8("panel_dyn_chase_p1"));
        panel_dyn_chase_p1->setFont(font1);
        panel_dyn_chase_p1->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        panel_dyn_chase_p1->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_9->addWidget(panel_dyn_chase_p1, 0, 1, 1, 1);

        panel_dyn_chase_param1 = new QSlider(panel_dyn_chase);
        panel_dyn_chase_param1->setObjectName(QString::fromUtf8("panel_dyn_chase_param1"));
        panel_dyn_chase_param1->setFont(font1);
        panel_dyn_chase_param1->setMinimum(0);
        panel_dyn_chase_param1->setMaximum(6);
        panel_dyn_chase_param1->setValue(0);
        panel_dyn_chase_param1->setTracking(true);
        panel_dyn_chase_param1->setOrientation(Qt::Horizontal);
        panel_dyn_chase_param1->setTickPosition(QSlider::TicksBelow);

        gridLayout_9->addWidget(panel_dyn_chase_param1, 0, 2, 1, 1);

        panel_dyn_chase_p2 = new QLabel(panel_dyn_chase);
        panel_dyn_chase_p2->setObjectName(QString::fromUtf8("panel_dyn_chase_p2"));
        panel_dyn_chase_p2->setFont(font1);
        panel_dyn_chase_p2->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        panel_dyn_chase_p2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_9->addWidget(panel_dyn_chase_p2, 0, 3, 1, 1);

        panel_dyn_chase_ptext_2 = new QLabel(panel_dyn_chase);
        panel_dyn_chase_ptext_2->setObjectName(QString::fromUtf8("panel_dyn_chase_ptext_2"));
        panel_dyn_chase_ptext_2->setFont(font1);
        panel_dyn_chase_ptext_2->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 26px \"RalewayBold\";"));

        gridLayout_9->addWidget(panel_dyn_chase_ptext_2, 1, 0, 1, 1);

        panel_dyn_chase_param2 = new QComboBox(panel_dyn_chase);
        panel_dyn_chase_param2->addItem(QString());
        panel_dyn_chase_param2->addItem(QString());
        panel_dyn_chase_param2->addItem(QString());
        panel_dyn_chase_param2->addItem(QString());
        panel_dyn_chase_param2->setObjectName(QString::fromUtf8("panel_dyn_chase_param2"));
        panel_dyn_chase_param2->setMinimumSize(QSize(0, 45));
        panel_dyn_chase_param2->setFont(font1);
        panel_dyn_chase_param2->setStyleSheet(QString::fromUtf8("font: 18px \"RalewayBold\";"));
        panel_dyn_chase_param2->setFrame(false);

        gridLayout_9->addWidget(panel_dyn_chase_param2, 1, 1, 1, 3);

        panel_dyn_chase_set = new QPushButton(panel_dyn_chase);
        panel_dyn_chase_set->setObjectName(QString::fromUtf8("panel_dyn_chase_set"));
        panel_dyn_chase_set->setMinimumSize(QSize(0, 60));
        panel_dyn_chase_set->setFont(font2);
        panel_dyn_chase_set->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 26px \"RalewayBlack\";"));

        gridLayout_9->addWidget(panel_dyn_chase_set, 2, 0, 1, 4);


        verticalLayout_6->addWidget(panel_dyn_chase);

        panel_dyn_tcode = new QWidget(tab_presets_dynamic_content);
        panel_dyn_tcode->setObjectName(QString::fromUtf8("panel_dyn_tcode"));
        panel_dyn_tcode->setFont(font1);
        gridLayout_4 = new QGridLayout(panel_dyn_tcode);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setVerticalSpacing(20);
        panel_dyn_tcode_intro = new QLabel(panel_dyn_tcode);
        panel_dyn_tcode_intro->setObjectName(QString::fromUtf8("panel_dyn_tcode_intro"));
        panel_dyn_tcode_intro->setFont(font1);
        panel_dyn_tcode_intro->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        panel_dyn_tcode_intro->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        gridLayout_4->addWidget(panel_dyn_tcode_intro, 0, 0, 1, 2);

        panel_dyn_tcode_ptext = new QLabel(panel_dyn_tcode);
        panel_dyn_tcode_ptext->setObjectName(QString::fromUtf8("panel_dyn_tcode_ptext"));
        panel_dyn_tcode_ptext->setFont(font1);
        panel_dyn_tcode_ptext->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 26px \"RalewayBold\";"));

        gridLayout_4->addWidget(panel_dyn_tcode_ptext, 1, 0, 1, 1);

        panel_dyn_tcode_param1 = new QComboBox(panel_dyn_tcode);
        panel_dyn_tcode_param1->addItem(QString());
        panel_dyn_tcode_param1->addItem(QString());
        panel_dyn_tcode_param1->setObjectName(QString::fromUtf8("panel_dyn_tcode_param1"));
        panel_dyn_tcode_param1->setMinimumSize(QSize(0, 45));
        panel_dyn_tcode_param1->setFont(font1);
        panel_dyn_tcode_param1->setStyleSheet(QString::fromUtf8("font: 18px \"RalewayBold\";"));
        panel_dyn_tcode_param1->setFrame(false);

        gridLayout_4->addWidget(panel_dyn_tcode_param1, 1, 1, 1, 1);

        panel_dyn_tcode_set = new QPushButton(panel_dyn_tcode);
        panel_dyn_tcode_set->setObjectName(QString::fromUtf8("panel_dyn_tcode_set"));
        panel_dyn_tcode_set->setMinimumSize(QSize(0, 60));
        panel_dyn_tcode_set->setFont(font2);
        panel_dyn_tcode_set->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 26px \"RalewayBlack\";"));

        gridLayout_4->addWidget(panel_dyn_tcode_set, 2, 0, 1, 2);


        verticalLayout_6->addWidget(panel_dyn_tcode);

        panel_dyn_music = new QWidget(tab_presets_dynamic_content);
        panel_dyn_music->setObjectName(QString::fromUtf8("panel_dyn_music"));
        panel_dyn_music->setFont(font1);
        gridLayout_8 = new QGridLayout(panel_dyn_music);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_8->setVerticalSpacing(20);
        panel_dyn_music_ptext_1 = new QLabel(panel_dyn_music);
        panel_dyn_music_ptext_1->setObjectName(QString::fromUtf8("panel_dyn_music_ptext_1"));
        panel_dyn_music_ptext_1->setFont(font1);
        panel_dyn_music_ptext_1->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 26px \"RalewayBold\";"));

        gridLayout_8->addWidget(panel_dyn_music_ptext_1, 0, 0, 1, 1);

        panel_dyn_music_p2 = new QLabel(panel_dyn_music);
        panel_dyn_music_p2->setObjectName(QString::fromUtf8("panel_dyn_music_p2"));
        panel_dyn_music_p2->setFont(font1);
        panel_dyn_music_p2->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        panel_dyn_music_p2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_8->addWidget(panel_dyn_music_p2, 0, 3, 1, 1);

        panel_dyn_music_set = new QPushButton(panel_dyn_music);
        panel_dyn_music_set->setObjectName(QString::fromUtf8("panel_dyn_music_set"));
        panel_dyn_music_set->setMinimumSize(QSize(0, 60));
        panel_dyn_music_set->setFont(font2);
        panel_dyn_music_set->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 26px \"RalewayBlack\";"));

        gridLayout_8->addWidget(panel_dyn_music_set, 2, 0, 1, 4);

        panel_dyn_music_p1 = new QLabel(panel_dyn_music);
        panel_dyn_music_p1->setObjectName(QString::fromUtf8("panel_dyn_music_p1"));
        panel_dyn_music_p1->setFont(font1);
        panel_dyn_music_p1->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        panel_dyn_music_p1->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_8->addWidget(panel_dyn_music_p1, 0, 1, 1, 1);

        panel_dyn_music_param2 = new QComboBox(panel_dyn_music);
        panel_dyn_music_param2->addItem(QString());
        panel_dyn_music_param2->addItem(QString());
        panel_dyn_music_param2->addItem(QString());
        panel_dyn_music_param2->setObjectName(QString::fromUtf8("panel_dyn_music_param2"));
        panel_dyn_music_param2->setMinimumSize(QSize(0, 45));
        panel_dyn_music_param2->setFont(font1);
        panel_dyn_music_param2->setStyleSheet(QString::fromUtf8("font: 18px \"RalewayBold\";"));
        panel_dyn_music_param2->setFrame(false);

        gridLayout_8->addWidget(panel_dyn_music_param2, 1, 1, 1, 3);

        panel_dyn_music_param1 = new QSlider(panel_dyn_music);
        panel_dyn_music_param1->setObjectName(QString::fromUtf8("panel_dyn_music_param1"));
        panel_dyn_music_param1->setFont(font1);
        panel_dyn_music_param1->setMinimum(10);
        panel_dyn_music_param1->setMaximum(42);
        panel_dyn_music_param1->setValue(18);
        panel_dyn_music_param1->setTracking(true);
        panel_dyn_music_param1->setOrientation(Qt::Horizontal);
        panel_dyn_music_param1->setTickPosition(QSlider::TicksBelow);

        gridLayout_8->addWidget(panel_dyn_music_param1, 0, 2, 1, 1);

        panel_dyn_music_ptext_2 = new QLabel(panel_dyn_music);
        panel_dyn_music_ptext_2->setObjectName(QString::fromUtf8("panel_dyn_music_ptext_2"));
        panel_dyn_music_ptext_2->setFont(font1);
        panel_dyn_music_ptext_2->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 26px \"RalewayBold\";"));

        gridLayout_8->addWidget(panel_dyn_music_ptext_2, 1, 0, 1, 1);


        verticalLayout_6->addWidget(panel_dyn_music);

        panel_dyn_cop = new QWidget(tab_presets_dynamic_content);
        panel_dyn_cop->setObjectName(QString::fromUtf8("panel_dyn_cop"));
        panel_dyn_cop->setFont(font1);
        gridLayout_7 = new QGridLayout(panel_dyn_cop);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_7->setVerticalSpacing(20);
        panel_dyn_cop_intro = new QLabel(panel_dyn_cop);
        panel_dyn_cop_intro->setObjectName(QString::fromUtf8("panel_dyn_cop_intro"));
        panel_dyn_cop_intro->setFont(font1);
        panel_dyn_cop_intro->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        panel_dyn_cop_intro->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        gridLayout_7->addWidget(panel_dyn_cop_intro, 0, 0, 1, 2);

        panel_dyn_cop_ptext = new QLabel(panel_dyn_cop);
        panel_dyn_cop_ptext->setObjectName(QString::fromUtf8("panel_dyn_cop_ptext"));
        panel_dyn_cop_ptext->setFont(font1);
        panel_dyn_cop_ptext->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 26px \"RalewayBold\";"));

        gridLayout_7->addWidget(panel_dyn_cop_ptext, 1, 0, 1, 1);

        panel_dyn_cop_param1 = new QComboBox(panel_dyn_cop);
        panel_dyn_cop_param1->addItem(QString());
        panel_dyn_cop_param1->addItem(QString());
        panel_dyn_cop_param1->addItem(QString());
        panel_dyn_cop_param1->setObjectName(QString::fromUtf8("panel_dyn_cop_param1"));
        panel_dyn_cop_param1->setMinimumSize(QSize(0, 45));
        panel_dyn_cop_param1->setFont(font1);
        panel_dyn_cop_param1->setStyleSheet(QString::fromUtf8("font: 18px \"RalewayBold\";"));
        panel_dyn_cop_param1->setFrame(false);

        gridLayout_7->addWidget(panel_dyn_cop_param1, 1, 1, 1, 1);

        panel_dyn_cop_set = new QPushButton(panel_dyn_cop);
        panel_dyn_cop_set->setObjectName(QString::fromUtf8("panel_dyn_cop_set"));
        panel_dyn_cop_set->setMinimumSize(QSize(0, 60));
        panel_dyn_cop_set->setFont(font2);
        panel_dyn_cop_set->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 26px \"RalewayBlack\";"));

        gridLayout_7->addWidget(panel_dyn_cop_set, 2, 0, 1, 2);


        verticalLayout_6->addWidget(panel_dyn_cop);

        panel_dyn_bliss = new QWidget(tab_presets_dynamic_content);
        panel_dyn_bliss->setObjectName(QString::fromUtf8("panel_dyn_bliss"));
        panel_dyn_bliss->setFont(font1);
        gridLayout_5 = new QGridLayout(panel_dyn_bliss);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setVerticalSpacing(20);
        panel_dyn_bliss_param = new QComboBox(panel_dyn_bliss);
        panel_dyn_bliss_param->addItem(QString());
        panel_dyn_bliss_param->addItem(QString());
        panel_dyn_bliss_param->addItem(QString());
        panel_dyn_bliss_param->addItem(QString());
        panel_dyn_bliss_param->addItem(QString());
        panel_dyn_bliss_param->addItem(QString());
        panel_dyn_bliss_param->setObjectName(QString::fromUtf8("panel_dyn_bliss_param"));
        panel_dyn_bliss_param->setMinimumSize(QSize(0, 45));
        panel_dyn_bliss_param->setFont(font1);
        panel_dyn_bliss_param->setStyleSheet(QString::fromUtf8("font: 18px \"RalewayBold\";"));
        panel_dyn_bliss_param->setFrame(false);

        gridLayout_5->addWidget(panel_dyn_bliss_param, 0, 1, 1, 1);

        panel_dyn_bliss_ptext = new QLabel(panel_dyn_bliss);
        panel_dyn_bliss_ptext->setObjectName(QString::fromUtf8("panel_dyn_bliss_ptext"));
        panel_dyn_bliss_ptext->setFont(font1);
        panel_dyn_bliss_ptext->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 26px \"RalewayBold\";"));

        gridLayout_5->addWidget(panel_dyn_bliss_ptext, 0, 0, 1, 1);

        panel_dyn_bliss_set = new QPushButton(panel_dyn_bliss);
        panel_dyn_bliss_set->setObjectName(QString::fromUtf8("panel_dyn_bliss_set"));
        panel_dyn_bliss_set->setMinimumSize(QSize(0, 60));
        panel_dyn_bliss_set->setFont(font2);
        panel_dyn_bliss_set->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 26px \"RalewayBlack\";"));

        gridLayout_5->addWidget(panel_dyn_bliss_set, 2, 0, 1, 2);

        panel_dyn_bliss_description = new QLabel(panel_dyn_bliss);
        panel_dyn_bliss_description->setObjectName(QString::fromUtf8("panel_dyn_bliss_description"));
        panel_dyn_bliss_description->setFont(font1);
        panel_dyn_bliss_description->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        panel_dyn_bliss_description->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        panel_dyn_bliss_description->setWordWrap(true);

        gridLayout_5->addWidget(panel_dyn_bliss_description, 1, 0, 1, 2);


        verticalLayout_6->addWidget(panel_dyn_bliss);

        dynamic_intro = new QLabel(tab_presets_dynamic_content);
        dynamic_intro->setObjectName(QString::fromUtf8("dynamic_intro"));
        dynamic_intro->setMinimumSize(QSize(0, 40));
        dynamic_intro->setFont(font1);
        dynamic_intro->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayBold\";"));
        dynamic_intro->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        verticalLayout_6->addWidget(dynamic_intro);

        dynamic_c_layout = new QWidget(tab_presets_dynamic_content);
        dynamic_c_layout->setObjectName(QString::fromUtf8("dynamic_c_layout"));
        dynamic_c_layout->setMinimumSize(QSize(0, 80));
        dynamic_c_layout->setMaximumSize(QSize(16777215, 16777215));
        horizontalLayout_3 = new QHBoxLayout(dynamic_c_layout);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(12, 12, 12, 12);
        dynamic_c1 = new ClickableLabel(dynamic_c_layout);
        dynamic_c1->setObjectName(QString::fromUtf8("dynamic_c1"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dynamic_c1->sizePolicy().hasHeightForWidth());
        dynamic_c1->setSizePolicy(sizePolicy);
        dynamic_c1->setMinimumSize(QSize(0, 60));
        dynamic_c1->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_3->addWidget(dynamic_c1);

        dynamic_c2 = new ClickableLabel(dynamic_c_layout);
        dynamic_c2->setObjectName(QString::fromUtf8("dynamic_c2"));
        sizePolicy.setHeightForWidth(dynamic_c2->sizePolicy().hasHeightForWidth());
        dynamic_c2->setSizePolicy(sizePolicy);
        dynamic_c2->setMinimumSize(QSize(0, 60));
        dynamic_c2->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_3->addWidget(dynamic_c2);

        dynamic_c3 = new ClickableLabel(dynamic_c_layout);
        dynamic_c3->setObjectName(QString::fromUtf8("dynamic_c3"));
        sizePolicy.setHeightForWidth(dynamic_c3->sizePolicy().hasHeightForWidth());
        dynamic_c3->setSizePolicy(sizePolicy);
        dynamic_c3->setMinimumSize(QSize(0, 60));
        dynamic_c3->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_3->addWidget(dynamic_c3);

        dynamic_c4 = new ClickableLabel(dynamic_c_layout);
        dynamic_c4->setObjectName(QString::fromUtf8("dynamic_c4"));
        sizePolicy.setHeightForWidth(dynamic_c4->sizePolicy().hasHeightForWidth());
        dynamic_c4->setSizePolicy(sizePolicy);
        dynamic_c4->setMinimumSize(QSize(0, 60));
        dynamic_c4->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_3->addWidget(dynamic_c4);

        dynamic_c5 = new ClickableLabel(dynamic_c_layout);
        dynamic_c5->setObjectName(QString::fromUtf8("dynamic_c5"));
        sizePolicy.setHeightForWidth(dynamic_c5->sizePolicy().hasHeightForWidth());
        dynamic_c5->setSizePolicy(sizePolicy);
        dynamic_c5->setMinimumSize(QSize(0, 60));
        dynamic_c5->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_3->addWidget(dynamic_c5);

        dynamic_c6 = new ClickableLabel(dynamic_c_layout);
        dynamic_c6->setObjectName(QString::fromUtf8("dynamic_c6"));
        sizePolicy.setHeightForWidth(dynamic_c6->sizePolicy().hasHeightForWidth());
        dynamic_c6->setSizePolicy(sizePolicy);
        dynamic_c6->setMinimumSize(QSize(0, 60));
        dynamic_c6->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_3->addWidget(dynamic_c6);


        verticalLayout_6->addWidget(dynamic_c_layout);

        dynamic_t_intro = new QLabel(tab_presets_dynamic_content);
        dynamic_t_intro->setObjectName(QString::fromUtf8("dynamic_t_intro"));
        dynamic_t_intro->setMinimumSize(QSize(0, 30));
        dynamic_t_intro->setFont(font1);
        dynamic_t_intro->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayBold\";"));
        dynamic_t_intro->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        verticalLayout_6->addWidget(dynamic_t_intro);

        dynamic_t = new QWidget(tab_presets_dynamic_content);
        dynamic_t->setObjectName(QString::fromUtf8("dynamic_t"));
        dynamic_t->setFont(font1);
        horizontalLayout_4 = new QHBoxLayout(dynamic_t);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 12, -1, -1);
        dynamic_t_1 = new ClickableLabel(dynamic_t);
        dynamic_t_1->setObjectName(QString::fromUtf8("dynamic_t_1"));
        dynamic_t_1->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_1);

        dynamic_t_2 = new ClickableLabel(dynamic_t);
        dynamic_t_2->setObjectName(QString::fromUtf8("dynamic_t_2"));
        dynamic_t_2->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_2);

        dynamic_t_3 = new ClickableLabel(dynamic_t);
        dynamic_t_3->setObjectName(QString::fromUtf8("dynamic_t_3"));
        dynamic_t_3->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_3);

        dynamic_t_4 = new ClickableLabel(dynamic_t);
        dynamic_t_4->setObjectName(QString::fromUtf8("dynamic_t_4"));
        dynamic_t_4->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_4);

        dynamic_t_5 = new ClickableLabel(dynamic_t);
        dynamic_t_5->setObjectName(QString::fromUtf8("dynamic_t_5"));
        dynamic_t_5->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_5);

        dynamic_t_6 = new ClickableLabel(dynamic_t);
        dynamic_t_6->setObjectName(QString::fromUtf8("dynamic_t_6"));
        dynamic_t_6->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_6);

        dynamic_t_7 = new ClickableLabel(dynamic_t);
        dynamic_t_7->setObjectName(QString::fromUtf8("dynamic_t_7"));
        dynamic_t_7->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_7);

        dynamic_t_8 = new ClickableLabel(dynamic_t);
        dynamic_t_8->setObjectName(QString::fromUtf8("dynamic_t_8"));
        dynamic_t_8->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_8);

        dynamic_t_9 = new ClickableLabel(dynamic_t);
        dynamic_t_9->setObjectName(QString::fromUtf8("dynamic_t_9"));
        dynamic_t_9->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_9);

        dynamic_t_10 = new ClickableLabel(dynamic_t);
        dynamic_t_10->setObjectName(QString::fromUtf8("dynamic_t_10"));
        dynamic_t_10->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_10);

        dynamic_t_11 = new ClickableLabel(dynamic_t);
        dynamic_t_11->setObjectName(QString::fromUtf8("dynamic_t_11"));
        dynamic_t_11->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_11);

        dynamic_t_12 = new ClickableLabel(dynamic_t);
        dynamic_t_12->setObjectName(QString::fromUtf8("dynamic_t_12"));
        dynamic_t_12->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_12);

        dynamic_t_13 = new ClickableLabel(dynamic_t);
        dynamic_t_13->setObjectName(QString::fromUtf8("dynamic_t_13"));
        dynamic_t_13->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_13);

        dynamic_t_14 = new ClickableLabel(dynamic_t);
        dynamic_t_14->setObjectName(QString::fromUtf8("dynamic_t_14"));
        dynamic_t_14->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_14);

        dynamic_t_15 = new ClickableLabel(dynamic_t);
        dynamic_t_15->setObjectName(QString::fromUtf8("dynamic_t_15"));
        dynamic_t_15->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_15);

        dynamic_t_16 = new ClickableLabel(dynamic_t);
        dynamic_t_16->setObjectName(QString::fromUtf8("dynamic_t_16"));
        dynamic_t_16->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_16);

        dynamic_t_17 = new ClickableLabel(dynamic_t);
        dynamic_t_17->setObjectName(QString::fromUtf8("dynamic_t_17"));
        dynamic_t_17->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_17);

        dynamic_t_18 = new ClickableLabel(dynamic_t);
        dynamic_t_18->setObjectName(QString::fromUtf8("dynamic_t_18"));
        dynamic_t_18->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_18);

        dynamic_t_19 = new ClickableLabel(dynamic_t);
        dynamic_t_19->setObjectName(QString::fromUtf8("dynamic_t_19"));
        dynamic_t_19->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_19);

        dynamic_t_20 = new ClickableLabel(dynamic_t);
        dynamic_t_20->setObjectName(QString::fromUtf8("dynamic_t_20"));
        dynamic_t_20->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_20);

        dynamic_t_21 = new ClickableLabel(dynamic_t);
        dynamic_t_21->setObjectName(QString::fromUtf8("dynamic_t_21"));
        dynamic_t_21->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_4->addWidget(dynamic_t_21);


        verticalLayout_6->addWidget(dynamic_t);

        scroll_presets_dynamic->setWidget(tab_presets_dynamic_content);

        horizontalLayout_6->addWidget(scroll_presets_dynamic);

        tab_presets_container->addTab(tab_presets_dynamic, QString());
        tab_moodsync = new QWidget();
        tab_moodsync->setObjectName(QString::fromUtf8("tab_moodsync"));
        horizontalLayout_7 = new QHBoxLayout(tab_moodsync);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        scroll_moodsync = new QScrollArea(tab_moodsync);
        scroll_moodsync->setObjectName(QString::fromUtf8("scroll_moodsync"));
        scroll_moodsync->setFrameShape(QFrame::NoFrame);
        scroll_moodsync->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll_moodsync->setWidgetResizable(true);
        tab_moodsync_content = new QWidget();
        tab_moodsync_content->setObjectName(QString::fromUtf8("tab_moodsync_content"));
        tab_moodsync_content->setGeometry(QRect(0, 0, 499, 430));
        verticalLayout_7 = new QVBoxLayout(tab_moodsync_content);
        verticalLayout_7->setSpacing(10);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(-1, 15, -1, 20);
        lisync_intro = new QLabel(tab_moodsync_content);
        lisync_intro->setObjectName(QString::fromUtf8("lisync_intro"));
        lisync_intro->setFont(font1);
        lisync_intro->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);font: 12px \"RalewayBold\";"));
        lisync_intro->setAlignment(Qt::AlignCenter);
        lisync_intro->setWordWrap(true);

        verticalLayout_7->addWidget(lisync_intro);

        lisync_computer_layout = new QWidget(tab_moodsync_content);
        lisync_computer_layout->setObjectName(QString::fromUtf8("lisync_computer_layout"));
        lisync_computer_layout->setMinimumSize(QSize(281, 251));
        lisync_computer = new QLabel(lisync_computer_layout);
        lisync_computer->setObjectName(QString::fromUtf8("lisync_computer"));
        lisync_computer->setGeometry(QRect(0, 0, 281, 251));
        lisync_computer->setPixmap(QPixmap(QString::fromUtf8(":/Resources/computer.png")));
        lisync_sample = new ClickableLabel(lisync_computer_layout);
        lisync_sample->setObjectName(QString::fromUtf8("lisync_sample"));
        lisync_sample->setGeometry(QRect(10, 20, 251, 141));
        lisync_sample->setCursor(QCursor(Qt::OpenHandCursor));
        lisync_sample->setStyleSheet(QString::fromUtf8("background-color: rgb(55, 55, 55)"));
        lisync_sample->setPixmap(QPixmap(QString::fromUtf8(":/Resources/res0.jpg")));

        verticalLayout_7->addWidget(lisync_computer_layout, 0, Qt::AlignHCenter);

        lisync_layout = new QWidget(tab_moodsync_content);
        lisync_layout->setObjectName(QString::fromUtf8("lisync_layout"));
        horizontalLayout_5 = new QHBoxLayout(lisync_layout);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        lisync_c1 = new ClickableLabel(lisync_layout);
        lisync_c1->setObjectName(QString::fromUtf8("lisync_c1"));
        lisync_c1->setMinimumSize(QSize(0, 60));
        lisync_c1->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_5->addWidget(lisync_c1);

        lisync_c2 = new ClickableLabel(lisync_layout);
        lisync_c2->setObjectName(QString::fromUtf8("lisync_c2"));
        lisync_c2->setMinimumSize(QSize(0, 60));
        lisync_c2->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_5->addWidget(lisync_c2);

        lisync_c3 = new ClickableLabel(lisync_layout);
        lisync_c3->setObjectName(QString::fromUtf8("lisync_c3"));
        lisync_c3->setMinimumSize(QSize(0, 60));
        lisync_c3->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_5->addWidget(lisync_c3);

        lisync_c4 = new ClickableLabel(lisync_layout);
        lisync_c4->setObjectName(QString::fromUtf8("lisync_c4"));
        lisync_c4->setMinimumSize(QSize(0, 60));
        lisync_c4->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_5->addWidget(lisync_c4);

        lisync_c5 = new ClickableLabel(lisync_layout);
        lisync_c5->setObjectName(QString::fromUtf8("lisync_c5"));
        lisync_c5->setMinimumSize(QSize(0, 60));
        lisync_c5->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_5->addWidget(lisync_c5);

        lisync_c6 = new ClickableLabel(lisync_layout);
        lisync_c6->setObjectName(QString::fromUtf8("lisync_c6"));
        lisync_c6->setMinimumSize(QSize(0, 60));
        lisync_c6->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);"));

        horizontalLayout_5->addWidget(lisync_c6);


        verticalLayout_7->addWidget(lisync_layout);

        lisync_status = new QLabel(tab_moodsync_content);
        lisync_status->setObjectName(QString::fromUtf8("lisync_status"));
        lisync_status->setFont(font1);
        lisync_status->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);font: 12px \"RalewayBold\";"));
        lisync_status->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(lisync_status);

        scroll_moodsync->setWidget(tab_moodsync_content);

        horizontalLayout_7->addWidget(scroll_moodsync);

        tab_presets_container->addTab(tab_moodsync, QString());
        tab_shuffle = new QWidget();
        tab_shuffle->setObjectName(QString::fromUtf8("tab_shuffle"));
        horizontalLayout_8 = new QHBoxLayout(tab_shuffle);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        scroll_shuffle = new QScrollArea(tab_shuffle);
        scroll_shuffle->setObjectName(QString::fromUtf8("scroll_shuffle"));
        scroll_shuffle->setFrameShape(QFrame::NoFrame);
        scroll_shuffle->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll_shuffle->setWidgetResizable(true);
        tab_shuffle_content = new QWidget();
        tab_shuffle_content->setObjectName(QString::fromUtf8("tab_shuffle_content"));
        tab_shuffle_content->setGeometry(QRect(0, 0, 499, 331));
        gridLayout_12 = new QGridLayout(tab_shuffle_content);
        gridLayout_12->setSpacing(10);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout_12->setContentsMargins(-1, 10, -1, 30);
        shuffle_intro1 = new QLabel(tab_shuffle_content);
        shuffle_intro1->setObjectName(QString::fromUtf8("shuffle_intro1"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(shuffle_intro1->sizePolicy().hasHeightForWidth());
        shuffle_intro1->setSizePolicy(sizePolicy1);
        shuffle_intro1->setMaximumSize(QSize(16777215, 40));
        shuffle_intro1->setFont(font1);
        shuffle_intro1->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);font: 18px \"RalewayBold\";"));
        shuffle_intro1->setAlignment(Qt::AlignCenter);

        gridLayout_12->addWidget(shuffle_intro1, 0, 0, 1, 2);

        shuffle_intro3 = new QLabel(tab_shuffle_content);
        shuffle_intro3->setObjectName(QString::fromUtf8("shuffle_intro3"));
        sizePolicy1.setHeightForWidth(shuffle_intro3->sizePolicy().hasHeightForWidth());
        shuffle_intro3->setSizePolicy(sizePolicy1);
        shuffle_intro3->setMaximumSize(QSize(16777215, 40));
        shuffle_intro3->setFont(font1);
        shuffle_intro3->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);font: 12px \"RalewayBold\";"));
        shuffle_intro3->setAlignment(Qt::AlignCenter);

        gridLayout_12->addWidget(shuffle_intro3, 1, 0, 1, 2);

        shuffle_en_static = new QCheckBox(tab_shuffle_content);
        shuffle_en_static->setObjectName(QString::fromUtf8("shuffle_en_static"));
        shuffle_en_static->setEnabled(false);
        shuffle_en_static->setFont(font1);
        shuffle_en_static->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 12px \"RalewayBold\";"));
        shuffle_en_static->setChecked(true);

        gridLayout_12->addWidget(shuffle_en_static, 2, 0, 1, 1);

        shuffle_en_chase = new QCheckBox(tab_shuffle_content);
        shuffle_en_chase->setObjectName(QString::fromUtf8("shuffle_en_chase"));
        shuffle_en_chase->setFont(font1);
        shuffle_en_chase->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 12px \"RalewayBold\";"));
        shuffle_en_chase->setChecked(true);

        gridLayout_12->addWidget(shuffle_en_chase, 2, 1, 1, 1);

        shuffle_en_bliss = new QCheckBox(tab_shuffle_content);
        shuffle_en_bliss->setObjectName(QString::fromUtf8("shuffle_en_bliss"));
        shuffle_en_bliss->setFont(font1);
        shuffle_en_bliss->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 12px \"RalewayBold\";"));
        shuffle_en_bliss->setChecked(true);

        gridLayout_12->addWidget(shuffle_en_bliss, 3, 0, 1, 1);

        shuffle_en_tcode = new QCheckBox(tab_shuffle_content);
        shuffle_en_tcode->setObjectName(QString::fromUtf8("shuffle_en_tcode"));
        shuffle_en_tcode->setFont(font1);
        shuffle_en_tcode->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 12px \"RalewayBold\";"));
        shuffle_en_tcode->setChecked(false);

        gridLayout_12->addWidget(shuffle_en_tcode, 3, 1, 1, 1);

        shuffle_en_spectrum = new QCheckBox(tab_shuffle_content);
        shuffle_en_spectrum->setObjectName(QString::fromUtf8("shuffle_en_spectrum"));
        shuffle_en_spectrum->setFont(font1);
        shuffle_en_spectrum->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 12px \"RalewayBold\";"));
        shuffle_en_spectrum->setChecked(true);

        gridLayout_12->addWidget(shuffle_en_spectrum, 4, 0, 1, 1);

        shuffle_en_music = new QCheckBox(tab_shuffle_content);
        shuffle_en_music->setObjectName(QString::fromUtf8("shuffle_en_music"));
        shuffle_en_music->setFont(font1);
        shuffle_en_music->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 12px \"RalewayBold\";"));

        gridLayout_12->addWidget(shuffle_en_music, 4, 1, 1, 1);

        shuffle_en_rnb = new QCheckBox(tab_shuffle_content);
        shuffle_en_rnb->setObjectName(QString::fromUtf8("shuffle_en_rnb"));
        shuffle_en_rnb->setFont(font1);
        shuffle_en_rnb->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 12px \"RalewayBold\";"));
        shuffle_en_rnb->setChecked(true);

        gridLayout_12->addWidget(shuffle_en_rnb, 5, 0, 1, 1);

        shuffle_en_cop = new QCheckBox(tab_shuffle_content);
        shuffle_en_cop->setObjectName(QString::fromUtf8("shuffle_en_cop"));
        shuffle_en_cop->setFont(font1);
        shuffle_en_cop->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 12px \"RalewayBold\";"));

        gridLayout_12->addWidget(shuffle_en_cop, 5, 1, 1, 1);

        shuffle_intro2 = new QLabel(tab_shuffle_content);
        shuffle_intro2->setObjectName(QString::fromUtf8("shuffle_intro2"));
        shuffle_intro2->setMaximumSize(QSize(16777215, 40));
        shuffle_intro2->setFont(font1);
        shuffle_intro2->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);font: 18px \"RalewayBold\";"));
        shuffle_intro2->setAlignment(Qt::AlignCenter);

        gridLayout_12->addWidget(shuffle_intro2, 6, 0, 1, 2);

        shuffle_speed = new QComboBox(tab_shuffle_content);
        shuffle_speed->addItem(QString());
        shuffle_speed->addItem(QString());
        shuffle_speed->addItem(QString());
        shuffle_speed->addItem(QString());
        shuffle_speed->addItem(QString());
        shuffle_speed->setObjectName(QString::fromUtf8("shuffle_speed"));
        shuffle_speed->setFont(font1);
        shuffle_speed->setStyleSheet(QString::fromUtf8("font: 18px \"RalewayBold\";"));
        shuffle_speed->setFrame(false);

        gridLayout_12->addWidget(shuffle_speed, 7, 0, 1, 2);

        shuffle_set = new QPushButton(tab_shuffle_content);
        shuffle_set->setObjectName(QString::fromUtf8("shuffle_set"));
        shuffle_set->setMinimumSize(QSize(0, 75));
        shuffle_set->setFont(font2);
        shuffle_set->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 26px \"RalewayBlack\";"));

        gridLayout_12->addWidget(shuffle_set, 8, 0, 1, 2);

        scroll_shuffle->setWidget(tab_shuffle_content);

        horizontalLayout_8->addWidget(scroll_shuffle);

        tab_presets_container->addTab(tab_shuffle, QString());

        verticalLayout_4->addWidget(tab_presets_container);

        panel_presets->setWidget(panel_presets_content);

        verticalLayout_17->addWidget(panel_presets);

        panel_custom_colors = new QScrollArea(control_parent);
        panel_custom_colors->setObjectName(QString::fromUtf8("panel_custom_colors"));
        panel_custom_colors->setMinimumSize(QSize(450, 0));
        panel_custom_colors->setFrameShape(QFrame::NoFrame);
        panel_custom_colors->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        panel_custom_colors->setWidgetResizable(true);
        panel_custom_colors_content = new QWidget();
        panel_custom_colors_content->setObjectName(QString::fromUtf8("panel_custom_colors_content"));
        panel_custom_colors_content->setGeometry(QRect(0, 0, 545, 917));
        verticalLayout_3 = new QVBoxLayout(panel_custom_colors_content);
        verticalLayout_3->setSpacing(20);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        custom_intro = new QLabel(panel_custom_colors_content);
        custom_intro->setObjectName(QString::fromUtf8("custom_intro"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("RalewayMedium"));
        font3.setBold(false);
        font3.setItalic(false);
        font3.setWeight(50);
        custom_intro->setFont(font3);
        custom_intro->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));
        custom_intro->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(custom_intro);

        custom_color_wheel_parent = new QWidget(panel_custom_colors_content);
        custom_color_wheel_parent->setObjectName(QString::fromUtf8("custom_color_wheel_parent"));
        custom_color_wheel_parent->setMinimumSize(QSize(255, 255));
        custom_color_wheel_cursor = new MovableLabel(custom_color_wheel_parent);
        custom_color_wheel_cursor->setObjectName(QString::fromUtf8("custom_color_wheel_cursor"));
        custom_color_wheel_cursor->setGeometry(QRect(100, 120, 10, 10));
        custom_color_wheel_cursor->setMinimumSize(QSize(10, 10));
        custom_color_wheel_cursor->setMaximumSize(QSize(10, 10));
        custom_color_wheel_cursor->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
        custom_color_wheel_cursor->setPixmap(QPixmap(QString::fromUtf8(":/Resources/cursor.png")));
        custom_color_wheel_cursor->setScaledContents(true);
        custom_color_wheel_cursor->setAlignment(Qt::AlignCenter);
        formLayout = new QFormLayout(custom_color_wheel_parent);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setLabelAlignment(Qt::AlignCenter);
        formLayout->setContentsMargins(0, 0, 0, 0);
        custom_color_wheel = new ClickableLabel(custom_color_wheel_parent);
        custom_color_wheel->setObjectName(QString::fromUtf8("custom_color_wheel"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(custom_color_wheel->sizePolicy().hasHeightForWidth());
        custom_color_wheel->setSizePolicy(sizePolicy2);
        custom_color_wheel->setMinimumSize(QSize(255, 255));
        custom_color_wheel->setMaximumSize(QSize(255, 255));
        custom_color_wheel->setPixmap(QPixmap(QString::fromUtf8(":/Resources/hsl_wheel.png")));
        custom_color_wheel->setScaledContents(true);
        custom_color_wheel->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, custom_color_wheel);

        custom_color_wheel->raise();
        custom_color_wheel_cursor->raise();

        verticalLayout_3->addWidget(custom_color_wheel_parent, 0, Qt::AlignHCenter);

        custom_led_grid = new QWidget(panel_custom_colors_content);
        custom_led_grid->setObjectName(QString::fromUtf8("custom_led_grid"));
        gridLayout = new QGridLayout(custom_led_grid);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        custom_select_led_6 = new QCheckBox(custom_led_grid);
        custom_select_led_6->setObjectName(QString::fromUtf8("custom_select_led_6"));
        custom_select_led_6->setFont(font3);
        custom_select_led_6->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));
        custom_select_led_6->setChecked(true);

        gridLayout->addWidget(custom_select_led_6, 0, 0, 1, 1, Qt::AlignHCenter);

        custom_select_led_5 = new QCheckBox(custom_led_grid);
        custom_select_led_5->setObjectName(QString::fromUtf8("custom_select_led_5"));
        custom_select_led_5->setFont(font3);
        custom_select_led_5->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));
        custom_select_led_5->setChecked(true);

        gridLayout->addWidget(custom_select_led_5, 0, 1, 1, 1, Qt::AlignHCenter);

        custom_select_led_4 = new QCheckBox(custom_led_grid);
        custom_select_led_4->setObjectName(QString::fromUtf8("custom_select_led_4"));
        custom_select_led_4->setFont(font3);
        custom_select_led_4->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));
        custom_select_led_4->setChecked(true);

        gridLayout->addWidget(custom_select_led_4, 0, 2, 1, 1, Qt::AlignHCenter);

        custom_select_led_3 = new QCheckBox(custom_led_grid);
        custom_select_led_3->setObjectName(QString::fromUtf8("custom_select_led_3"));
        custom_select_led_3->setFont(font3);
        custom_select_led_3->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));
        custom_select_led_3->setChecked(true);

        gridLayout->addWidget(custom_select_led_3, 1, 0, 1, 1, Qt::AlignHCenter);

        custom_select_led_2 = new QCheckBox(custom_led_grid);
        custom_select_led_2->setObjectName(QString::fromUtf8("custom_select_led_2"));
        custom_select_led_2->setFont(font3);
        custom_select_led_2->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));
        custom_select_led_2->setChecked(true);

        gridLayout->addWidget(custom_select_led_2, 1, 1, 1, 1, Qt::AlignHCenter);

        custom_select_led_1 = new QCheckBox(custom_led_grid);
        custom_select_led_1->setObjectName(QString::fromUtf8("custom_select_led_1"));
        custom_select_led_1->setFont(font3);
        custom_select_led_1->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));
        custom_select_led_1->setChecked(true);

        gridLayout->addWidget(custom_select_led_1, 1, 2, 1, 1, Qt::AlignHCenter);


        verticalLayout_3->addWidget(custom_led_grid);

        custom_label_r_2 = new QLabel(panel_custom_colors_content);
        custom_label_r_2->setObjectName(QString::fromUtf8("custom_label_r_2"));
        custom_label_r_2->setFont(font1);
        custom_label_r_2->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        custom_label_r_2->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(custom_label_r_2);

        custom_grid_rgbw = new QWidget(panel_custom_colors_content);
        custom_grid_rgbw->setObjectName(QString::fromUtf8("custom_grid_rgbw"));
        gridLayout_2 = new QGridLayout(custom_grid_rgbw);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        custom_label_r = new QLabel(custom_grid_rgbw);
        custom_label_r->setObjectName(QString::fromUtf8("custom_label_r"));
        custom_label_r->setFont(font3);
        custom_label_r->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));

        gridLayout_2->addWidget(custom_label_r, 0, 0, 1, 1);

        custom_slider_r = new QSlider(custom_grid_rgbw);
        custom_slider_r->setObjectName(QString::fromUtf8("custom_slider_r"));
        custom_slider_r->setMaximum(255);
        custom_slider_r->setOrientation(Qt::Horizontal);
        custom_slider_r->setInvertedAppearance(false);

        gridLayout_2->addWidget(custom_slider_r, 0, 1, 1, 1);

        custom_value_r = new QSpinBox(custom_grid_rgbw);
        custom_value_r->setObjectName(QString::fromUtf8("custom_value_r"));
        custom_value_r->setFont(font3);
        custom_value_r->setLayoutDirection(Qt::LeftToRight);
        custom_value_r->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        custom_value_r->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_r->setMaximum(255);

        gridLayout_2->addWidget(custom_value_r, 0, 2, 1, 1);

        custom_label_g = new QLabel(custom_grid_rgbw);
        custom_label_g->setObjectName(QString::fromUtf8("custom_label_g"));
        custom_label_g->setFont(font3);
        custom_label_g->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));

        gridLayout_2->addWidget(custom_label_g, 1, 0, 1, 1);

        custom_slider_g = new QSlider(custom_grid_rgbw);
        custom_slider_g->setObjectName(QString::fromUtf8("custom_slider_g"));
        custom_slider_g->setMaximum(255);
        custom_slider_g->setOrientation(Qt::Horizontal);
        custom_slider_g->setInvertedAppearance(false);

        gridLayout_2->addWidget(custom_slider_g, 1, 1, 1, 1);

        custom_value_g = new QSpinBox(custom_grid_rgbw);
        custom_value_g->setObjectName(QString::fromUtf8("custom_value_g"));
        custom_value_g->setFont(font3);
        custom_value_g->setLayoutDirection(Qt::LeftToRight);
        custom_value_g->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        custom_value_g->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_g->setMaximum(255);

        gridLayout_2->addWidget(custom_value_g, 1, 2, 1, 1);

        custom_label_b = new QLabel(custom_grid_rgbw);
        custom_label_b->setObjectName(QString::fromUtf8("custom_label_b"));
        custom_label_b->setFont(font3);
        custom_label_b->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));

        gridLayout_2->addWidget(custom_label_b, 2, 0, 1, 1);

        custom_slider_b = new QSlider(custom_grid_rgbw);
        custom_slider_b->setObjectName(QString::fromUtf8("custom_slider_b"));
        custom_slider_b->setMaximum(255);
        custom_slider_b->setOrientation(Qt::Horizontal);
        custom_slider_b->setInvertedAppearance(false);

        gridLayout_2->addWidget(custom_slider_b, 2, 1, 1, 1);

        custom_value_b = new QSpinBox(custom_grid_rgbw);
        custom_value_b->setObjectName(QString::fromUtf8("custom_value_b"));
        custom_value_b->setFont(font3);
        custom_value_b->setLayoutDirection(Qt::LeftToRight);
        custom_value_b->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        custom_value_b->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_b->setMaximum(255);

        gridLayout_2->addWidget(custom_value_b, 2, 2, 1, 1);

        custom_label_w = new QLabel(custom_grid_rgbw);
        custom_label_w->setObjectName(QString::fromUtf8("custom_label_w"));
        custom_label_w->setFont(font3);
        custom_label_w->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));

        gridLayout_2->addWidget(custom_label_w, 3, 0, 1, 1);

        custom_slider_w = new QSlider(custom_grid_rgbw);
        custom_slider_w->setObjectName(QString::fromUtf8("custom_slider_w"));
        custom_slider_w->setMaximum(255);
        custom_slider_w->setOrientation(Qt::Horizontal);
        custom_slider_w->setInvertedAppearance(false);

        gridLayout_2->addWidget(custom_slider_w, 3, 1, 1, 1);

        custom_value_w = new QSpinBox(custom_grid_rgbw);
        custom_value_w->setObjectName(QString::fromUtf8("custom_value_w"));
        custom_value_w->setFont(font3);
        custom_value_w->setLayoutDirection(Qt::LeftToRight);
        custom_value_w->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        custom_value_w->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_w->setMaximum(255);

        gridLayout_2->addWidget(custom_value_w, 3, 2, 1, 1);


        verticalLayout_3->addWidget(custom_grid_rgbw);

        custom_label_r_3 = new QLabel(panel_custom_colors_content);
        custom_label_r_3->setObjectName(QString::fromUtf8("custom_label_r_3"));
        custom_label_r_3->setFont(font1);
        custom_label_r_3->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        custom_label_r_3->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(custom_label_r_3);

        custom_grid_hsl = new QWidget(panel_custom_colors_content);
        custom_grid_hsl->setObjectName(QString::fromUtf8("custom_grid_hsl"));
        gridLayout_3 = new QGridLayout(custom_grid_hsl);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        custom_label_h = new QLabel(custom_grid_hsl);
        custom_label_h->setObjectName(QString::fromUtf8("custom_label_h"));
        custom_label_h->setFont(font3);
        custom_label_h->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));

        gridLayout_3->addWidget(custom_label_h, 0, 0, 1, 1);

        custom_slider_h = new QSlider(custom_grid_hsl);
        custom_slider_h->setObjectName(QString::fromUtf8("custom_slider_h"));
        custom_slider_h->setMaximum(359);
        custom_slider_h->setOrientation(Qt::Horizontal);
        custom_slider_h->setInvertedAppearance(false);

        gridLayout_3->addWidget(custom_slider_h, 0, 1, 1, 1);

        custom_value_h = new QSpinBox(custom_grid_hsl);
        custom_value_h->setObjectName(QString::fromUtf8("custom_value_h"));
        custom_value_h->setFont(font3);
        custom_value_h->setLayoutDirection(Qt::LeftToRight);
        custom_value_h->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        custom_value_h->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_h->setMaximum(359);

        gridLayout_3->addWidget(custom_value_h, 0, 2, 1, 1);

        custom_label_s = new QLabel(custom_grid_hsl);
        custom_label_s->setObjectName(QString::fromUtf8("custom_label_s"));
        custom_label_s->setFont(font3);
        custom_label_s->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));

        gridLayout_3->addWidget(custom_label_s, 1, 0, 1, 1);

        custom_slider_s = new QSlider(custom_grid_hsl);
        custom_slider_s->setObjectName(QString::fromUtf8("custom_slider_s"));
        custom_slider_s->setMaximum(255);
        custom_slider_s->setOrientation(Qt::Horizontal);
        custom_slider_s->setInvertedAppearance(false);

        gridLayout_3->addWidget(custom_slider_s, 1, 1, 1, 1);

        custom_value_s = new QSpinBox(custom_grid_hsl);
        custom_value_s->setObjectName(QString::fromUtf8("custom_value_s"));
        custom_value_s->setFont(font3);
        custom_value_s->setLayoutDirection(Qt::LeftToRight);
        custom_value_s->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        custom_value_s->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_s->setMaximum(255);

        gridLayout_3->addWidget(custom_value_s, 1, 2, 1, 1);

        custom_label_l = new QLabel(custom_grid_hsl);
        custom_label_l->setObjectName(QString::fromUtf8("custom_label_l"));
        custom_label_l->setFont(font3);
        custom_label_l->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));

        gridLayout_3->addWidget(custom_label_l, 2, 0, 1, 1);

        custom_slider_l = new QSlider(custom_grid_hsl);
        custom_slider_l->setObjectName(QString::fromUtf8("custom_slider_l"));
        custom_slider_l->setMaximum(255);
        custom_slider_l->setOrientation(Qt::Horizontal);
        custom_slider_l->setInvertedAppearance(false);

        gridLayout_3->addWidget(custom_slider_l, 2, 1, 1, 1);

        custom_value_l = new QSpinBox(custom_grid_hsl);
        custom_value_l->setObjectName(QString::fromUtf8("custom_value_l"));
        custom_value_l->setFont(font3);
        custom_value_l->setLayoutDirection(Qt::LeftToRight);
        custom_value_l->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        custom_value_l->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        custom_value_l->setMaximum(255);

        gridLayout_3->addWidget(custom_value_l, 2, 2, 1, 1);


        verticalLayout_3->addWidget(custom_grid_hsl);

        custom_grid_hex = new QWidget(panel_custom_colors_content);
        custom_grid_hex->setObjectName(QString::fromUtf8("custom_grid_hex"));
        custom_grid_hex->setMaximumSize(QSize(16777215, 50));
        horizontalLayout_2 = new QHBoxLayout(custom_grid_hex);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        custom_label_hex = new QLabel(custom_grid_hex);
        custom_label_hex->setObjectName(QString::fromUtf8("custom_label_hex"));
        custom_label_hex->setFont(font1);
        custom_label_hex->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));

        horizontalLayout_2->addWidget(custom_label_hex);

        custom_value_hex = new QPlainTextEdit(custom_grid_hex);
        custom_value_hex->setObjectName(QString::fromUtf8("custom_value_hex"));
        custom_value_hex->setMaximumSize(QSize(90, 16777215));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Courier New"));
        font4.setPointSize(12);
        font4.setBold(true);
        font4.setWeight(75);
        custom_value_hex->setFont(font4);
        custom_value_hex->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        custom_value_hex->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        custom_value_hex->setLineWrapMode(QPlainTextEdit::NoWrap);
        custom_value_hex->setReadOnly(true);

        horizontalLayout_2->addWidget(custom_value_hex);


        verticalLayout_3->addWidget(custom_grid_hex);

        custom_value_save = new QPushButton(panel_custom_colors_content);
        custom_value_save->setObjectName(QString::fromUtf8("custom_value_save"));
        custom_value_save->setMinimumSize(QSize(0, 45));
        custom_value_save->setFont(font3);
        custom_value_save->setAutoFillBackground(false);
        custom_value_save->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayMedium\";"));
        custom_value_save->setFlat(false);

        verticalLayout_3->addWidget(custom_value_save);

        custom_value_load = new QPushButton(panel_custom_colors_content);
        custom_value_load->setObjectName(QString::fromUtf8("custom_value_load"));
        custom_value_load->setMinimumSize(QSize(45, 45));
        custom_value_load->setFont(font3);
        custom_value_load->setAutoFillBackground(false);
        custom_value_load->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayMedium\";"));
        custom_value_load->setFlat(false);

        verticalLayout_3->addWidget(custom_value_load);

        panel_custom_colors->setWidget(panel_custom_colors_content);
        custom_intro->raise();
        custom_label_r_2->raise();
        custom_grid_rgbw->raise();
        custom_label_r_3->raise();
        custom_grid_hsl->raise();
        custom_led_grid->raise();
        custom_value_save->raise();
        custom_grid_hex->raise();
        custom_value_load->raise();
        custom_color_wheel_parent->raise();

        verticalLayout_17->addWidget(panel_custom_colors);

        panel_lsettings = new QScrollArea(control_parent);
        panel_lsettings->setObjectName(QString::fromUtf8("panel_lsettings"));
        panel_lsettings->setMinimumSize(QSize(450, 0));
        panel_lsettings->setFrameShape(QFrame::NoFrame);
        panel_lsettings->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        panel_lsettings->setWidgetResizable(true);
        panel_lsettings_content = new QWidget();
        panel_lsettings_content->setObjectName(QString::fromUtf8("panel_lsettings_content"));
        panel_lsettings_content->setGeometry(QRect(0, 0, 545, 739));
        verticalLayout_16 = new QVBoxLayout(panel_lsettings_content);
        verticalLayout_16->setSpacing(10);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        verticalLayout_16->setContentsMargins(20, 30, 20, 30);
        lsettings_td = new QGroupBox(panel_lsettings_content);
        lsettings_td->setObjectName(QString::fromUtf8("lsettings_td"));
        lsettings_td->setFont(font1);
        lsettings_td->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));
        lsettings_td->setFlat(true);
        verticalLayout_14 = new QVBoxLayout(lsettings_td);
        verticalLayout_14->setSpacing(20);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        lsettings_td_12hr_enable = new QCheckBox(lsettings_td);
        lsettings_td_12hr_enable->setObjectName(QString::fromUtf8("lsettings_td_12hr_enable"));
        lsettings_td_12hr_enable->setFont(font3);
        lsettings_td_12hr_enable->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        verticalLayout_14->addWidget(lsettings_td_12hr_enable);

        lsettings_td_l0_enable = new QCheckBox(lsettings_td);
        lsettings_td_l0_enable->setObjectName(QString::fromUtf8("lsettings_td_l0_enable"));
        lsettings_td_l0_enable->setEnabled(true);
        lsettings_td_l0_enable->setFont(font3);
        lsettings_td_l0_enable->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        verticalLayout_14->addWidget(lsettings_td_l0_enable);

        lsettings_td_intldate_enable = new QCheckBox(lsettings_td);
        lsettings_td_intldate_enable->setObjectName(QString::fromUtf8("lsettings_td_intldate_enable"));
        lsettings_td_intldate_enable->setEnabled(true);
        lsettings_td_intldate_enable->setFont(font3);
        lsettings_td_intldate_enable->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        verticalLayout_14->addWidget(lsettings_td_intldate_enable);


        verticalLayout_16->addWidget(lsettings_td);

        lsettings_nsh = new QGroupBox(panel_lsettings_content);
        lsettings_nsh->setObjectName(QString::fromUtf8("lsettings_nsh"));
        lsettings_nsh->setFont(font1);
        lsettings_nsh->setStyleSheet(QString::fromUtf8("\n"
"font: 12px \"RalewayBold\";"));
        lsettings_nsh->setFlat(true);
        verticalLayout_15 = new QVBoxLayout(lsettings_nsh);
        verticalLayout_15->setSpacing(20);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        lsettings_nsh_enable = new QCheckBox(lsettings_nsh);
        lsettings_nsh_enable->setObjectName(QString::fromUtf8("lsettings_nsh_enable"));
        lsettings_nsh_enable->setFont(font3);
        lsettings_nsh_enable->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        verticalLayout_15->addWidget(lsettings_nsh_enable);

        lsettings_nsh_scheduled_enable = new QCheckBox(lsettings_nsh);
        lsettings_nsh_scheduled_enable->setObjectName(QString::fromUtf8("lsettings_nsh_scheduled_enable"));
        lsettings_nsh_scheduled_enable->setFont(font3);
        lsettings_nsh_scheduled_enable->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        verticalLayout_15->addWidget(lsettings_nsh_scheduled_enable);

        lsettings_nsh_panel = new QWidget(lsettings_nsh);
        lsettings_nsh_panel->setObjectName(QString::fromUtf8("lsettings_nsh_panel"));
        lsettings_nsh_panel->setEnabled(false);
        lsettings_nsh_panel->setFont(font1);
        gridLayout_21 = new QGridLayout(lsettings_nsh_panel);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        lsettings_nsh_intro1 = new QLabel(lsettings_nsh_panel);
        lsettings_nsh_intro1->setObjectName(QString::fromUtf8("lsettings_nsh_intro1"));
        lsettings_nsh_intro1->setFont(font3);
        lsettings_nsh_intro1->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_21->addWidget(lsettings_nsh_intro1, 0, 0, 1, 2);

        lsettings_nsh_start = new QTimeEdit(lsettings_nsh_panel);
        lsettings_nsh_start->setObjectName(QString::fromUtf8("lsettings_nsh_start"));
        lsettings_nsh_start->setFont(font3);
        lsettings_nsh_start->setTime(QTime(22, 0, 0));

        gridLayout_21->addWidget(lsettings_nsh_start, 0, 2, 1, 1);

        lsettings_nsh_intro2 = new QLabel(lsettings_nsh_panel);
        lsettings_nsh_intro2->setObjectName(QString::fromUtf8("lsettings_nsh_intro2"));
        lsettings_nsh_intro2->setFont(font3);
        lsettings_nsh_intro2->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_21->addWidget(lsettings_nsh_intro2, 1, 0, 1, 1);

        lsettings_nsh_stop = new QTimeEdit(lsettings_nsh_panel);
        lsettings_nsh_stop->setObjectName(QString::fromUtf8("lsettings_nsh_stop"));
        lsettings_nsh_stop->setFont(font3);
        lsettings_nsh_stop->setTime(QTime(8, 0, 0));

        gridLayout_21->addWidget(lsettings_nsh_stop, 1, 1, 1, 1);

        lsettings_nsh_intro3 = new QLabel(lsettings_nsh_panel);
        lsettings_nsh_intro3->setObjectName(QString::fromUtf8("lsettings_nsh_intro3"));
        lsettings_nsh_intro3->setFont(font3);
        lsettings_nsh_intro3->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_21->addWidget(lsettings_nsh_intro3, 1, 2, 1, 1);


        verticalLayout_15->addWidget(lsettings_nsh_panel);


        verticalLayout_16->addWidget(lsettings_nsh);

        lsettings_settings = new QGroupBox(panel_lsettings_content);
        lsettings_settings->setObjectName(QString::fromUtf8("lsettings_settings"));
        lsettings_settings->setFont(font1);
        lsettings_settings->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        lsettings_settings->setFlat(true);
        verticalLayout_13 = new QVBoxLayout(lsettings_settings);
        verticalLayout_13->setSpacing(20);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        lsettings_settings_default = new QPushButton(lsettings_settings);
        lsettings_settings_default->setObjectName(QString::fromUtf8("lsettings_settings_default"));
        lsettings_settings_default->setMinimumSize(QSize(0, 40));
        lsettings_settings_default->setFont(font1);
        lsettings_settings_default->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        verticalLayout_13->addWidget(lsettings_settings_default);

        lsettings_settings_save = new QPushButton(lsettings_settings);
        lsettings_settings_save->setObjectName(QString::fromUtf8("lsettings_settings_save"));
        lsettings_settings_save->setMinimumSize(QSize(0, 40));
        lsettings_settings_save->setFont(font1);
        lsettings_settings_save->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        verticalLayout_13->addWidget(lsettings_settings_save);


        verticalLayout_16->addWidget(lsettings_settings);

        lsettings_info = new QGroupBox(panel_lsettings_content);
        lsettings_info->setObjectName(QString::fromUtf8("lsettings_info"));
        lsettings_info->setFont(font1);
        lsettings_info->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        lsettings_info->setFlat(true);
        gridLayout_20 = new QGridLayout(lsettings_info);
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        gridLayout_20->setVerticalSpacing(20);
        lsettings_info_hw_t = new QLabel(lsettings_info);
        lsettings_info_hw_t->setObjectName(QString::fromUtf8("lsettings_info_hw_t"));
        lsettings_info_hw_t->setFont(font3);
        lsettings_info_hw_t->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_20->addWidget(lsettings_info_hw_t, 0, 0, 1, 1);

        lsettings_info_hw = new QLabel(lsettings_info);
        lsettings_info_hw->setObjectName(QString::fromUtf8("lsettings_info_hw"));
        lsettings_info_hw->setFont(font3);
        lsettings_info_hw->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_20->addWidget(lsettings_info_hw, 0, 1, 1, 1);

        lsettings_info_fw_t = new QLabel(lsettings_info);
        lsettings_info_fw_t->setObjectName(QString::fromUtf8("lsettings_info_fw_t"));
        lsettings_info_fw_t->setFont(font3);
        lsettings_info_fw_t->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_20->addWidget(lsettings_info_fw_t, 1, 0, 1, 1);

        lsettings_info_fw = new QLabel(lsettings_info);
        lsettings_info_fw->setObjectName(QString::fromUtf8("lsettings_info_fw"));
        lsettings_info_fw->setFont(font3);
        lsettings_info_fw->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_20->addWidget(lsettings_info_fw, 1, 1, 1, 1);

        lsettings_info_app_t = new QLabel(lsettings_info);
        lsettings_info_app_t->setObjectName(QString::fromUtf8("lsettings_info_app_t"));
        lsettings_info_app_t->setFont(font3);
        lsettings_info_app_t->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_20->addWidget(lsettings_info_app_t, 2, 0, 1, 1);

        lsettings_info_app = new QLabel(lsettings_info);
        lsettings_info_app->setObjectName(QString::fromUtf8("lsettings_info_app"));
        lsettings_info_app->setFont(font3);
        lsettings_info_app->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_20->addWidget(lsettings_info_app, 2, 1, 1, 1);

        lsettings_info_notice = new QLabel(lsettings_info);
        lsettings_info_notice->setObjectName(QString::fromUtf8("lsettings_info_notice"));
        lsettings_info_notice->setFont(font3);
        lsettings_info_notice->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";"));
        lsettings_info_notice->setWordWrap(true);

        gridLayout_20->addWidget(lsettings_info_notice, 3, 0, 1, 2);

        lsettings_info_download = new QPushButton(lsettings_info);
        lsettings_info_download->setObjectName(QString::fromUtf8("lsettings_info_download"));
        lsettings_info_download->setMinimumSize(QSize(0, 40));
        lsettings_info_download->setFont(font1);
        lsettings_info_download->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_20->addWidget(lsettings_info_download, 4, 0, 1, 2);

        lsettings_info_update = new QPushButton(lsettings_info);
        lsettings_info_update->setObjectName(QString::fromUtf8("lsettings_info_update"));
        lsettings_info_update->setMinimumSize(QSize(0, 40));
        lsettings_info_update->setFont(font1);
        lsettings_info_update->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_20->addWidget(lsettings_info_update, 5, 0, 1, 2);


        verticalLayout_16->addWidget(lsettings_info);

        panel_lsettings->setWidget(panel_lsettings_content);

        verticalLayout_17->addWidget(panel_lsettings);

        panel_message = new QScrollArea(control_parent);
        panel_message->setObjectName(QString::fromUtf8("panel_message"));
        panel_message->setMinimumSize(QSize(450, 0));
        panel_message->setFrameShape(QFrame::NoFrame);
        panel_message->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        panel_message->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        panel_message->setWidgetResizable(true);
        panel_message_content = new QWidget();
        panel_message_content->setObjectName(QString::fromUtf8("panel_message_content"));
        panel_message_content->setGeometry(QRect(0, 0, 545, 642));
        verticalLayout = new QVBoxLayout(panel_message_content);
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(20, 30, 20, 30);
        message_intro = new QLabel(panel_message_content);
        message_intro->setObjectName(QString::fromUtf8("message_intro"));
        message_intro->setFont(font3);
        message_intro->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        message_intro->setAlignment(Qt::AlignCenter);
        message_intro->setWordWrap(true);

        verticalLayout->addWidget(message_intro);

        message_text = new QPlainTextEdit(panel_message_content);
        message_text->setObjectName(QString::fromUtf8("message_text"));
        message_text->setMaximumSize(QSize(16777215, 80));
        message_text->setFont(font2);
        message_text->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 48px \"RalewayBlack\";"));
        message_text->setInputMethodHints(Qt::ImhNone);
        message_text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        message_text->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        message_text->setLineWrapMode(QPlainTextEdit::NoWrap);

        verticalLayout->addWidget(message_text);

        message_intro_2 = new QLabel(panel_message_content);
        message_intro_2->setObjectName(QString::fromUtf8("message_intro_2"));
        message_intro_2->setFont(font1);
        message_intro_2->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        message_intro_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(message_intro_2);

        message_intro_3 = new QLabel(panel_message_content);
        message_intro_3->setObjectName(QString::fromUtf8("message_intro_3"));
        message_intro_3->setFont(font3);
        message_intro_3->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        message_intro_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        message_intro_3->setWordWrap(true);

        verticalLayout->addWidget(message_intro_3);

        message_intro_4 = new QLabel(panel_message_content);
        message_intro_4->setObjectName(QString::fromUtf8("message_intro_4"));
        message_intro_4->setFont(font3);
        message_intro_4->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        message_intro_4->setWordWrap(true);

        verticalLayout->addWidget(message_intro_4);

        message_intro_5 = new QLabel(panel_message_content);
        message_intro_5->setObjectName(QString::fromUtf8("message_intro_5"));
        message_intro_5->setFont(font3);
        message_intro_5->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        message_intro_5->setWordWrap(true);

        verticalLayout->addWidget(message_intro_5);

        message_intro_6 = new QLabel(panel_message_content);
        message_intro_6->setObjectName(QString::fromUtf8("message_intro_6"));
        message_intro_6->setFont(font3);
        message_intro_6->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        message_intro_6->setScaledContents(false);
        message_intro_6->setWordWrap(true);

        verticalLayout->addWidget(message_intro_6);

        message_set_custom_welcome = new QCheckBox(panel_message_content);
        message_set_custom_welcome->setObjectName(QString::fromUtf8("message_set_custom_welcome"));
        message_set_custom_welcome->setMinimumSize(QSize(0, 40));
        message_set_custom_welcome->setFont(font3);
        message_set_custom_welcome->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));

        verticalLayout->addWidget(message_set_custom_welcome, 0, Qt::AlignHCenter);

        message_intro_7 = new QLabel(panel_message_content);
        message_intro_7->setObjectName(QString::fromUtf8("message_intro_7"));
        message_intro_7->setFont(font2);
        message_intro_7->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 48px \"RalewayBlack\";"));
        message_intro_7->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(message_intro_7);

        message_length = new QPlainTextEdit(panel_message_content);
        message_length->setObjectName(QString::fromUtf8("message_length"));
        message_length->setMaximumSize(QSize(16777215, 75));
        message_length->setFont(font2);
        message_length->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 48px \"RalewayBlack\";"));
        message_length->setLineWrapMode(QPlainTextEdit::NoWrap);

        verticalLayout->addWidget(message_length);

        message_send = new QPushButton(panel_message_content);
        message_send->setObjectName(QString::fromUtf8("message_send"));
        message_send->setFont(font2);
        message_send->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 36px \"RalewayBlack\";"));
        message_send->setFlat(false);

        verticalLayout->addWidget(message_send);

        panel_message->setWidget(panel_message_content);

        verticalLayout_17->addWidget(panel_message);

        panel_settings = new QScrollArea(control_parent);
        panel_settings->setObjectName(QString::fromUtf8("panel_settings"));
        panel_settings->setMinimumSize(QSize(450, 0));
        panel_settings->setFrameShape(QFrame::NoFrame);
        panel_settings->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        panel_settings->setWidgetResizable(true);
        panel_settings_content = new QWidget();
        panel_settings_content->setObjectName(QString::fromUtf8("panel_settings_content"));
        panel_settings_content->setGeometry(QRect(0, 0, 545, 1140));
        verticalLayout_11 = new QVBoxLayout(panel_settings_content);
        verticalLayout_11->setSpacing(20);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(20, 30, 20, 30);
        settings_td = new QGroupBox(panel_settings_content);
        settings_td->setObjectName(QString::fromUtf8("settings_td"));
        settings_td->setFont(font1);
        settings_td->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        settings_td->setFlat(true);
        gridLayout_17 = new QGridLayout(settings_td);
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        gridLayout_17->setVerticalSpacing(20);
        settings_td_timemode_t = new QLabel(settings_td);
        settings_td_timemode_t->setObjectName(QString::fromUtf8("settings_td_timemode_t"));
        settings_td_timemode_t->setFont(font1);
        settings_td_timemode_t->setAutoFillBackground(false);
        settings_td_timemode_t->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_17->addWidget(settings_td_timemode_t, 0, 0, 1, 1);

        settings_td_timemode = new QComboBox(settings_td);
        settings_td_timemode->addItem(QString());
        settings_td_timemode->addItem(QString());
        settings_td_timemode->addItem(QString());
        settings_td_timemode->setObjectName(QString::fromUtf8("settings_td_timemode"));
        settings_td_timemode->setFont(font3);
        settings_td_timemode->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        settings_td_timemode->setFrame(false);

        gridLayout_17->addWidget(settings_td_timemode, 0, 1, 1, 1);

        settings_td_datemode_t = new QLabel(settings_td);
        settings_td_datemode_t->setObjectName(QString::fromUtf8("settings_td_datemode_t"));
        settings_td_datemode_t->setFont(font1);
        settings_td_datemode_t->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_17->addWidget(settings_td_datemode_t, 1, 0, 1, 1);

        settings_td_datemode = new QComboBox(settings_td);
        settings_td_datemode->addItem(QString());
        settings_td_datemode->addItem(QString());
        settings_td_datemode->setObjectName(QString::fromUtf8("settings_td_datemode"));
        settings_td_datemode->setFont(font3);
        settings_td_datemode->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        settings_td_datemode->setFrame(false);

        gridLayout_17->addWidget(settings_td_datemode, 1, 1, 1, 1);

        settings_td_set = new QPushButton(settings_td);
        settings_td_set->setObjectName(QString::fromUtf8("settings_td_set"));
        settings_td_set->setMinimumSize(QSize(0, 40));
        settings_td_set->setFont(font1);
        settings_td_set->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_17->addWidget(settings_td_set, 2, 0, 1, 2);


        verticalLayout_11->addWidget(settings_td);

        settings_bri = new QGroupBox(panel_settings_content);
        settings_bri->setObjectName(QString::fromUtf8("settings_bri"));
        settings_bri->setFont(font1);
        settings_bri->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        settings_bri->setFlat(true);
        gridLayout_18 = new QGridLayout(settings_bri);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        gridLayout_18->setVerticalSpacing(20);
        settings_bri_t_disp = new QLabel(settings_bri);
        settings_bri_t_disp->setObjectName(QString::fromUtf8("settings_bri_t_disp"));
        settings_bri_t_disp->setFont(font1);
        settings_bri_t_disp->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_18->addWidget(settings_bri_t_disp, 0, 0, 1, 1);

        settings_bri_disp = new QComboBox(settings_bri);
        settings_bri_disp->addItem(QString());
        settings_bri_disp->addItem(QString());
        settings_bri_disp->addItem(QString());
        settings_bri_disp->setObjectName(QString::fromUtf8("settings_bri_disp"));
        settings_bri_disp->setFont(font3);
        settings_bri_disp->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        settings_bri_disp->setFrame(false);

        gridLayout_18->addWidget(settings_bri_disp, 0, 1, 1, 1);

        settings_bri_t_led = new QLabel(settings_bri);
        settings_bri_t_led->setObjectName(QString::fromUtf8("settings_bri_t_led"));
        settings_bri_t_led->setFont(font1);
        settings_bri_t_led->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_18->addWidget(settings_bri_t_led, 1, 0, 1, 1);

        settings_bri_led = new QComboBox(settings_bri);
        settings_bri_led->addItem(QString());
        settings_bri_led->addItem(QString());
        settings_bri_led->addItem(QString());
        settings_bri_led->setObjectName(QString::fromUtf8("settings_bri_led"));
        settings_bri_led->setFont(font3);
        settings_bri_led->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        settings_bri_led->setFrame(false);

        gridLayout_18->addWidget(settings_bri_led, 1, 1, 1, 1);

        settings_bri_set = new QPushButton(settings_bri);
        settings_bri_set->setObjectName(QString::fromUtf8("settings_bri_set"));
        settings_bri_set->setMinimumSize(QSize(0, 40));
        settings_bri_set->setFont(font1);
        settings_bri_set->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_18->addWidget(settings_bri_set, 2, 0, 1, 2);


        verticalLayout_11->addWidget(settings_bri);

        settings_presets = new QGroupBox(panel_settings_content);
        settings_presets->setObjectName(QString::fromUtf8("settings_presets"));
        settings_presets->setFont(font1);
        settings_presets->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        settings_presets->setFlat(true);
        gridLayout_16 = new QGridLayout(settings_presets);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        gridLayout_16->setVerticalSpacing(20);
        settings_presets_en_static = new QCheckBox(settings_presets);
        settings_presets_en_static->setObjectName(QString::fromUtf8("settings_presets_en_static"));
        settings_presets_en_static->setEnabled(false);
        settings_presets_en_static->setFont(font1);
        settings_presets_en_static->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));
        settings_presets_en_static->setChecked(true);

        gridLayout_16->addWidget(settings_presets_en_static, 0, 0, 1, 1);

        settings_presets_en_chase = new QCheckBox(settings_presets);
        settings_presets_en_chase->setObjectName(QString::fromUtf8("settings_presets_en_chase"));
        settings_presets_en_chase->setFont(font1);
        settings_presets_en_chase->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));
        settings_presets_en_chase->setChecked(true);

        gridLayout_16->addWidget(settings_presets_en_chase, 0, 1, 1, 1);

        settings_presets_en_bliss = new QCheckBox(settings_presets);
        settings_presets_en_bliss->setObjectName(QString::fromUtf8("settings_presets_en_bliss"));
        settings_presets_en_bliss->setFont(font1);
        settings_presets_en_bliss->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));
        settings_presets_en_bliss->setChecked(true);

        gridLayout_16->addWidget(settings_presets_en_bliss, 1, 0, 1, 1);

        settings_presets_en_tcode = new QCheckBox(settings_presets);
        settings_presets_en_tcode->setObjectName(QString::fromUtf8("settings_presets_en_tcode"));
        settings_presets_en_tcode->setFont(font1);
        settings_presets_en_tcode->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));
        settings_presets_en_tcode->setChecked(true);

        gridLayout_16->addWidget(settings_presets_en_tcode, 1, 1, 1, 1);

        settings_presets_en_spectrum = new QCheckBox(settings_presets);
        settings_presets_en_spectrum->setObjectName(QString::fromUtf8("settings_presets_en_spectrum"));
        settings_presets_en_spectrum->setFont(font1);
        settings_presets_en_spectrum->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));
        settings_presets_en_spectrum->setChecked(true);

        gridLayout_16->addWidget(settings_presets_en_spectrum, 2, 0, 1, 1);

        settings_presets_en_music = new QCheckBox(settings_presets);
        settings_presets_en_music->setObjectName(QString::fromUtf8("settings_presets_en_music"));
        settings_presets_en_music->setFont(font1);
        settings_presets_en_music->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_16->addWidget(settings_presets_en_music, 2, 1, 1, 1);

        settings_presets_en_rnb = new QCheckBox(settings_presets);
        settings_presets_en_rnb->setObjectName(QString::fromUtf8("settings_presets_en_rnb"));
        settings_presets_en_rnb->setFont(font1);
        settings_presets_en_rnb->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));
        settings_presets_en_rnb->setChecked(true);

        gridLayout_16->addWidget(settings_presets_en_rnb, 3, 0, 1, 1);

        settings_presets_en_cop = new QCheckBox(settings_presets);
        settings_presets_en_cop->setObjectName(QString::fromUtf8("settings_presets_en_cop"));
        settings_presets_en_cop->setFont(font1);
        settings_presets_en_cop->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_16->addWidget(settings_presets_en_cop, 3, 1, 1, 1);

        settings_presets_en_set = new QPushButton(settings_presets);
        settings_presets_en_set->setObjectName(QString::fromUtf8("settings_presets_en_set"));
        settings_presets_en_set->setMinimumSize(QSize(0, 40));
        settings_presets_en_set->setFont(font1);
        settings_presets_en_set->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_16->addWidget(settings_presets_en_set, 4, 0, 1, 2);


        verticalLayout_11->addWidget(settings_presets);

        settings_nsh = new QGroupBox(panel_settings_content);
        settings_nsh->setObjectName(QString::fromUtf8("settings_nsh"));
        settings_nsh->setFont(font1);
        settings_nsh->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        settings_nsh->setFlat(true);
        verticalLayout_5 = new QVBoxLayout(settings_nsh);
        verticalLayout_5->setSpacing(20);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        settings_nsh_enable = new QCheckBox(settings_nsh);
        settings_nsh_enable->setObjectName(QString::fromUtf8("settings_nsh_enable"));
        settings_nsh_enable->setFont(font1);
        settings_nsh_enable->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));

        verticalLayout_5->addWidget(settings_nsh_enable);

        settings_nsh_panel = new QWidget(settings_nsh);
        settings_nsh_panel->setObjectName(QString::fromUtf8("settings_nsh_panel"));
        settings_nsh_panel->setEnabled(false);
        settings_nsh_panel->setFont(font1);
        gridLayout_14 = new QGridLayout(settings_nsh_panel);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        settings_nsh_intro1 = new QLabel(settings_nsh_panel);
        settings_nsh_intro1->setObjectName(QString::fromUtf8("settings_nsh_intro1"));
        settings_nsh_intro1->setFont(font3);
        settings_nsh_intro1->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_14->addWidget(settings_nsh_intro1, 0, 0, 1, 2);

        settings_nsh_start = new QTimeEdit(settings_nsh_panel);
        settings_nsh_start->setObjectName(QString::fromUtf8("settings_nsh_start"));
        settings_nsh_start->setFont(font3);
        settings_nsh_start->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        settings_nsh_start->setTime(QTime(22, 0, 0));

        gridLayout_14->addWidget(settings_nsh_start, 0, 2, 1, 1);

        settings_nsh_intro2 = new QLabel(settings_nsh_panel);
        settings_nsh_intro2->setObjectName(QString::fromUtf8("settings_nsh_intro2"));
        settings_nsh_intro2->setFont(font3);
        settings_nsh_intro2->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_14->addWidget(settings_nsh_intro2, 1, 0, 1, 1);

        settings_nsh_stop = new QTimeEdit(settings_nsh_panel);
        settings_nsh_stop->setObjectName(QString::fromUtf8("settings_nsh_stop"));
        settings_nsh_stop->setFont(font3);
        settings_nsh_stop->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayMedium\";"));
        settings_nsh_stop->setTime(QTime(8, 0, 0));

        gridLayout_14->addWidget(settings_nsh_stop, 1, 1, 1, 1);

        settings_nsh_intro3 = new QLabel(settings_nsh_panel);
        settings_nsh_intro3->setObjectName(QString::fromUtf8("settings_nsh_intro3"));
        settings_nsh_intro3->setFont(font3);
        settings_nsh_intro3->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_14->addWidget(settings_nsh_intro3, 1, 2, 1, 1);


        verticalLayout_5->addWidget(settings_nsh_panel);

        settings_nsh_set = new QPushButton(settings_nsh);
        settings_nsh_set->setObjectName(QString::fromUtf8("settings_nsh_set"));
        settings_nsh_set->setMinimumSize(QSize(0, 40));
        settings_nsh_set->setFont(font1);
        settings_nsh_set->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 12px \"RalewayBold\";"));

        verticalLayout_5->addWidget(settings_nsh_set);


        verticalLayout_11->addWidget(settings_nsh);

        settings_settings = new QGroupBox(panel_settings_content);
        settings_settings->setObjectName(QString::fromUtf8("settings_settings"));
        settings_settings->setFont(font1);
        settings_settings->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        settings_settings->setFlat(true);
        verticalLayout_10 = new QVBoxLayout(settings_settings);
        verticalLayout_10->setSpacing(20);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        settings_settings_default = new QPushButton(settings_settings);
        settings_settings_default->setObjectName(QString::fromUtf8("settings_settings_default"));
        settings_settings_default->setMinimumSize(QSize(0, 40));
        settings_settings_default->setFont(font1);
        settings_settings_default->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);font: 12px \"RalewayBold\";"));

        verticalLayout_10->addWidget(settings_settings_default);

        settings_settings_save = new QPushButton(settings_settings);
        settings_settings_save->setObjectName(QString::fromUtf8("settings_settings_save"));
        settings_settings_save->setMinimumSize(QSize(0, 40));
        settings_settings_save->setFont(font1);
        settings_settings_save->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);font: 12px \"RalewayBold\";"));

        verticalLayout_10->addWidget(settings_settings_save);


        verticalLayout_11->addWidget(settings_settings);

        settings_info = new QGroupBox(panel_settings_content);
        settings_info->setObjectName(QString::fromUtf8("settings_info"));
        settings_info->setFont(font1);
        settings_info->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        settings_info->setFlat(true);
        gridLayout_15 = new QGridLayout(settings_info);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        gridLayout_15->setVerticalSpacing(20);
        settings_info_hw_t = new QLabel(settings_info);
        settings_info_hw_t->setObjectName(QString::fromUtf8("settings_info_hw_t"));
        settings_info_hw_t->setFont(font1);
        settings_info_hw_t->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_15->addWidget(settings_info_hw_t, 0, 0, 1, 1);

        settings_info_hw = new QLabel(settings_info);
        settings_info_hw->setObjectName(QString::fromUtf8("settings_info_hw"));
        settings_info_hw->setFont(font3);
        settings_info_hw->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_15->addWidget(settings_info_hw, 0, 1, 1, 1);

        settings_info_fw_t = new QLabel(settings_info);
        settings_info_fw_t->setObjectName(QString::fromUtf8("settings_info_fw_t"));
        settings_info_fw_t->setFont(font1);
        settings_info_fw_t->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_15->addWidget(settings_info_fw_t, 1, 0, 1, 1);

        settings_info_fw = new QLabel(settings_info);
        settings_info_fw->setObjectName(QString::fromUtf8("settings_info_fw"));
        settings_info_fw->setFont(font3);
        settings_info_fw->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_15->addWidget(settings_info_fw, 1, 1, 1, 1);

        settings_info_app_t = new QLabel(settings_info);
        settings_info_app_t->setObjectName(QString::fromUtf8("settings_info_app_t"));
        settings_info_app_t->setFont(font1);
        settings_info_app_t->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayBold\";"));

        gridLayout_15->addWidget(settings_info_app_t, 2, 0, 1, 1);

        settings_info_app = new ClickableLabel(settings_info);
        settings_info_app->setObjectName(QString::fromUtf8("settings_info_app"));
        QFont font5;
        font5.setFamily(QString::fromUtf8("RalewayMedium"));
        font5.setBold(false);
        font5.setItalic(false);
        font5.setUnderline(true);
        font5.setWeight(50);
        settings_info_app->setFont(font5);
        settings_info_app->setCursor(QCursor(Qt::OpenHandCursor));
        settings_info_app->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 12px \"RalewayMedium\";"));

        gridLayout_15->addWidget(settings_info_app, 2, 1, 1, 1);


        verticalLayout_11->addWidget(settings_info);

        settings_update = new QGroupBox(panel_settings_content);
        settings_update->setObjectName(QString::fromUtf8("settings_update"));
        settings_update->setFont(font1);
        settings_update->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));
        settings_update->setFlat(true);
        verticalLayout_9 = new QVBoxLayout(settings_update);
        verticalLayout_9->setSpacing(20);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        settings_info_download = new QPushButton(settings_update);
        settings_info_download->setObjectName(QString::fromUtf8("settings_info_download"));
        settings_info_download->setMinimumSize(QSize(0, 40));
        settings_info_download->setFont(font1);
        settings_info_download->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);font: 12px \"RalewayBold\";"));

        verticalLayout_9->addWidget(settings_info_download);

        settings_info_update = new QPushButton(settings_update);
        settings_info_update->setObjectName(QString::fromUtf8("settings_info_update"));
        settings_info_update->setMinimumSize(QSize(0, 40));
        settings_info_update->setFont(font1);
        settings_info_update->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);font: 12px \"RalewayBold\";"));

        verticalLayout_9->addWidget(settings_info_update);


        verticalLayout_11->addWidget(settings_update);

        panel_settings->setWidget(panel_settings_content);

        verticalLayout_17->addWidget(panel_settings);

        panel_timesync = new QScrollArea(control_parent);
        panel_timesync->setObjectName(QString::fromUtf8("panel_timesync"));
        panel_timesync->setMinimumSize(QSize(450, 0));
        panel_timesync->setFrameShape(QFrame::NoFrame);
        panel_timesync->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        panel_timesync->setWidgetResizable(true);
        panel_timesync_content = new QWidget();
        panel_timesync_content->setObjectName(QString::fromUtf8("panel_timesync_content"));
        panel_timesync_content->setGeometry(QRect(0, 0, 545, 426));
        verticalLayout_12 = new QVBoxLayout(panel_timesync_content);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalLayout_12->setContentsMargins(30, -1, 30, -1);
        timesync_time_label = new ClickableLabel(panel_timesync_content);
        timesync_time_label->setObjectName(QString::fromUtf8("timesync_time_label"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(timesync_time_label->sizePolicy().hasHeightForWidth());
        timesync_time_label->setSizePolicy(sizePolicy3);
        QFont font6;
        font6.setFamily(QString::fromUtf8("RalewayBlack"));
        font6.setBold(false);
        font6.setItalic(false);
        font6.setUnderline(false);
        font6.setWeight(50);
        timesync_time_label->setFont(font6);
        timesync_time_label->setCursor(QCursor(Qt::OpenHandCursor));
        timesync_time_label->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 96px \"RalewayBlack\";"));
        timesync_time_label->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(timesync_time_label);

        timesync_date_label = new QLabel(panel_timesync_content);
        timesync_date_label->setObjectName(QString::fromUtf8("timesync_date_label"));
        timesync_date_label->setMinimumSize(QSize(0, 40));
        timesync_date_label->setFont(font2);
        timesync_date_label->setStyleSheet(QString::fromUtf8("font: 24px \"RalewayBlack\";"));
        timesync_date_label->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(timesync_date_label);

        timesync_layout = new QWidget(panel_timesync_content);
        timesync_layout->setObjectName(QString::fromUtf8("timesync_layout"));
        gridLayout_19 = new QGridLayout(timesync_layout);
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        gridLayout_19->setContentsMargins(0, 0, 0, 0);
        timesync_label_6 = new QLabel(timesync_layout);
        timesync_label_6->setObjectName(QString::fromUtf8("timesync_label_6"));
        timesync_label_6->setFont(font1);
        timesync_label_6->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 24px \"RalewayBold\";"));
        timesync_label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_19->addWidget(timesync_label_6, 0, 0, 1, 1);

        timesync_p_y = new QSpinBox(timesync_layout);
        timesync_p_y->setObjectName(QString::fromUtf8("timesync_p_y"));
        timesync_p_y->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 24px \"RalewayBold\";"));
        timesync_p_y->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        timesync_p_y->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        timesync_p_y->setMinimum(-10);
        timesync_p_y->setMaximum(10);

        gridLayout_19->addWidget(timesync_p_y, 0, 1, 2, 1);

        timesync_p_mo = new QSpinBox(timesync_layout);
        timesync_p_mo->setObjectName(QString::fromUtf8("timesync_p_mo"));
        timesync_p_mo->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 24px \"RalewayBold\";"));
        timesync_p_mo->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        timesync_p_mo->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        timesync_p_mo->setMinimum(-24);
        timesync_p_mo->setMaximum(24);
        timesync_p_mo->setValue(0);

        gridLayout_19->addWidget(timesync_p_mo, 0, 2, 2, 1);

        timesync_p_d = new QSpinBox(timesync_layout);
        timesync_p_d->setObjectName(QString::fromUtf8("timesync_p_d"));
        timesync_p_d->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 24px \"RalewayBold\";"));
        timesync_p_d->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        timesync_p_d->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        timesync_p_d->setMinimum(-60);
        timesync_p_d->setMaximum(60);

        gridLayout_19->addWidget(timesync_p_d, 0, 3, 2, 1);

        timesync_p_h = new QSpinBox(timesync_layout);
        timesync_p_h->setObjectName(QString::fromUtf8("timesync_p_h"));
        timesync_p_h->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 24px \"RalewayBold\";"));
        timesync_p_h->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        timesync_p_h->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        timesync_p_h->setMinimum(-48);
        timesync_p_h->setMaximum(48);

        gridLayout_19->addWidget(timesync_p_h, 0, 4, 2, 1);

        timesync_p_m = new QSpinBox(timesync_layout);
        timesync_p_m->setObjectName(QString::fromUtf8("timesync_p_m"));
        timesync_p_m->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 24px \"RalewayBold\";"));
        timesync_p_m->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        timesync_p_m->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        timesync_p_m->setMinimum(-90);
        timesync_p_m->setMaximum(90);

        gridLayout_19->addWidget(timesync_p_m, 0, 5, 2, 1);

        timesync_label_7 = new QLabel(timesync_layout);
        timesync_label_7->setObjectName(QString::fromUtf8("timesync_label_7"));
        timesync_label_7->setFont(font1);
        timesync_label_7->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 24px \"RalewayBold\";"));
        timesync_label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_19->addWidget(timesync_label_7, 1, 0, 2, 1);

        timesync_label = new QLabel(timesync_layout);
        timesync_label->setObjectName(QString::fromUtf8("timesync_label"));
        timesync_label->setFont(font1);
        timesync_label->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 18px \"RalewayBold\";"));
        timesync_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_19->addWidget(timesync_label, 2, 1, 1, 1);

        timesync_label_2 = new QLabel(timesync_layout);
        timesync_label_2->setObjectName(QString::fromUtf8("timesync_label_2"));
        timesync_label_2->setFont(font1);
        timesync_label_2->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 18px \"RalewayBold\";"));
        timesync_label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_19->addWidget(timesync_label_2, 2, 2, 1, 1);

        timesync_label_3 = new QLabel(timesync_layout);
        timesync_label_3->setObjectName(QString::fromUtf8("timesync_label_3"));
        timesync_label_3->setFont(font1);
        timesync_label_3->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 18px \"RalewayBold\";"));
        timesync_label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_19->addWidget(timesync_label_3, 2, 3, 1, 1);

        timesync_label_4 = new QLabel(timesync_layout);
        timesync_label_4->setObjectName(QString::fromUtf8("timesync_label_4"));
        timesync_label_4->setFont(font1);
        timesync_label_4->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 18px \"RalewayBold\";"));
        timesync_label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_19->addWidget(timesync_label_4, 2, 4, 1, 1);

        timesync_label_5 = new QLabel(timesync_layout);
        timesync_label_5->setObjectName(QString::fromUtf8("timesync_label_5"));
        timesync_label_5->setFont(font1);
        timesync_label_5->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);font: 18px \"RalewayBold\";"));
        timesync_label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_19->addWidget(timesync_label_5, 2, 5, 1, 1);


        verticalLayout_12->addWidget(timesync_layout);

        timesync_intro = new QLabel(panel_timesync_content);
        timesync_intro->setObjectName(QString::fromUtf8("timesync_intro"));
        timesync_intro->setFont(font1);
        timesync_intro->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));

        verticalLayout_12->addWidget(timesync_intro);

        timesync_button = new QPushButton(panel_timesync_content);
        timesync_button->setObjectName(QString::fromUtf8("timesync_button"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(timesync_button->sizePolicy().hasHeightForWidth());
        timesync_button->setSizePolicy(sizePolicy4);
        timesync_button->setMinimumSize(QSize(0, 100));
        timesync_button->setFont(font2);
        timesync_button->setStyleSheet(QString::fromUtf8("background-color: rgb(16, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"font: 36px \"RalewayBlack\";"));
        timesync_button->setFlat(false);

        verticalLayout_12->addWidget(timesync_button);

        timesync_intro_2 = new QLabel(panel_timesync_content);
        timesync_intro_2->setObjectName(QString::fromUtf8("timesync_intro_2"));
        timesync_intro_2->setFont(font1);
        timesync_intro_2->setStyleSheet(QString::fromUtf8("font: 12px \"RalewayBold\";"));

        verticalLayout_12->addWidget(timesync_intro_2);

        verticalLayout_12->setStretch(0, 3);
        verticalLayout_12->setStretch(1, 2);
        verticalLayout_12->setStretch(2, 2);
        verticalLayout_12->setStretch(3, 1);
        verticalLayout_12->setStretch(4, 3);
        verticalLayout_12->setStretch(5, 1);
        panel_timesync->setWidget(panel_timesync_content);

        verticalLayout_17->addWidget(panel_timesync);

        panel_welcome = new QScrollArea(control_parent);
        panel_welcome->setObjectName(QString::fromUtf8("panel_welcome"));
        panel_welcome->setMinimumSize(QSize(450, 0));
        panel_welcome->setFrameShape(QFrame::NoFrame);
        panel_welcome->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        panel_welcome->setWidgetResizable(true);
        panel_welcome_content = new QWidget();
        panel_welcome_content->setObjectName(QString::fromUtf8("panel_welcome_content"));
        panel_welcome_content->setGeometry(QRect(0, 0, 545, 251));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(panel_welcome_content->sizePolicy().hasHeightForWidth());
        panel_welcome_content->setSizePolicy(sizePolicy5);
        verticalLayout_2 = new QVBoxLayout(panel_welcome_content);
        verticalLayout_2->setSpacing(20);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(40, 20, 40, 20);
        widget_2 = new QWidget(panel_welcome_content);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));

        verticalLayout_2->addWidget(widget_2);

        com_intro = new QLabel(panel_welcome_content);
        com_intro->setObjectName(QString::fromUtf8("com_intro"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(com_intro->sizePolicy().hasHeightForWidth());
        com_intro->setSizePolicy(sizePolicy6);
        com_intro->setMaximumSize(QSize(16777215, 50));
        com_intro->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 18px \"RalewayBlack\";"));
        com_intro->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_2->addWidget(com_intro);

        com_select = new QComboBox(panel_welcome_content);
        com_select->setObjectName(QString::fromUtf8("com_select"));
        sizePolicy6.setHeightForWidth(com_select->sizePolicy().hasHeightForWidth());
        com_select->setSizePolicy(sizePolicy6);
        com_select->setMinimumSize(QSize(0, 75));
        com_select->setFont(font1);
        com_select->setStyleSheet(QString::fromUtf8("font: 16px \"RalewayBold\";"));
        com_select->setFrame(true);

        verticalLayout_2->addWidget(com_select);

        com_control_dock = new QWidget(panel_welcome_content);
        com_control_dock->setObjectName(QString::fromUtf8("com_control_dock"));
        sizePolicy6.setHeightForWidth(com_control_dock->sizePolicy().hasHeightForWidth());
        com_control_dock->setSizePolicy(sizePolicy6);
        com_control_dock->setMaximumSize(QSize(16777215, 80));
        horizontalLayout = new QHBoxLayout(com_control_dock);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        com_reload = new ClickableLabel(com_control_dock);
        com_reload->setObjectName(QString::fromUtf8("com_reload"));
        com_reload->setMaximumSize(QSize(40, 40));
        com_reload->setCursor(QCursor(Qt::OpenHandCursor));
        com_reload->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 36px \"RalewayBlack\";"));
        com_reload->setPixmap(QPixmap(QString::fromUtf8(":/Resources/reload.png")));
        com_reload->setScaledContents(true);
        com_reload->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(com_reload, 0, Qt::AlignLeft|Qt::AlignVCenter);

        widget = new QWidget(com_control_dock);
        widget->setObjectName(QString::fromUtf8("widget"));

        horizontalLayout->addWidget(widget);

        com_connect = new ClickableLabel(com_control_dock);
        com_connect->setObjectName(QString::fromUtf8("com_connect"));
        com_connect->setMaximumSize(QSize(40, 40));
        com_connect->setCursor(QCursor(Qt::OpenHandCursor));
        com_connect->setStyleSheet(QString::fromUtf8("color: rgb(16, 128, 128);\n"
"font: 36px \"RalewayBlack\";"));
        com_connect->setScaledContents(true);
        com_connect->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(com_connect, 0, Qt::AlignRight|Qt::AlignVCenter);


        verticalLayout_2->addWidget(com_control_dock);

        widget_3 = new QWidget(panel_welcome_content);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));

        verticalLayout_2->addWidget(widget_3);

        verticalLayout_2->setStretch(0, 2);
        verticalLayout_2->setStretch(1, 3);
        verticalLayout_2->setStretch(2, 3);
        verticalLayout_2->setStretch(3, 3);
        verticalLayout_2->setStretch(4, 3);
        panel_welcome->setWidget(panel_welcome_content);

        verticalLayout_17->addWidget(panel_welcome);


        horizontalLayout_9->addWidget(control_parent);


        verticalLayout_18->addWidget(parent_menu_control);

        com_label_connect = new QLabel(centralWidget);
        com_label_connect->setObjectName(QString::fromUtf8("com_label_connect"));
        com_label_connect->setMinimumSize(QSize(0, 21));
        com_label_connect->setFont(font1);
        com_label_connect->setStyleSheet(QString::fromUtf8("color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayBold\";"));
        com_label_connect->setScaledContents(false);
        com_label_connect->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        com_label_connect->setMargin(0);
        com_label_connect->setIndent(10);

        verticalLayout_18->addWidget(com_label_connect);

        FluorescenceApp->setCentralWidget(centralWidget);
        com_label_connect->raise();
        menu_layout->raise();
        parent_menu_control->raise();

        retranslateUi(FluorescenceApp);

        tab_presets_container->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(FluorescenceApp);
    } // setupUi

    void retranslateUi(QMainWindow *FluorescenceApp)
    {
        FluorescenceApp->setWindowTitle(QCoreApplication::translate("FluorescenceApp", "FluorescenceApp", nullptr));
        menu_button->setText(QString());
        menu_about->setText(QCoreApplication::translate("FluorescenceApp", "Fluorescence App   ", nullptr));
        menu_about_2->setText(QCoreApplication::translate("FluorescenceApp", "\n"
"   T H E   V F D   C O L L E C T I V E", nullptr));
        menu_button_balancer->setText(QString());
        main_welcome->setText(QCoreApplication::translate("FluorescenceApp", "Device", nullptr));
        main_timesync->setText(QCoreApplication::translate("FluorescenceApp", "Time", nullptr));
        main_preset->setText(QCoreApplication::translate("FluorescenceApp", "Color Presets", nullptr));
        main_custom->setText(QCoreApplication::translate("FluorescenceApp", "Custom Color", nullptr));
        main_settings->setText(QCoreApplication::translate("FluorescenceApp", "Preferences", nullptr));
        main_message->setText(QCoreApplication::translate("FluorescenceApp", "Message", nullptr));
        static_intro->setText(QCoreApplication::translate("FluorescenceApp", "Some hand crafted presets that look fantastic on Fluorescence.", nullptr));
        static_off->setText(QCoreApplication::translate("FluorescenceApp", "#off", nullptr));
        static_white->setText(QCoreApplication::translate("FluorescenceApp", "#white", nullptr));
        static_warmwhite->setText(QCoreApplication::translate("FluorescenceApp", "#warmwhite", nullptr));
        static_rainbow->setText(QString());
        static_pastelrainbow->setText(QString());
        static_red2blue->setText(QString());
        static_red2green->setText(QString());
        static_green2blue->setText(QString());
        static_blue->setText(QCoreApplication::translate("FluorescenceApp", "#blue", nullptr));
        static_cyan->setText(QCoreApplication::translate("FluorescenceApp", "#cyan", nullptr));
        static_green->setText(QCoreApplication::translate("FluorescenceApp", "#green", nullptr));
        static_yellow->setText(QCoreApplication::translate("FluorescenceApp", "#yellow", nullptr));
        static_orange->setText(QCoreApplication::translate("FluorescenceApp", "#orange", nullptr));
        static_red->setText(QCoreApplication::translate("FluorescenceApp", "#red", nullptr));
        static_magenta->setText(QCoreApplication::translate("FluorescenceApp", "#magenta", nullptr));
        static_violet->setText(QCoreApplication::translate("FluorescenceApp", "#violet", nullptr));
        tab_presets_container->setTabText(tab_presets_container->indexOf(tab_presets_static), QCoreApplication::translate("FluorescenceApp", "Static Presets", nullptr));
        dynamic_bliss_dnc->setText(QCoreApplication::translate("FluorescenceApp", "Moments of Bliss", nullptr));
        dynamic_spectrum->setText(QCoreApplication::translate("FluorescenceApp", "Spectrum", nullptr));
        dynamic_rainbow->setText(QCoreApplication::translate("FluorescenceApp", "Rainbow Fade", nullptr));
        dynamic_chase->setText(QCoreApplication::translate("FluorescenceApp", "Chase Fade", nullptr));
        dynamic_music->setText(QCoreApplication::translate("FluorescenceApp", "Music", nullptr));
        dynamic_timecode->setText(QCoreApplication::translate("FluorescenceApp", "Time Code", nullptr));
        dynamic_police->setText(QCoreApplication::translate("FluorescenceApp", "Police Lights", nullptr));
        panel_dyn_rnb_ptext_1->setText(QCoreApplication::translate("FluorescenceApp", "distance:", nullptr));
        panel_dyn_rnb_p1_1->setText(QCoreApplication::translate("FluorescenceApp", "chill", nullptr));
        panel_dyn_rnb_p1_2->setText(QCoreApplication::translate("FluorescenceApp", "tight", nullptr));
        panel_dyn_rnb_ptext_2->setText(QCoreApplication::translate("FluorescenceApp", "saturation:", nullptr));
        panel_dyn_rnb_param2->setItemText(0, QCoreApplication::translate("FluorescenceApp", "high", nullptr));
        panel_dyn_rnb_param2->setItemText(1, QCoreApplication::translate("FluorescenceApp", "medium", nullptr));
        panel_dyn_rnb_param2->setItemText(2, QCoreApplication::translate("FluorescenceApp", "low", nullptr));

        panel_dyn_rnb_set->setText(QCoreApplication::translate("FluorescenceApp", "Set", nullptr));
        panel_dyn_spectrum_ptext1->setText(QCoreApplication::translate("FluorescenceApp", "saturation:", nullptr));
        panel_dyn_spectrum_set->setText(QCoreApplication::translate("FluorescenceApp", "Set", nullptr));
        panel_dyn_spectrum_ptext2->setText(QCoreApplication::translate("FluorescenceApp", "lightness:", nullptr));
        panel_dyn_spectrum_param_2->setItemText(0, QCoreApplication::translate("FluorescenceApp", "medium", nullptr));
        panel_dyn_spectrum_param_2->setItemText(1, QCoreApplication::translate("FluorescenceApp", "high", nullptr));
        panel_dyn_spectrum_param_2->setItemText(2, QCoreApplication::translate("FluorescenceApp", "low", nullptr));

        panel_dyn_spectrum_intro->setText(QCoreApplication::translate("FluorescenceApp", "Guides you through\n"
"all the amazing colors.", nullptr));
        panel_dyn_spectrum_param_1->setItemText(0, QCoreApplication::translate("FluorescenceApp", "high", nullptr));
        panel_dyn_spectrum_param_1->setItemText(1, QCoreApplication::translate("FluorescenceApp", "medium", nullptr));
        panel_dyn_spectrum_param_1->setItemText(2, QCoreApplication::translate("FluorescenceApp", "low", nullptr));

        panel_dyn_chase_ptext_1->setText(QCoreApplication::translate("FluorescenceApp", "distance:", nullptr));
        panel_dyn_chase_p1->setText(QCoreApplication::translate("FluorescenceApp", "chill", nullptr));
        panel_dyn_chase_p2->setText(QCoreApplication::translate("FluorescenceApp", "tight", nullptr));
        panel_dyn_chase_ptext_2->setText(QCoreApplication::translate("FluorescenceApp", "mode:", nullptr));
        panel_dyn_chase_param2->setItemText(0, QCoreApplication::translate("FluorescenceApp", "from right to left", nullptr));
        panel_dyn_chase_param2->setItemText(1, QCoreApplication::translate("FluorescenceApp", "from left to right", nullptr));
        panel_dyn_chase_param2->setItemText(2, QCoreApplication::translate("FluorescenceApp", "flip direction every second", nullptr));
        panel_dyn_chase_param2->setItemText(3, QCoreApplication::translate("FluorescenceApp", "flip sound sensitive", nullptr));

        panel_dyn_chase_set->setText(QCoreApplication::translate("FluorescenceApp", "Set", nullptr));
        panel_dyn_tcode_intro->setText(QCoreApplication::translate("FluorescenceApp", "For every digit value, there is a different color!", nullptr));
        panel_dyn_tcode_ptext->setText(QCoreApplication::translate("FluorescenceApp", "color palette:", nullptr));
        panel_dyn_tcode_param1->setItemText(0, QCoreApplication::translate("FluorescenceApp", "resistor value code", nullptr));
        panel_dyn_tcode_param1->setItemText(1, QCoreApplication::translate("FluorescenceApp", "rainbow", nullptr));

        panel_dyn_tcode_set->setText(QCoreApplication::translate("FluorescenceApp", "Set", nullptr));
        panel_dyn_music_ptext_1->setText(QCoreApplication::translate("FluorescenceApp", "distance:", nullptr));
        panel_dyn_music_p2->setText(QCoreApplication::translate("FluorescenceApp", "tight", nullptr));
        panel_dyn_music_set->setText(QCoreApplication::translate("FluorescenceApp", "Set", nullptr));
        panel_dyn_music_p1->setText(QCoreApplication::translate("FluorescenceApp", "chill", nullptr));
        panel_dyn_music_param2->setItemText(0, QCoreApplication::translate("FluorescenceApp", "high", nullptr));
        panel_dyn_music_param2->setItemText(1, QCoreApplication::translate("FluorescenceApp", "medium", nullptr));
        panel_dyn_music_param2->setItemText(2, QCoreApplication::translate("FluorescenceApp", "low", nullptr));

        panel_dyn_music_ptext_2->setText(QCoreApplication::translate("FluorescenceApp", "saturation:", nullptr));
        panel_dyn_cop_intro->setText(QCoreApplication::translate("FluorescenceApp", "Should Fluorescence be on patrol or a high speed, hot pursuit?", nullptr));
        panel_dyn_cop_ptext->setText(QCoreApplication::translate("FluorescenceApp", "pattern:", nullptr));
        panel_dyn_cop_param1->setItemText(0, QCoreApplication::translate("FluorescenceApp", "patrol", nullptr));
        panel_dyn_cop_param1->setItemText(1, QCoreApplication::translate("FluorescenceApp", "high speed pursuit", nullptr));
        panel_dyn_cop_param1->setItemText(2, QCoreApplication::translate("FluorescenceApp", "german police", nullptr));

        panel_dyn_cop_set->setText(QCoreApplication::translate("FluorescenceApp", "Set", nullptr));
        panel_dyn_bliss_param->setItemText(0, QCoreApplication::translate("FluorescenceApp", "nordlicht", nullptr));
        panel_dyn_bliss_param->setItemText(1, QCoreApplication::translate("FluorescenceApp", "pastellfr\303\274hling", nullptr));
        panel_dyn_bliss_param->setItemText(2, QCoreApplication::translate("FluorescenceApp", "hummelhonig", nullptr));
        panel_dyn_bliss_param->setItemText(3, QCoreApplication::translate("FluorescenceApp", "meeresgefl\303\274ster", nullptr));
        panel_dyn_bliss_param->setItemText(4, QCoreApplication::translate("FluorescenceApp", "herbstlagerfeuer", nullptr));
        panel_dyn_bliss_param->setItemText(5, QCoreApplication::translate("FluorescenceApp", "abendhimmel", nullptr));

        panel_dyn_bliss_ptext->setText(QCoreApplication::translate("FluorescenceApp", "mood:", nullptr));
        panel_dyn_bliss_set->setText(QCoreApplication::translate("FluorescenceApp", "Set", nullptr));
        panel_dyn_bliss_description->setText(QCoreApplication::translate("FluorescenceApp", "Means wumbo in German. i wumbo, you wumbo\n"
"he she me wumbo. That's first grade Spongebob!", nullptr));
        dynamic_intro->setText(QCoreApplication::translate("FluorescenceApp", "Animated preview. We know it's prettier on Fluorescence.", nullptr));
        dynamic_c1->setText(QString());
        dynamic_c2->setText(QString());
        dynamic_c3->setText(QString());
        dynamic_c4->setText(QString());
        dynamic_c5->setText(QString());
        dynamic_c6->setText(QString());
        dynamic_t_intro->setText(QCoreApplication::translate("FluorescenceApp", "Time:", nullptr));
        dynamic_t_1->setText(QString());
        dynamic_t_2->setText(QString());
        dynamic_t_3->setText(QString());
        dynamic_t_4->setText(QString());
        dynamic_t_5->setText(QString());
        dynamic_t_6->setText(QString());
        dynamic_t_7->setText(QString());
        dynamic_t_8->setText(QString());
        dynamic_t_9->setText(QString());
        dynamic_t_10->setText(QString());
        dynamic_t_11->setText(QString());
        dynamic_t_12->setText(QString());
        dynamic_t_13->setText(QString());
        dynamic_t_14->setText(QString());
        dynamic_t_15->setText(QString());
        dynamic_t_16->setText(QString());
        dynamic_t_17->setText(QString());
        dynamic_t_18->setText(QString());
        dynamic_t_19->setText(QString());
        dynamic_t_20->setText(QString());
        dynamic_t_21->setText(QString());
        tab_presets_container->setTabText(tab_presets_container->indexOf(tab_presets_dynamic), QCoreApplication::translate("FluorescenceApp", "Dynamic Presets", nullptr));
        lisync_intro->setText(QCoreApplication::translate("FluorescenceApp", "Generate ambient light colors to the mood of your computer screen.", nullptr));
        lisync_computer->setText(QString());
        lisync_sample->setText(QString());
        lisync_c1->setText(QString());
        lisync_c2->setText(QString());
        lisync_c3->setText(QString());
        lisync_c4->setText(QString());
        lisync_c5->setText(QString());
        lisync_c6->setText(QString());
        lisync_status->setText(QCoreApplication::translate("FluorescenceApp", "Click on the computer to begin.", nullptr));
        tab_presets_container->setTabText(tab_presets_container->indexOf(tab_moodsync), QCoreApplication::translate("FluorescenceApp", "Ambient Light", nullptr));
        shuffle_intro1->setText(QCoreApplication::translate("FluorescenceApp", "Not in the mood to pick?", nullptr));
        shuffle_intro3->setText(QCoreApplication::translate("FluorescenceApp", "Not a problem! Shuffle to let Fluorescence pick a pattern,\n"
"randomly out of:", nullptr));
        shuffle_en_static->setText(QCoreApplication::translate("FluorescenceApp", "Static", nullptr));
        shuffle_en_chase->setText(QCoreApplication::translate("FluorescenceApp", "Chase Fade", nullptr));
        shuffle_en_bliss->setText(QCoreApplication::translate("FluorescenceApp", "Moments of Bliss", nullptr));
        shuffle_en_tcode->setText(QCoreApplication::translate("FluorescenceApp", "Time Code", nullptr));
        shuffle_en_spectrum->setText(QCoreApplication::translate("FluorescenceApp", "Spectrum Fade", nullptr));
        shuffle_en_music->setText(QCoreApplication::translate("FluorescenceApp", "Music", nullptr));
        shuffle_en_rnb->setText(QCoreApplication::translate("FluorescenceApp", "Rainbow Fade", nullptr));
        shuffle_en_cop->setText(QCoreApplication::translate("FluorescenceApp", "Police Lights", nullptr));
        shuffle_intro2->setText(QCoreApplication::translate("FluorescenceApp", "and pick another one every...", nullptr));
        shuffle_speed->setItemText(0, QCoreApplication::translate("FluorescenceApp", "minute", nullptr));
        shuffle_speed->setItemText(1, QCoreApplication::translate("FluorescenceApp", "other minute", nullptr));
        shuffle_speed->setItemText(2, QCoreApplication::translate("FluorescenceApp", "ten minutes", nullptr));
        shuffle_speed->setItemText(3, QCoreApplication::translate("FluorescenceApp", "half hour", nullptr));
        shuffle_speed->setItemText(4, QCoreApplication::translate("FluorescenceApp", "hour", nullptr));

        shuffle_set->setText(QCoreApplication::translate("FluorescenceApp", "Looks good, go!", nullptr));
        tab_presets_container->setTabText(tab_presets_container->indexOf(tab_shuffle), QCoreApplication::translate("FluorescenceApp", "Shuffle", nullptr));
        custom_intro->setText(QCoreApplication::translate("FluorescenceApp", "Time to paint the night sky. Or some LEDs.", nullptr));
        custom_color_wheel_cursor->setText(QString());
        custom_color_wheel->setText(QString());
        custom_select_led_6->setText(QCoreApplication::translate("FluorescenceApp", "LED 6", nullptr));
        custom_select_led_5->setText(QCoreApplication::translate("FluorescenceApp", "LED 5", nullptr));
        custom_select_led_4->setText(QCoreApplication::translate("FluorescenceApp", "LED 4", nullptr));
        custom_select_led_3->setText(QCoreApplication::translate("FluorescenceApp", "LED 3", nullptr));
        custom_select_led_2->setText(QCoreApplication::translate("FluorescenceApp", "LED 2", nullptr));
        custom_select_led_1->setText(QCoreApplication::translate("FluorescenceApp", "LED 1", nullptr));
        custom_label_r_2->setText(QCoreApplication::translate("FluorescenceApp", "red, green, blue & white.", nullptr));
        custom_label_r->setText(QCoreApplication::translate("FluorescenceApp", "R:", nullptr));
        custom_label_g->setText(QCoreApplication::translate("FluorescenceApp", "G:", nullptr));
        custom_label_b->setText(QCoreApplication::translate("FluorescenceApp", "B:", nullptr));
        custom_label_w->setText(QCoreApplication::translate("FluorescenceApp", "W:", nullptr));
        custom_label_r_3->setText(QCoreApplication::translate("FluorescenceApp", "hue, saturation & lightness.", nullptr));
        custom_label_h->setText(QCoreApplication::translate("FluorescenceApp", "H:", nullptr));
        custom_label_s->setText(QCoreApplication::translate("FluorescenceApp", "S:", nullptr));
        custom_label_l->setText(QCoreApplication::translate("FluorescenceApp", "L:", nullptr));
        custom_label_hex->setText(QCoreApplication::translate("FluorescenceApp", "hex color code:", nullptr));
        custom_value_hex->setPlainText(QCoreApplication::translate("FluorescenceApp", "#C0FFEE", nullptr));
        custom_value_save->setText(QCoreApplication::translate("FluorescenceApp", "Save color", nullptr));
        custom_value_load->setText(QCoreApplication::translate("FluorescenceApp", "Load color", nullptr));
        lsettings_td->setTitle(QCoreApplication::translate("FluorescenceApp", "Set time and date format", nullptr));
        lsettings_td_12hr_enable->setText(QCoreApplication::translate("FluorescenceApp", "12h hour format", nullptr));
        lsettings_td_l0_enable->setText(QCoreApplication::translate("FluorescenceApp", "Enable leading zero '0'", nullptr));
        lsettings_td_intldate_enable->setText(QCoreApplication::translate("FluorescenceApp", "MM.DD.YY format", nullptr));
        lsettings_nsh->setTitle(QCoreApplication::translate("FluorescenceApp", "Brightness && Night Shift", nullptr));
        lsettings_nsh_enable->setText(QCoreApplication::translate("FluorescenceApp", "Enable silent mode", nullptr));
        lsettings_nsh_scheduled_enable->setText(QCoreApplication::translate("FluorescenceApp", "Scheduled night shift", nullptr));
        lsettings_nsh_intro1->setText(QCoreApplication::translate("FluorescenceApp", "Every evening from", nullptr));
        lsettings_nsh_start->setStyleSheet(QCoreApplication::translate("FluorescenceApp", "color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";", nullptr));
        lsettings_nsh_intro2->setText(QCoreApplication::translate("FluorescenceApp", "until", nullptr));
        lsettings_nsh_stop->setStyleSheet(QCoreApplication::translate("FluorescenceApp", "color: rgb(128, 0, 128);\n"
"font: 12px \"RalewayMedium\";", nullptr));
        lsettings_nsh_intro3->setText(QCoreApplication::translate("FluorescenceApp", "the next morning", nullptr));
        lsettings_settings->setTitle(QCoreApplication::translate("FluorescenceApp", "Clock Settings", nullptr));
        lsettings_settings_default->setText(QCoreApplication::translate("FluorescenceApp", "Load Default", nullptr));
        lsettings_settings_save->setText(QCoreApplication::translate("FluorescenceApp", "Save All", nullptr));
        lsettings_info->setTitle(QCoreApplication::translate("FluorescenceApp", "Hardware && Firmware", nullptr));
        lsettings_info_hw_t->setText(QCoreApplication::translate("FluorescenceApp", "Hardware version:", nullptr));
        lsettings_info_hw->setText(QCoreApplication::translate("FluorescenceApp", "2.0 or 2.1", nullptr));
        lsettings_info_fw_t->setText(QCoreApplication::translate("FluorescenceApp", "Firmware version:", nullptr));
        lsettings_info_fw->setText(QCoreApplication::translate("FluorescenceApp", "sw_str", nullptr));
        lsettings_info_app_t->setText(QCoreApplication::translate("FluorescenceApp", "Fluorescence app version:", nullptr));
        lsettings_info_app->setText(QCoreApplication::translate("FluorescenceApp", "3.0", nullptr));
        lsettings_info_notice->setText(QCoreApplication::translate("FluorescenceApp", "You are using an outdated version of firmware on Fluorescence. Firmware version 3.0 is available. Download the latest firmware and click on 'firmware update' to update to the latest version!", nullptr));
        lsettings_info_download->setText(QCoreApplication::translate("FluorescenceApp", "Download Latest Firmware", nullptr));
        lsettings_info_update->setText(QCoreApplication::translate("FluorescenceApp", "Firmware Update", nullptr));
        message_intro->setText(QCoreApplication::translate("FluorescenceApp", "Write a poem. Or say something lovely. We wrote this so that you can have a fun time :)", nullptr));
        message_text->setPlainText(QCoreApplication::translate("FluorescenceApp", "HEART HOPE  ", nullptr));
        message_intro_2->setText(QCoreApplication::translate("FluorescenceApp", "Rules of the game:", nullptr));
        message_intro_3->setText(QCoreApplication::translate("FluorescenceApp", "24 letters for you. The message will be split in max. 4 parts shown separately", nullptr));
        message_intro_4->setText(QCoreApplication::translate("FluorescenceApp", "Write in CAPS, and only use those the display can display (see next point)", nullptr));
        message_intro_5->setText(QCoreApplication::translate("FluorescenceApp", "Allowed: A B C D E F G H I J L N O P Q R S T U V Y, 0, spacebar and minus (-)", nullptr));
        message_intro_6->setText(QCoreApplication::translate("FluorescenceApp", "It's a bit like SNAPCHAT. But you decide for how long each message part is on display:", nullptr));
        message_set_custom_welcome->setText(QCoreApplication::translate("FluorescenceApp", "Use this message as the new welcome message\n"
"Fluorescence uses to greet you on startup", nullptr));
        message_intro_7->setText(QCoreApplication::translate("FluorescenceApp", "second(s)", nullptr));
        message_length->setPlainText(QCoreApplication::translate("FluorescenceApp", "1", nullptr));
        message_send->setText(QCoreApplication::translate("FluorescenceApp", "Send Message", nullptr));
        settings_td->setTitle(QCoreApplication::translate("FluorescenceApp", "Set time and date format", nullptr));
        settings_td_timemode_t->setText(QCoreApplication::translate("FluorescenceApp", "Hour format:", nullptr));
        settings_td_timemode->setItemText(0, QCoreApplication::translate("FluorescenceApp", "24h", nullptr));
        settings_td_timemode->setItemText(1, QCoreApplication::translate("FluorescenceApp", "12h", nullptr));
        settings_td_timemode->setItemText(2, QCoreApplication::translate("FluorescenceApp", "12h w/o leading 0", nullptr));

        settings_td_datemode_t->setText(QCoreApplication::translate("FluorescenceApp", "Date format:", nullptr));
        settings_td_datemode->setItemText(0, QCoreApplication::translate("FluorescenceApp", "DD.MM.YY", nullptr));
        settings_td_datemode->setItemText(1, QCoreApplication::translate("FluorescenceApp", "MM.DD.YY", nullptr));

        settings_td_set->setText(QCoreApplication::translate("FluorescenceApp", "Set", nullptr));
        settings_bri->setTitle(QCoreApplication::translate("FluorescenceApp", "Brightness", nullptr));
        settings_bri_t_disp->setText(QCoreApplication::translate("FluorescenceApp", "Display brightness:", nullptr));
        settings_bri_disp->setItemText(0, QCoreApplication::translate("FluorescenceApp", "Bright", nullptr));
        settings_bri_disp->setItemText(1, QCoreApplication::translate("FluorescenceApp", "Half Bright", nullptr));
        settings_bri_disp->setItemText(2, QCoreApplication::translate("FluorescenceApp", "Dim", nullptr));

        settings_bri_t_led->setText(QCoreApplication::translate("FluorescenceApp", "LED brightness:", nullptr));
        settings_bri_led->setItemText(0, QCoreApplication::translate("FluorescenceApp", "Bright", nullptr));
        settings_bri_led->setItemText(1, QCoreApplication::translate("FluorescenceApp", "Half Bright", nullptr));
        settings_bri_led->setItemText(2, QCoreApplication::translate("FluorescenceApp", "Dim", nullptr));

        settings_bri_set->setText(QCoreApplication::translate("FluorescenceApp", "Set", nullptr));
        settings_presets->setTitle(QCoreApplication::translate("FluorescenceApp", "Enabled presets", nullptr));
        settings_presets_en_static->setText(QCoreApplication::translate("FluorescenceApp", "Static", nullptr));
        settings_presets_en_chase->setText(QCoreApplication::translate("FluorescenceApp", "Chase Fade", nullptr));
        settings_presets_en_bliss->setText(QCoreApplication::translate("FluorescenceApp", "Moments of Bliss", nullptr));
        settings_presets_en_tcode->setText(QCoreApplication::translate("FluorescenceApp", "Time Code", nullptr));
        settings_presets_en_spectrum->setText(QCoreApplication::translate("FluorescenceApp", "Spectrum Fade", nullptr));
        settings_presets_en_music->setText(QCoreApplication::translate("FluorescenceApp", "Music", nullptr));
        settings_presets_en_rnb->setText(QCoreApplication::translate("FluorescenceApp", "Rainbow Fade", nullptr));
        settings_presets_en_cop->setText(QCoreApplication::translate("FluorescenceApp", "Police Lights", nullptr));
        settings_presets_en_set->setText(QCoreApplication::translate("FluorescenceApp", "Set", nullptr));
        settings_nsh->setTitle(QCoreApplication::translate("FluorescenceApp", "Night shift", nullptr));
        settings_nsh_enable->setText(QCoreApplication::translate("FluorescenceApp", "Enable night shift", nullptr));
        settings_nsh_intro1->setText(QCoreApplication::translate("FluorescenceApp", "Every evening from", nullptr));
        settings_nsh_intro2->setText(QCoreApplication::translate("FluorescenceApp", "until", nullptr));
        settings_nsh_intro3->setText(QCoreApplication::translate("FluorescenceApp", "next morning.", nullptr));
        settings_nsh_set->setText(QCoreApplication::translate("FluorescenceApp", "Set", nullptr));
        settings_settings->setTitle(QCoreApplication::translate("FluorescenceApp", "Clock Settings", nullptr));
        settings_settings_default->setText(QCoreApplication::translate("FluorescenceApp", "Load Default", nullptr));
        settings_settings_save->setText(QCoreApplication::translate("FluorescenceApp", "Save All", nullptr));
        settings_info->setTitle(QCoreApplication::translate("FluorescenceApp", "Hardware && Firmware Info", nullptr));
        settings_info_hw_t->setText(QCoreApplication::translate("FluorescenceApp", "Hardware version:", nullptr));
        settings_info_hw->setText(QCoreApplication::translate("FluorescenceApp", "hw_str", nullptr));
        settings_info_fw_t->setText(QCoreApplication::translate("FluorescenceApp", "Firmware version:", nullptr));
        settings_info_fw->setText(QCoreApplication::translate("FluorescenceApp", "sw_str", nullptr));
        settings_info_app_t->setText(QCoreApplication::translate("FluorescenceApp", "App version:", nullptr));
        settings_info_app->setText(QCoreApplication::translate("FluorescenceApp", "3.0", nullptr));
        settings_update->setTitle(QCoreApplication::translate("FluorescenceApp", "Updates", nullptr));
        settings_info_download->setText(QCoreApplication::translate("FluorescenceApp", "Download Latest Firmware", nullptr));
        settings_info_update->setText(QCoreApplication::translate("FluorescenceApp", "Firmware Update", nullptr));
#if QT_CONFIG(tooltip)
        timesync_time_label->setToolTip(QCoreApplication::translate("FluorescenceApp", "Click to reset adjustment", nullptr));
#endif // QT_CONFIG(tooltip)
        timesync_time_label->setText(QCoreApplication::translate("FluorescenceApp", "20:15:16", nullptr));
        timesync_date_label->setText(QCoreApplication::translate("FluorescenceApp", "Friyay, March 7th 2032", nullptr));
        timesync_label_6->setText(QCoreApplication::translate("FluorescenceApp", "+", nullptr));
        timesync_label_7->setText(QString());
        timesync_label->setText(QCoreApplication::translate("FluorescenceApp", "years", nullptr));
        timesync_label_2->setText(QCoreApplication::translate("FluorescenceApp", "months", nullptr));
        timesync_label_3->setText(QCoreApplication::translate("FluorescenceApp", "days", nullptr));
        timesync_label_4->setText(QCoreApplication::translate("FluorescenceApp", "hours", nullptr));
        timesync_label_5->setText(QCoreApplication::translate("FluorescenceApp", "minutes", nullptr));
        timesync_intro->setText(QCoreApplication::translate("FluorescenceApp", "Match the time of Fluorescence\n"
"to the time of your computer", nullptr));
        timesync_button->setText(QCoreApplication::translate("FluorescenceApp", "Sync", nullptr));
        timesync_intro_2->setText(QCoreApplication::translate("FluorescenceApp", "Ready to sync.", nullptr));
        com_intro->setText(QCoreApplication::translate("FluorescenceApp", "Time to connect. Let's talk to Fluorescence...", nullptr));
        com_reload->setText(QString());
        com_connect->setText(QCoreApplication::translate("FluorescenceApp", ">>", nullptr));
        com_label_connect->setText(QCoreApplication::translate("FluorescenceApp", "Ready to connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FluorescenceApp: public Ui_FluorescenceApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLUORESCENCEAPP_H
