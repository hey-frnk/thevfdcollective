#include "fluorescenceapp.h"
#include "ui_fluorescenceapp.h"

#include "fwupdate.h"
#include "infobox.h"

#include "src/fl_app_time.h"
#include "src/fl_app_lights.h"
#include "../FluorescenceV3/Commons/vfdco_clock_routine_defines.h"
#include "../FluorescenceV3/Commons/vfdco_config.h"

#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QDesktopServices>
#include <QMessageBox>
#include <QScreen>
#include <QFileDialog>
#include <QGraphicsOpacityEffect>

// Global dynamic colors
QLabel *preset_dynamic_colors[NUM_PRESET_DYNAMIC_COLORS];
QLabel *preset_dynamic_time[NUM_PRESET_DYNAMIC_TIME];
QLabel *preset_dynamic_lisync[NUM_PRESET_LIGHT_SYNC];

#define GLOBAL_UPDATE_TIMER_INTERVAL 50
#define NUM_AMBIENT_LIGHT_SAMPLES 3

const QString bliss_descriptions[6] = {
    "Nordlicht means northern light in German. Temperament: mysterious, majestic, spirited, impressive\n\
Ink: fluorescent green to teal, touches of purple, Song: Higher Ground by ODESZA",

    "Pastellfrühling means pastel spring in German. Temperament: delightful, lovely, awakening, inspirational\n\
Ink: pastel. cherry, pink, some light green and rarely drip of light blue. Taste: Cherry and honey",

    "Hummelhonig means bumble bee honey in German. Temperament: lively, energetic, active, peaceful\n\
Ink: highly saturated green and blue gradients. Song: Here We Are by BRVTHR",

    "Meeresgeflüster means whispers of the sea in German. Temperament: light-hearted, carefree, liberating, calm\n\
Ink: watercolor. light sky blue to turquoise, with warm white sparks. Song: Island by Unknown Neighbour",

    "Herbstlagerfeuer means fall camp fire in German. Temperament: cozy, sentimental, sincere, warm\n\
Ink: acrylic. lots of orange and strong yellow tones. rarely some green and brick red. Song: Portland\n\
by Andrea von Kampen. Taste: pumpkin, blood orange, maple, cinnamon",

    "Abendhimmel means evening sky in German. Temperament: passionate, untamed, infinite, intense\n\
Ink: strong red. every warm red tone, some orange, some magenta. Song: Lost In The Night - THBD"
};

const QString ambient_light_sample_paths[NUM_AMBIENT_LIGHT_SAMPLES] = {
    ":/Resources/res0.jpg",
    ":/Resources/res1.jpg",
    ":/Resources/res2.jpg"
    // ":/Resources/res3.jpg"
};

const QColor ambient_light_colors[NUM_AMBIENT_LIGHT_SAMPLES][6] = {
    {QColor("#DFA78A"), QColor("#958579"), QColor("#596365"), QColor("#933233"), QColor("#2D444D"), QColor("#1F2A33")},
    {QColor("#A166A3"), QColor("#CE5C70"), QColor("#7B46A3"), QColor("#4E3196"), QColor("#7E395C"), QColor("#301E39")},
    {QColor("#DFE0E3"), QColor("#BEC6CB"), QColor("#A2AFB7"), QColor("#80909B"), QColor("#9F897B"), QColor("#685F5C")}
};

FluorescenceApp::FluorescenceApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FluorescenceApp)
{
    ui->setupUi(this);
    this->setWindowTitle("Fluorescence App by The VFD Collective");
    this->setFixedSize(this->size());

    ui->com_connect->hide(); ui->com_reload->hide(); ui->com_select->hide();

    ui->com_connect->setText(">>");
    // List all available serial ports to com_select + empty
    ui->com_select->addItem("");
    for(QSerialPortInfo available_ports : QSerialPortInfo::availablePorts()) {
        ui->com_select->addItem(available_ports.portName());
    }
    ui->com_select->addItem("Bluetooth");

    hide_all_panels();
    ui->menu_controls_layout->hide();

    ui->panel_welcome->show();

    // Animate fade in
    QGraphicsOpacityEffect *com_connect_eff = new QGraphicsOpacityEffect(this);
    QGraphicsOpacityEffect *com_reload_eff = new QGraphicsOpacityEffect(this);
    QGraphicsOpacityEffect *com_select_eff = new QGraphicsOpacityEffect(this);
    ui->com_connect->setGraphicsEffect(com_connect_eff); ui->com_reload->setGraphicsEffect(com_reload_eff); ui->com_select->setGraphicsEffect(com_select_eff);
    QPropertyAnimation *com_connect_a = new QPropertyAnimation(com_connect_eff,"opacity");
    QPropertyAnimation *com_reload_a = new QPropertyAnimation(com_reload_eff,"opacity");
    QPropertyAnimation *com_select_a = new QPropertyAnimation(com_select_eff,"opacity");
    com_connect_a->setDuration(1000); com_reload_a->setDuration(1000); com_select_a->setDuration(1000);
    com_connect_a->setStartValue(0); com_reload_a->setStartValue(0); com_select_a->setStartValue(0);
    com_connect_a->setEndValue(1); com_reload_a->setEndValue(1); com_select_a->setEndValue(1);
    ui->com_connect->show(); ui->com_reload->show(); ui->com_select->show();
    com_connect_a->start(QPropertyAnimation::DeleteWhenStopped); com_reload_a->start(QPropertyAnimation::DeleteWhenStopped); com_select_a->start(QPropertyAnimation::DeleteWhenStopped);

    // Init timer
    global_timer = new QTimer(this);
    connect(global_timer, &QTimer::timeout, this, QOverload<>::of(&FluorescenceApp::update));
    global_timer->start(GLOBAL_UPDATE_TIMER_INTERVAL);

    // Init custom color wheel & white
    this->custom_global_color.setHsl(0, 255, 127);
    this->custom_global_color_white = 0;
    ui->custom_slider_w->setValue(custom_global_color_white);

    custom_color_update_all_sliders(0);

    // Init dynamic colors & time
    QLabel *tmp_dynamic_colors[NUM_PRESET_DYNAMIC_COLORS] = {ui->dynamic_c1, ui->dynamic_c2, ui->dynamic_c3, ui->dynamic_c4, ui->dynamic_c5, ui->dynamic_c6};
    QLabel *tmp_dynamic_time[NUM_PRESET_DYNAMIC_TIME] = {
        ui->dynamic_t_1, ui->dynamic_t_2, ui->dynamic_t_3, ui->dynamic_t_4, ui->dynamic_t_5, ui->dynamic_t_6, ui->dynamic_t_7,
        ui->dynamic_t_8, ui->dynamic_t_9, ui->dynamic_t_10, ui->dynamic_t_11, ui->dynamic_t_12, ui->dynamic_t_13, ui->dynamic_t_14,
        ui->dynamic_t_15, ui->dynamic_t_16, ui->dynamic_t_17, ui->dynamic_t_18, ui->dynamic_t_19, ui->dynamic_t_20, ui->dynamic_t_21
    };
    QLabel *tmp_dynamic_lisync[NUM_PRESET_LIGHT_SYNC] = {
        ui->lisync_c1, ui->lisync_c2, ui->lisync_c3, ui->lisync_c4, ui->lisync_c5, ui->lisync_c6
    };
    memcpy(preset_dynamic_colors, tmp_dynamic_colors, NUM_PRESET_DYNAMIC_COLORS * sizeof(QLabel *));
    memcpy(preset_dynamic_time, tmp_dynamic_time, NUM_PRESET_DYNAMIC_TIME * sizeof(QLabel *));
    memcpy(preset_dynamic_lisync, tmp_dynamic_lisync, NUM_PRESET_LIGHT_SYNC * sizeof(QLabel *));
    hide_all_dynamic_control_panels();
    clear_lights_instance();
    Light_Pattern_Spectrum_Init((struct Light_Pattern_Spectrum *)&global_lights_instance, NULL);
    ui->panel_dyn_spectrum->show();
    // Set description of Bliss Moment by default
    on_panel_dyn_bliss_param_currentIndexChanged(0);
}

FluorescenceApp::~FluorescenceApp()
{
    delete global_timer;
    delete ui;
}

void FluorescenceApp::preset_ambient_light_update(uint_fast8_t counter) {
    if(preset_dynamic_light_sync_enable) {
        QImage src_q;
        QScreen *screen = QGuiApplication::primaryScreen();
        if (const QWindow *window = windowHandle()) screen = this->screen();
        if (!screen) return;
        src_q = screen->grabWindow(0).toImage().convertToFormat(QImage::Format_RGB888);

        const uint_fast8_t W = 60, H = 10;
        src_q = src_q.scaled(W, H, Qt::IgnoreAspectRatio, Qt::TransformationMode::FastTransformation);
        // ui->lisync_sample->setPixmap(QPixmap::fromImage(src_q));

        uint8_t color_arr[4 * CONFIG_NUM_PIXELS] = {0};
        for(uint_fast8_t p = 0; p < CONFIG_NUM_PIXELS; ++p) {
            uint32_t r_mean = 0, g_mean = 0, b_mean = 0;

            for(uint_fast8_t i = 0; i < H; ++i) {
                for(uint_fast8_t j = 1; j < 7; ++j) {
                    uint32_t k = i + (H * p);
                    r_mean += src_q.pixelColor(k, j).red();
                    g_mean += src_q.pixelColor(k, j).green();
                    b_mean += src_q.pixelColor(k, j).blue();
                }
            }
            r_mean /= 70;
            g_mean /= 70;
            b_mean /= 70;
            qDebug() << r_mean << " " << g_mean << " " << b_mean;
            QColor rgbval = QColor(r_mean, g_mean, b_mean);

            int16_t r_s = rgbval.red() < 128 ? rgbval.red() : rgbval.red() - 256;
            int16_t g_s = rgbval.green() < 128 ? rgbval.green() : rgbval.green() - 256;
            int16_t b_s = rgbval.blue() < 128 ? rgbval.blue() : rgbval.blue() - 256;

            uint8_t threshold = 0, right_shift_factor = 1;
            // Weighting
            if(rgbval.red() > rgbval.blue() && rgbval.red() > rgbval.green()) {
                threshold = rgbval.red() >> 2;
                int16_t b_g_diff = b_s - g_s;
                if(b_g_diff > threshold) {
                    rgbval.setGreen(rgbval.green() >> right_shift_factor);
                } else if(b_g_diff < threshold) {
                    rgbval.setBlue(rgbval.blue() >> (right_shift_factor + 1));
                } else {
                    rgbval.setGreen(rgbval.green() >> right_shift_factor);
                    rgbval.setBlue(rgbval.blue() >> (right_shift_factor + 1));
                }
            } else if(rgbval.green() > rgbval.red() && rgbval.green() > rgbval.blue()) {
                if((uint16_t)rgbval.green() + (rgbval.green() / 2) < 255) {
                    rgbval.setGreen(rgbval.green() + rgbval.green() / 2);
                } else {
                    rgbval.setGreen(255);
                }
                threshold = rgbval.green() >> 2;
                int16_t r_b_diff = r_s - b_s;
                if(r_b_diff > threshold) {
                    rgbval.setBlue(rgbval.blue() >> (right_shift_factor + 3));
                } else if(r_b_diff < threshold) {
                    rgbval.setRed(rgbval.red() >> (right_shift_factor + 1));
                } else {
                    rgbval.setRed(rgbval.red() >> (right_shift_factor + 1));
                    rgbval.setBlue(rgbval.blue() >> (right_shift_factor + 3));
                }
            } else if(rgbval.blue() > rgbval.red() && rgbval.blue() > rgbval.green()) {
                threshold = rgbval.blue() >> 2;
                int16_t r_g_diff = r_s - g_s;
                if(r_g_diff > threshold) {
                    rgbval.setGreen(rgbval.green() >> right_shift_factor);
                } else if(r_g_diff < threshold) {
                    rgbval.setRed(rgbval.red() >> right_shift_factor);
                } else {
                    rgbval.setRed(rgbval.red() >> right_shift_factor);
                    rgbval.setGreen(rgbval.green() >> right_shift_factor);
                }
            }

            color_arr[4 * (CONFIG_NUM_PIXELS - 1) - 4 * p] = (uint8_t)rgbval.red();
            color_arr[4 * (CONFIG_NUM_PIXELS - 1) - 4 * p + 1] = (uint8_t)rgbval.green();
            color_arr[4 * (CONFIG_NUM_PIXELS - 1) - 4 * p + 2] = (uint8_t)rgbval.blue();
            color_arr[4 * (CONFIG_NUM_PIXELS - 1) - 4 * p + 3] = 0;
        }
        if(global_com_instance) global_com_instance->transfer_serial1(color_arr);
    } else {
        ui->lisync_sample->setPixmap(QPixmap::fromImage(QImage(ambient_light_sample_paths[counter])));
        for(int i = 0; i < 6; ++i) preset_dynamic_lisync[i]->setStyleSheet("background-color:" + ambient_light_colors[counter][i].name());
    }
}

void FluorescenceApp::update(){
    // Display current time!
    QTime ct = QTime::currentTime().addSecs(60 * ui->timesync_p_m->value() + 60 * 60 * ui->timesync_p_h->value());
    QDate cd = QDate::currentDate().addDays(ui->timesync_p_d->value()).addMonths(ui->timesync_p_mo->value()).addYears(ui->timesync_p_y->value());
    ui->timesync_time_label->setText(ct.toString("hh:mm:ss"));
    ui->timesync_date_label->setText(cd.toString(Qt::DefaultLocaleLongDate));

    // Update color wheel
    vfdco_date_t d;
    vfdco_get_date_time(&d, &global_time);
    Light_Pattern_Update(&global_lights_instance);
    ++preset_dynamic_timer;
    for(uint_fast8_t i = 0; i < NUM_PRESET_DYNAMIC_TIME; ++i) {
        uint_fast8_t hpdt = preset_dynamic_timer / 8;
        if(i < hpdt) preset_dynamic_time[i]->setStyleSheet("background-color:" + QColor::fromRgb(16, 128, 128).name());
        else preset_dynamic_time[i]->setStyleSheet("background-color:" + QColor::fromRgb(196, 196, 196).name());
    }
    if(preset_dynamic_timer == (NUM_PRESET_DYNAMIC_TIME * 8)) preset_dynamic_timer = 0;

    // Update ambient light every 2 seconds
    ++preset_dynamic_light_sync_timer;
    if(preset_dynamic_light_sync_timer == 40) {
        preset_dynamic_light_sync_timer = 0;
        preset_ambient_light_update(preset_dynamic_light_sync_counter++);
        if(preset_dynamic_light_sync_counter == NUM_AMBIENT_LIGHT_SAMPLES) preset_dynamic_light_sync_counter = 0;
    }
}

void FluorescenceApp::hide_all_panels() {
    ui->panel_welcome->hide();
    ui->panel_custom_colors->hide();
    ui->panel_presets->hide();
    ui->panel_message->hide();
    ui->panel_timesync->hide();
    ui->panel_settings->hide();
    ui->panel_lsettings->hide();

    ui->menu_button->setPixmap(QPixmap::fromImage(QImage(":/Resources/menu.png")));
}

void FluorescenceApp::on_com_connect_clicked()
{
    // Not connected
    if(ui->com_label_connect->text() == "Ready to connect" || ui->com_label_connect->text() == "Click to find Fluorescence and pair") {
        // Check for no selection
        if(!QString::compare(ui->com_select->currentText(), "")) {
            error_message("Please select a device from the list", QMessageBox::Information);
            return;
        }

        // Initialize COM instance & try to connect
        if(ui->com_select->currentText() == "Bluetooth") {
            global_com_instance = new fl_app_com();
            QObject::connect(global_com_instance, &fl_app_com::app_com_connected, this, &FluorescenceApp::app_com_connected_callback);        
            QObject::connect(global_com_instance, &fl_app_com::bt_status_changed, this, &FluorescenceApp::app_com_status_callback);
        }
        else {
            global_com_instance = new fl_app_com(ui->com_select->currentText());
            app_com_connected_callback();
        }
    }

    // Connected
    else if(ui->com_label_connect->text() == "Fluorescence is connected") {
        delete global_com_instance;
        global_com_instance = nullptr;

        ui->com_select->setEnabled(true);
        ui->com_select->setCurrentIndex(0); // Empty
        ui->com_label_connect->setText("Ready to connect");
        ui->com_intro->setText("Time to connect. Let's talk to Fluorescence...");
        ui->com_connect->clear();
        ui->com_connect->setText(">>");
        ui->menu_button->setEnabled(false);

        hide_all_panels();
        ui->panel_welcome->show();
    }
}

void FluorescenceApp::on_com_reload_clicked()
{
    // if in unconnected state
    if(ui->com_label_connect->text() == "Ready to connect" || ui->com_label_connect->text() == "Click to find Fluorescence and pair") {
        ui->com_select->clear();
        ui->com_select->addItem("");
        for(QSerialPortInfo available_ports : QSerialPortInfo::availablePorts()) {
            ui->com_select->addItem(available_ports.portName());
        }
        ui->com_select->addItem("Bluetooth");
    }
}

void FluorescenceApp::app_com_connected_callback()
{
    // Request FW version
    QString fw_version = global_com_instance->transfer_clock_control(COM_PROTOCOL_FW_VERSION_REQ);
    if(global_com_instance->legacy_protocol()) {
        // Set legacy
        global_is_fw2 = true;
        ui->lsettings_info_fw->setText((fw_version).mid(0, 6));
    } else {
        // Set regular
        global_is_fw2 = false;
        ui->settings_info_fw->setText(fw_version.mid(0, 6));
        // Additional hardware version
        QString hw_version = global_com_instance->transfer_clock_control(COM_PROTOCOL_HW_VERSION_REQ);
        ui->settings_info_hw->setText(hw_version.mid(0, 4));
    }

    if(global_com_instance->getStatus() == FL_APP_COM_STATUS_CONNECTION_FAILED) {
        error_message("Oh deer 🦌, seems like we couldn't connect to Fluorescence. Please try again and make sure the right device is selected!", QMessageBox::Critical);

        delete global_com_instance;
        global_com_instance = nullptr;

        // Enable reconnecting
        ui->com_select->setEnabled(true);
        return;
    }

    ui->com_select->setEnabled(false);
    ui->menu_button->setEnabled(true);
    ui->com_label_connect->setText("Fluorescence is connected");
    ui->com_connect->setText("");
    ui->com_connect->setPixmap(QPixmap::fromImage(QImage(":/Resources/stop.png")));
    ui->com_intro->setText("Fluorescence is connected. Have a colorful time.");
}

void FluorescenceApp::app_com_status_callback(QString status) {
    if(status.contains(QStringLiteral("Error connecting to Fluorescence"))) {
        error_message(status, QMessageBox::Icon::Critical);
    }
    ui->com_label_connect->setText(status);
}

void FluorescenceApp::on_com_select_currentTextChanged(const QString &arg1)
{
    if(arg1.compare("Bluetooth") == 0) {
        ui->com_label_connect->setText("Click to find Fluorescence and pair");
    } else {
        ui->com_label_connect->setText("Ready to connect");
    }
}

void FluorescenceApp::on_main_welcome_clicked()
{
    hide_all_panels();
    ui->menu_controls_layout->hide();
    ui->panel_welcome->show();
}

void FluorescenceApp::on_main_preset_clicked()
{
    hide_all_panels();
    ui->menu_controls_layout->hide();
    ui->panel_presets->show();

    if(!global_is_fw2) { // Legacy has some controls disabled
        ui->dynamic_bliss_dnc->setEnabled(true);
        ui->tab_shuffle->setEnabled(true);
    } else {
        ui->dynamic_bliss_dnc->setEnabled(false);
        ui->tab_shuffle->setEnabled(false);
    }
}

void FluorescenceApp::on_main_custom_clicked()
{
    hide_all_panels();
    ui->menu_controls_layout->hide();
    ui->panel_custom_colors->show();

    if(!global_is_fw2) { // Legacy has some controls disabled
        ui->custom_label_w->setEnabled(true);
        ui->custom_slider_w->setEnabled(true);
    } else {
        ui->custom_label_w->setEnabled(false);
        ui->custom_slider_w->setEnabled(false);
    }
}

void FluorescenceApp::on_main_timesync_clicked()
{
    hide_all_panels();
    ui->menu_controls_layout->hide();
    ui->panel_timesync->show();
}

void FluorescenceApp::on_main_message_clicked()
{
    hide_all_panels();
    ui->menu_controls_layout->hide();
    ui->panel_message->show();
}

void FluorescenceApp::on_main_settings_clicked()
{
    hide_all_panels();
    ui->menu_controls_layout->hide();
    if(!global_is_fw2) {
        ui->panel_settings->show();
    } else { // Legacy (fw2) Settings
        ui->panel_lsettings->show();
    }

}


void FluorescenceApp::custom_color_update_all_sliders(bool block_color_wheel) {
    // Mutual exclusion
    bool blocked = signalsBlocked();
    blockSignals(true);
    Q_FOREACH(QWidget* w, findChildren<QWidget*>())
        w->blockSignals(true);

    // Sync elements
    if(!block_color_wheel) {
        QPoint move_point = map_hue(custom_global_color.hueF() * 359.0f);
        ui->custom_color_wheel_cursor->move(move_point);
    }

    ui->custom_value_h->setValue((double)custom_global_color.hueF() * 359.0f);
    ui->custom_value_s->setValue(custom_global_color.hslSaturationF() * 255.0f);
    ui->custom_value_l->setValue(custom_global_color.lightnessF() * 255.0f);
    ui->custom_value_r->setValue(custom_global_color.red());
    ui->custom_value_g->setValue(custom_global_color.green());
    ui->custom_value_b->setValue(custom_global_color.blue());

    ui->custom_slider_h->setValue(custom_global_color.hslHueF() * 359.0f);
    ui->custom_slider_s->setValue(custom_global_color.hslSaturationF() * 255.0f);
    ui->custom_slider_l->setValue(custom_global_color.lightnessF() * 255.0f);

    ui->custom_slider_r->setValue(custom_global_color.red());
    ui->custom_slider_g->setValue(custom_global_color.green());
    ui->custom_slider_b->setValue(custom_global_color.blue());

    ui->custom_value_hex->setPlainText(custom_global_color.name().toUpper());

    ui->custom_value_w->setValue(custom_global_color_white);
    ui->custom_slider_w->setValue(custom_global_color_white);

    // Begin Transfer
    if(global_com_instance) {
        QCheckBox *ch[6] = {
            ui->custom_select_led_1, ui->custom_select_led_2, ui->custom_select_led_3,
            ui->custom_select_led_4, ui->custom_select_led_5, ui->custom_select_led_6
        };
        uint8_t color_arr[4 * CONFIG_NUM_PIXELS] = {0};
        for(uint_fast8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) {
            if(ch[i]->checkState() == Qt::Checked) {
                custom_led_colors[i] = custom_global_color;
                custom_led_white[i] = custom_global_color_white;
            }
            color_arr[4 * i] = custom_led_colors[i].red();
            color_arr[4 * i + 1] = custom_led_colors[i].green();
            color_arr[4 * i + 2] = custom_led_colors[i].blue();
            color_arr[4 * i + 3] = custom_led_white[i];
        }
        global_com_instance->transfer_serial0(color_arr);
    }

    blockSignals(blocked);
    Q_FOREACH(QWidget* w, findChildren<QWidget*>())
        w->blockSignals(false);
}

void FluorescenceApp::error_message(QString message, QMessageBox::Icon i)
{
    QMessageBox err;
    err.setText(message);
    err.setIcon(i);
    err.exec();
}

void FluorescenceApp::on_custom_slider_r_valueChanged(int position)
{
    if (!signalsBlocked()) {
        custom_global_color.setRed(position);
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_slider_g_valueChanged(int position)
{
    if (!signalsBlocked()) {
        custom_global_color.setGreen(position);
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_slider_b_valueChanged(int position)
{
    if (!signalsBlocked()) {
        custom_global_color.setBlue(position);
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_slider_w_valueChanged(int position)
{
    if (!signalsBlocked()) {
        custom_global_color_white = position;
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_slider_h_valueChanged(int position)
{
    if (!signalsBlocked()) {
        QColor new_color;
        new_color.setHslF((double)position / 359.0f, custom_global_color.hslSaturationF(), custom_global_color.lightnessF());
        custom_global_color = new_color;
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_slider_s_valueChanged(int position)
{
    if (!signalsBlocked()) {
        QColor new_color;
        new_color.setHslF(custom_global_color.hueF(), position / 255.0f, custom_global_color.lightnessF());
        custom_global_color = new_color;
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_slider_l_valueChanged(int position)
{
    if (!signalsBlocked()) {
        QColor new_color;
        new_color.setHslF(custom_global_color.hueF(), custom_global_color.hslSaturationF(), position / 255.0f);
        custom_global_color = new_color;
        custom_color_update_all_sliders(0);
    }
}

void FluorescenceApp::on_custom_value_r_valueChanged(int arg1) { on_custom_slider_r_valueChanged(arg1); }
void FluorescenceApp::on_custom_value_g_valueChanged(int arg1) { on_custom_slider_g_valueChanged(arg1); }
void FluorescenceApp::on_custom_value_b_valueChanged(int arg1) { on_custom_slider_b_valueChanged(arg1); }
void FluorescenceApp::on_custom_value_w_valueChanged(int arg1) { on_custom_slider_w_valueChanged(arg1); }
void FluorescenceApp::on_custom_value_h_valueChanged(int arg1) { on_custom_slider_h_valueChanged(arg1); }
void FluorescenceApp::on_custom_value_s_valueChanged(int arg1) { on_custom_slider_s_valueChanged(arg1); }
void FluorescenceApp::on_custom_value_l_valueChanged(int arg1) { on_custom_slider_l_valueChanged(arg1); }

void FluorescenceApp::on_custom_value_save_clicked()
{
    QString file_path = QFileDialog::getSaveFileName(this, "Save custom color", QDir::homePath(), "Fluorescence Custom Color File (*.flclr)");
    if(file_path.isEmpty()) return;

    QFile save_file(file_path);
    save_file.open(QIODevice::WriteOnly);
    QDataStream output_stream(&save_file);
    for(QColor c : custom_led_colors) output_stream << c;
    for(uint8_t c : custom_led_white) output_stream << (quint8)c; // QString("x%1").arg((uint8_t)c, 2, 16, QChar('0'));

    save_file.close();

    qDebug() << "Serialized: ";
    for(QColor c : custom_led_colors) qDebug() << c.name();
    for(uint8_t c : custom_led_white) qDebug() << QString("x%1").arg((uint8_t)c, 2, 16, QChar('0'));
}

void FluorescenceApp::on_custom_value_load_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "Load custom color", QDir::homePath(), "Fluorescence Custom Color File (*.flclr)");
    if(file_path.isEmpty()) return;

    QFile load_file(file_path);
    load_file.open(QIODevice::ReadOnly);
    QDataStream input_stream(&load_file);
    QColor color_read_in[6];
    quint8 color_read_in_w[6] = {0};
    for(int i = 0; i < 6; ++i) input_stream >> color_read_in[i];
    for(int i = 0; i < 6; ++i) input_stream >> color_read_in_w[i];

    load_file.close();

    qDebug() << "De-serialized: ";
    for(QColor c : color_read_in) qDebug() << c.name();
    for(quint8 c : color_read_in_w) qDebug() << QString("x%1").arg((uint8_t)c, 2, 16, QChar('0'));

    if(global_com_instance) {
        uint8_t color_arr[4 * CONFIG_NUM_PIXELS] = {0};
        for(uint_fast8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) {
            custom_led_colors[i] = color_read_in[i];
            custom_led_white[i] = color_read_in_w[i];
            color_arr[4 * i] = custom_led_colors[i].red();
            color_arr[4 * i + 1] = custom_led_colors[i].green();
            color_arr[4 * i + 2] = custom_led_colors[i].blue();
            color_arr[4 * i + 3] = custom_led_white[i];
        }
        global_com_instance->transfer_serial1(color_arr);
    }
}

float FluorescenceApp::capture_hue(const QPoint &pt) {
    // Check if in wheel
    auto posx = pt.x() - ui->custom_color_wheel->height() / 2,
         posy = pt.y() - ui->custom_color_wheel->width() / 2;

    // Into angle and magnitude
    float pos_angle = std::atan2((float)posx, (float)-posy) / M_PI * 180.0;
    if(pos_angle < 0.0f) pos_angle = 360.0 + pos_angle;
    return pos_angle;
}

QPoint FluorescenceApp::map_hue(float angle) {
    constexpr float radius = 127.0f;
    float angle_rad = angle / 180.0f * M_PI;
    return QPoint(radius + radius * std::cos(angle_rad), radius + radius * std::sin(angle_rad)) - QPoint(ui->custom_color_wheel_cursor->width() / 2, ui->custom_color_wheel_cursor->height() / 2);
}

void FluorescenceApp::on_custom_color_wheel_mousePressed(const QPoint &pt)
{
    if (!signalsBlocked()) {
        on_custom_color_wheel_cursor_mouseMoved(pt);
        ui->custom_color_wheel_cursor->move(pt - QPoint(ui->custom_color_wheel_cursor->width() / 2, ui->custom_color_wheel_cursor->height() / 2));
    }
}

void FluorescenceApp::on_custom_color_wheel_cursor_mouseMoved(const QPoint &pt)
{
    if (!signalsBlocked()) {
        float position = capture_hue(pt);
        QColor new_color;
        new_color.setHslF((double)position / 360.0f, custom_global_color.hslSaturationF(), custom_global_color.lightnessF());
        custom_global_color = new_color;
        custom_color_update_all_sliders(1);
    }
}

void FluorescenceApp::clear_lights_instance() {
    Container_Light_Pattern_Clear(&global_lights_instance);
}

void FluorescenceApp::on_dynamic_spectrum_clicked() {
    clear_lights_instance();
    hide_all_dynamic_control_panels();
    Light_Pattern_Spectrum_Init((struct Light_Pattern_Spectrum *)&global_lights_instance, NULL);
    ui->panel_dyn_spectrum->show();
}

void FluorescenceApp::on_dynamic_bliss_dnc_clicked() {
    clear_lights_instance();
    hide_all_dynamic_control_panels();
    Light_Pattern_MomentsOfBliss_Init((struct Light_Pattern_MomentsOfBliss *)&global_lights_instance, NULL);
    ui->panel_dyn_bliss->show();
}

void FluorescenceApp::on_dynamic_rainbow_clicked() {
    clear_lights_instance();
    hide_all_dynamic_control_panels();
    Light_Pattern_Rainbow_Init((struct Light_Pattern_Rainbow *)&global_lights_instance, NULL);
    ui->panel_dyn_rnb->show();
}

void FluorescenceApp::on_dynamic_chase_clicked() {
    clear_lights_instance();
    hide_all_dynamic_control_panels();
    Light_Pattern_Chase_Init((struct Light_Pattern_Chase *)&global_lights_instance, &global_time, NULL);
    ui->panel_dyn_chase->show();
}


void FluorescenceApp::on_dynamic_music_clicked()
{
    clear_lights_instance();
    hide_all_dynamic_control_panels();
    Light_Pattern_Music_Init((struct Light_Pattern_Music *)&global_lights_instance, NULL);
    ui->panel_dyn_music->show();
}

void FluorescenceApp::on_dynamic_timecode_clicked() {
    clear_lights_instance();
    hide_all_dynamic_control_panels();
    Light_Pattern_Time_Code_Init((struct Light_Pattern_Time_Code *)&global_lights_instance, &global_time);
    ui->panel_dyn_tcode->show();
}

void FluorescenceApp::on_dynamic_police_clicked() {
    clear_lights_instance();
    hide_all_dynamic_control_panels();
    Light_Pattern_Cop_Init((struct Light_Pattern_Cop *)&global_lights_instance);
    ui->panel_dyn_cop->show();
}

void FluorescenceApp::hide_all_dynamic_control_panels() {
    ui->panel_dyn_spectrum->hide();
    ui->panel_dyn_bliss->hide();
    ui->panel_dyn_music->hide();
    ui->panel_dyn_rnb->hide();
    ui->panel_dyn_chase->hide();
    ui->panel_dyn_tcode->hide();
    ui->panel_dyn_cop->hide();
}

void FluorescenceApp::on_panel_dyn_bliss_param_currentIndexChanged(int index)
{
    // Set moment description
    ui->panel_dyn_bliss_description->setText(bliss_descriptions[index]);
}

void FluorescenceApp::on_settings_td_set_clicked()
{
    // Direct parsing since indices correspond to mode enums. Two subsequent transfers
    uint8_t time_format = ui->settings_td_timemode->currentIndex();
    uint8_t date_format = ui->settings_td_datemode->currentIndex();
    global_com_instance->transfer_gui_set(0, time_format);
    QThread::msleep(25);
    global_com_instance->transfer_gui_set(1, date_format);
}

void FluorescenceApp::on_settings_bri_set_clicked()
{
    // Direct parsing, since indices correspond to brightness enums >> 1
    uint8_t brightness_display = ui->settings_bri_disp->currentIndex() << 1;
    uint8_t brightness_led = ui->settings_bri_led->currentIndex() << 1;
    global_com_instance->transfer_brightness(0, brightness_display);
    QThread::msleep(25);
    global_com_instance->transfer_brightness(1, brightness_led);
}

void FluorescenceApp::on_settings_presets_en_set_clicked()
{
    // Parse checks
    uint8_t enabled_instances = 0;
    enabled_instances |=  1 << (ui->settings_presets_en_cop->isChecked() * LIGHT_PATTERN_COP);
    enabled_instances |=  1 << (ui->settings_presets_en_tcode->isChecked() * LIGHT_PATTERN_TIME_CODE);
    enabled_instances |=  1 << (ui->settings_presets_en_music->isChecked() * LIGHT_PATTERN_MUSIC);
    enabled_instances |=  1 << (ui->settings_presets_en_chase->isChecked() * LIGHT_PATTERN_CHASE);
    enabled_instances |=  1 << (ui->settings_presets_en_rnb->isChecked() * LIGHT_PATTERN_RAINBOW);
    enabled_instances |=  1 << (ui->settings_presets_en_spectrum->isChecked() * LIGHT_PATTERN_SPECTRUM);
    enabled_instances |=  1 << (ui->settings_presets_en_bliss->isChecked() * LIGHT_PATTERN_MOMENTSOFBLISS);
    enabled_instances |=  1 << (ui->settings_presets_en_static->isChecked() * LIGHT_PATTERN_STATIC);
    global_com_instance->transfer_enable_presets(enabled_instances);
}


void FluorescenceApp::on_shuffle_set_clicked()
{
    // Parse checks
    uint8_t enabled_instances = 0;
    enabled_instances |=  1 << (ui->shuffle_en_cop->isChecked() * LIGHT_PATTERN_COP);
    enabled_instances |=  1 << (ui->shuffle_en_tcode->isChecked() * LIGHT_PATTERN_TIME_CODE);
    enabled_instances |=  1 << (ui->shuffle_en_music->isChecked() * LIGHT_PATTERN_MUSIC);
    enabled_instances |=  1 << (ui->shuffle_en_chase->isChecked() * LIGHT_PATTERN_CHASE);
    enabled_instances |=  1 << (ui->shuffle_en_rnb->isChecked() * LIGHT_PATTERN_RAINBOW);
    enabled_instances |=  1 << (ui->shuffle_en_spectrum->isChecked() * LIGHT_PATTERN_SPECTRUM);
    enabled_instances |=  1 << (ui->shuffle_en_bliss->isChecked() * LIGHT_PATTERN_MOMENTSOFBLISS);
    enabled_instances |=  1 << (ui->shuffle_en_static->isChecked() * LIGHT_PATTERN_STATIC);
    global_com_instance->transfer_random_set(enabled_instances, (random_speed_t)ui->shuffle_speed->currentIndex());
}


void FluorescenceApp::on_settings_nsh_enable_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked) ui->settings_nsh_panel->setEnabled(true);
    else ui->settings_nsh_panel->setEnabled(false);
}

void FluorescenceApp::on_settings_nsh_set_clicked()
{
    if(ui->settings_nsh_enable->isChecked()) {
        QDateTime current_datetime = QDateTime::currentDateTime();
        QDateTime t_in_start = ui->settings_nsh_start->dateTime();
        QDateTime t_in_stop = ui->settings_nsh_stop->dateTime();
        // Take time from input, but reference date to current date
        t_in_start.setDate(current_datetime.date()); t_in_stop.setDate(current_datetime.date());
        t_in_stop = t_in_stop.addDays(1); // The next day
        auto span_to_start = t_in_start.msecsTo(current_datetime);
        auto span_to_stop = t_in_stop.msecsTo(current_datetime);

        vfdco_time_t t_start;
        t_start.h = (uint8_t)t_in_start.time().hour();
        t_start.m = (uint8_t)t_in_start.time().minute();
        t_start.s = 0;
        vfdco_time_t t_stop;
        t_stop.h = (uint8_t)t_in_stop.time().hour();
        t_stop.m = (uint8_t)t_in_stop.time().minute();
        t_stop.s = 0;

        // Check if NSH has to be set explicitly
        if(span_to_start >= 0 && span_to_stop <= 0) {
            // If we are supposed to be in NSH already, do it
            global_com_instance->transfer_night_shift(t_start, t_stop, 1);
        } else {
            // Otherwise just set
            global_com_instance->transfer_night_shift(t_start, t_stop, 0);
        }
    } else {
        vfdco_time_t t_start;
        t_start.h = 0; t_start.m = 0; t_start.s = 0;
        vfdco_time_t t_stop;
        t_stop.h = 0; t_stop.m = 0; t_stop.s = 0;
        global_com_instance->transfer_night_shift(t_start, t_stop, 0);
    }
}

void FluorescenceApp::on_settings_settings_default_clicked() {
    global_com_instance->transfer_clock_control(COM_PROTOCOL_DEFAULT_REQ);
    // Overwrite brightness to maximum
}

void FluorescenceApp::on_settings_settings_save_clicked()
{
    global_com_instance->transfer_clock_control(COM_PROTOCOL_SETTINGS_SAVE_REQ);
}

void FluorescenceApp::on_settings_info_download_clicked()
{
    // Just take you to the support
    QDesktopServices::openUrl(QUrl("https://www.thevfdcollective.com/support"));
}

void FluorescenceApp::on_lsettings_td_12hr_enable_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked) global_com_instance->legacy_transfer_clock_flags(0, 1);
    else global_com_instance->legacy_transfer_clock_flags(0, 0);
}

void FluorescenceApp::on_lsettings_td_l0_enable_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked) global_com_instance->legacy_transfer_clock_flags(3, 1);
    else global_com_instance->legacy_transfer_clock_flags(3, 0);
}

void FluorescenceApp::on_lsettings_td_intldate_enable_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked) global_com_instance->legacy_transfer_clock_flags(1, 1);
    else global_com_instance->legacy_transfer_clock_flags(1, 0);
}

void FluorescenceApp::on_lsettings_nsh_enable_stateChanged(int arg1)
{
    vfdco_time_t dummy_time = {0, 0, 0};
    if(arg1 == Qt::Checked) global_com_instance->legacy_transfer_night_shift(dummy_time, dummy_time, 42, 1);
    else global_com_instance->legacy_transfer_night_shift(dummy_time, dummy_time, 42, 0);
}


void FluorescenceApp::on_lsettings_nsh_scheduled_enable_stateChanged(int arg1)
{
    vfdco_time_t t_start;
    t_start.h = (uint8_t)ui->settings_nsh_start->time().hour();
    t_start.m = (uint8_t)ui->settings_nsh_start->time().minute();
    t_start.s = 0;
    vfdco_time_t t_stop;
    t_stop.h = (uint8_t)ui->settings_nsh_stop->time().hour();
    t_stop.m = (uint8_t)ui->settings_nsh_stop->time().minute();
    t_stop.s = 0;
    if(arg1 == Qt::Checked) global_com_instance->legacy_transfer_night_shift(t_start, t_stop, 1, 42);
    else global_com_instance->legacy_transfer_night_shift(t_start, t_stop, 0, 42);
}

void FluorescenceApp::on_lsettings_settings_default_clicked() { on_settings_settings_default_clicked(); }
void FluorescenceApp::on_lsettings_settings_save_clicked() { on_settings_settings_save_clicked(); }
void FluorescenceApp::on_lsettings_info_download_clicked() { on_settings_info_download_clicked(); }

void FluorescenceApp::on_timesync_button_clicked()
{
    QTime ct = QTime::currentTime().addSecs(60 * ui->timesync_p_m->value() + 60 * 60 * ui->timesync_p_h->value());
    QDate cd = QDate::currentDate().addDays(ui->timesync_p_d->value()).addMonths(ui->timesync_p_mo->value()).addYears(ui->timesync_p_y->value());
    vfdco_time_t v_time;
    v_time.h = (uint8_t)ct.hour();
    v_time.m = (uint8_t)ct.minute();
    v_time.s = (uint8_t)ct.second();

    vfdco_date_t v_date;
    v_date.y = (uint8_t)(cd.year() % 100);
    v_date.m = (uint8_t)cd.month();
    v_date.d = (uint8_t)cd.day();

    QString resp = global_com_instance->transfer_time_date(v_time, v_date);
    if(resp.contains(QStringLiteral("Time Sy"))) ui->timesync_intro_2->setText("Sync successful.");
}


void FluorescenceApp::on_timesync_time_label_clicked()
{
    ui->timesync_p_y->setValue(0);
    ui->timesync_p_mo->setValue(0);
    ui->timesync_p_d->setValue(0);
    ui->timesync_p_h->setValue(0);
    ui->timesync_p_m->setValue(0);
}

void FluorescenceApp::on_static_off_clicked() {         global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 0, 0); }
void FluorescenceApp::on_static_white_clicked() {       global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 1, 0); }
void FluorescenceApp::on_static_warmwhite_clicked() {   global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 2, 0); }
void FluorescenceApp::on_static_red_clicked() {         global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 3, 0); }
void FluorescenceApp::on_static_green_clicked() {       global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 4, 0); }
void FluorescenceApp::on_static_blue_clicked() {        global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 5, 0); }
void FluorescenceApp::on_static_yellow_clicked() {      global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 6, 0); }
void FluorescenceApp::on_static_orange_clicked() {      global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 7, 0); }
void FluorescenceApp::on_static_cyan_clicked() {        global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 8, 0); }
void FluorescenceApp::on_static_magenta_clicked() {     global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 9, 0); }
void FluorescenceApp::on_static_violet_clicked() {      global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 10, 0); }
void FluorescenceApp::on_static_rainbow_clicked() {     global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 11, 0); }
void FluorescenceApp::on_static_pastelrainbow_clicked(){global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 12, 0); }
void FluorescenceApp::on_static_green2blue_clicked() {  global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 13, 0); }
void FluorescenceApp::on_static_red2blue_clicked() {    global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 14, 0); }
void FluorescenceApp::on_static_red2green_clicked() {   global_com_instance->transfer_light_pattern(LIGHT_PATTERN_STATIC, 15, 0); }

void FluorescenceApp::on_panel_dyn_spectrum_set_clicked()
{
    QString param1 = ui->panel_dyn_spectrum_param_1->currentText();
    QString param2 = ui->panel_dyn_spectrum_param_2->currentText();
    uint8_t param1_value, param2_value;
    // Sat
    if(param1 == "low") param1_value = 127;
    else if(param1 == "medium") param1_value = 196;
    else param1_value = 255;
    // Li
    if(param1 == "low") param2_value = 50;
    else if(param1 == "medium") param2_value = 90;
    else param2_value = 127;

    global_com_instance->transfer_light_pattern(LIGHT_PATTERN_SPECTRUM, param1_value, param2_value);
}

void FluorescenceApp::on_panel_dyn_bliss_set_clicked()
{
    // Direct index parsing
    uint8_t param1 = ui->panel_dyn_bliss_param->currentIndex();
    global_com_instance->transfer_light_pattern(LIGHT_PATTERN_MOMENTSOFBLISS, param1, 0);
}

void FluorescenceApp::on_panel_dyn_rnb_set_clicked()
{
    uint8_t param1_value = (uint8_t)ui->panel_dyn_rnb_param1->value();
    QString param2 = ui->panel_dyn_rnb_param2->currentText();
    uint8_t param2_value;
    // Sat
    if(param2 == "low") param2_value = 127;
    else if(param2 == "medium") param2_value = 196;
    else param2_value = 255;
    global_com_instance->transfer_light_pattern(LIGHT_PATTERN_RAINBOW, param1_value, param2_value);
}

void FluorescenceApp::on_panel_dyn_chase_set_clicked()
{
    uint8_t param1_value = (uint8_t)ui->panel_dyn_chase_param1->value();
    uint8_t param2_value = (uint8_t)ui->panel_dyn_chase_param2->currentIndex();
    global_com_instance->transfer_light_pattern(LIGHT_PATTERN_CHASE, param2_value, param1_value);
}

void FluorescenceApp::on_panel_dyn_tcode_set_clicked()
{
    // Direct index parsing
    uint8_t param1 = ui->panel_dyn_tcode_param1->currentIndex();
    global_com_instance->transfer_light_pattern(LIGHT_PATTERN_TIME_CODE, param1, 0);
}

void FluorescenceApp::on_panel_dyn_cop_set_clicked()
{
    // Direct index parsing
    uint8_t param1 = ui->panel_dyn_cop_param1->currentIndex();
    global_com_instance->transfer_light_pattern(LIGHT_PATTERN_COP, param1, 0);
}

void FluorescenceApp::on_panel_dyn_music_set_clicked()
{
    uint8_t param1_value = (uint8_t)ui->panel_dyn_music_param1->value();
    QString param2 = ui->panel_dyn_music_param2->currentText();
    uint8_t param2_value;
    // Sat
    if(param2 == "low") param2_value = 127;
    else if(param2 == "medium") param2_value = 196;
    else param2_value = 255;
    global_com_instance->transfer_light_pattern(LIGHT_PATTERN_MUSIC, param1_value, param2_value);
}


void FluorescenceApp::on_message_send_clicked()
{
    QString message = ui->message_text->toPlainText();
    uint8_t *m[4] = {NULL, NULL, NULL, NULL};

    if(message.length() == 0) {
        error_message("Oops, u forgot the message, did ya?", QMessageBox::Information);
        return;
    }
    if(ui->message_set_custom_welcome->checkState() == Qt::Checked) {
        if(message.length() == 6) {
            uint8_t welcome_msg[6];
            for(uint_fast8_t i = 0; i < 6; ++i) welcome_msg[i] = message.toLatin1().toUpper().data()[i];
            global_com_instance->transfer_welcome_set(welcome_msg);
        } else {
            error_message("Oops, the welcome message needs exactly 6 characters!", QMessageBox::Information);
            return;
        }
    } else {
        if(message.length() > 4 * 6) {
            error_message("Oops, the message is longer than 4 * 6 = 24 characters.\n Please try again with less.", QMessageBox::Information);
            return;
        }
        uint_fast8_t n_iterations = message.length() / 6;
        uint_fast8_t remainder = message.length() % 6;
        uint_fast8_t i;
        for(i = 0; i < n_iterations; ++i) {
            m[i] = (uint8_t *)malloc(6 * sizeof(uint8_t));
            memset(m[i], ' ', 6);
            QString message_substr = message.mid(i * 6, 6).toUpper();
            for(uint_fast8_t j = 0; j < message_substr.length(); ++j) m[i][j] = message_substr.at(j).toLatin1();
        }
        if(remainder) {
            m[i] = (uint8_t *)malloc(6 * sizeof(uint8_t));
            memset(m[i], ' ', 6);
            QString message_substr = message.mid(i * 6, remainder).toUpper();
            for(uint_fast8_t j = 0; j < message_substr.length(); ++j) m[i][j] = message_substr.at(j).toLatin1();
        }
        global_com_instance->transfer_message(m[0], m[1], m[2], m[3], (uint8_t)ui->message_length->toPlainText().toUShort());
        for(auto message : m) if(message) free(message);
    }
}

void FluorescenceApp::on_lisync_sample_clicked()
{
    // Toggle dynamic light sync
    preset_dynamic_light_sync_enable = !preset_dynamic_light_sync_enable;
    if(preset_dynamic_light_sync_enable) {
        ui->lisync_status->setText("Click on the computer to stop.");
    } else {
        ui->lisync_status->setText("Click on the computer to begin.");
    }
}

void FluorescenceApp::on_settings_info_update_clicked()
{
    if(ui->com_select->currentText() == "Bluetooth") {
        error_message("Bluetooth firmware update is not supported (yet). Please perform the firmware update over USB!", QMessageBox::Icon::Information);
        return;
    }
    FWUpdate fw_update_dialog(this, ui->settings_info_fw->text(), ui->com_select->currentText());
    // FWUpdate fw_update_dialog(this, "v3.9222s", ui->com_select->currentText());
    fw_update_dialog.setModal(true);
    fw_update_dialog.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    fw_update_dialog.setFixedSize(321, fw_update_dialog.height());
    fw_update_dialog.setWindowTitle("Fluorescence Updater");
    fw_update_dialog.exec();
}

void FluorescenceApp::on_lsettings_info_update_clicked()
{
    if(ui->com_select->currentText() == "Bluetooth") {
        error_message("Bluetooth firmware update is not supported (yet). Please perform the firmware update over USB!", QMessageBox::Icon::Information);
        return;
    }
    FWUpdate fw_update_dialog(this, "2.x(a)", ui->com_select->currentText());
    fw_update_dialog.setModal(true);
    fw_update_dialog.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    fw_update_dialog.setFixedSize(321, fw_update_dialog.height());
    fw_update_dialog.setWindowTitle("Fluorescence Updater");
    fw_update_dialog.exec();
}

void FluorescenceApp::fw_update_manual_disconnect()
{
    on_com_connect_clicked();
}

void FluorescenceApp::fw_update_manual_dfu_request()
{
    global_com_instance->transfer_dfu_request();
}

void FluorescenceApp::on_settings_info_app_clicked()
{
    infobox about_box;
    about_box.setModal(true);
    about_box.setWindowFlags(Qt::SplashScreen);
    about_box.exec();
}

void FluorescenceApp::on_menu_button_clicked()
{
    if(!ui->menu_controls_layout->isVisible()) {
        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
        ui->menu_controls_layout->setGraphicsEffect(eff);
        QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
        a->setDuration(500);
        a->setStartValue(0);
        a->setEndValue(1);
        a->setEasingCurve(QEasingCurve::InBack);
        a->start(QPropertyAnimation::DeleteWhenStopped);
        ui->menu_button->setPixmap(QPixmap::fromImage(QImage(":/Resources/menu_inv.png")));
        ui->menu_controls_layout->show();
        // hide_all_panels();
    } else {
        ui->menu_button->setPixmap(QPixmap::fromImage(QImage(":/Resources/menu.png")));
        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
        ui->menu_controls_layout->setGraphicsEffect(eff);
        QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
        a->setDuration(500);
        a->setStartValue(1);
        a->setEndValue(0);
        a->setEasingCurve(QEasingCurve::OutBack);
        a->start(QPropertyAnimation::DeleteWhenStopped);
        connect(a, SIGNAL(finished()), this, SLOT(menu_hide_menu()));
    }
}

void FluorescenceApp::menu_hide_menu() {
    ui->menu_controls_layout->hide();
}

