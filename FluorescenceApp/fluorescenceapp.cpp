#include "fluorescenceapp.h"
#include "ui_fluorescenceapp.h"

#include "src/fl_app_time.h"
#include "src/fl_app_lights.h"

#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsRectItem>

#include "QtColorWidgets/colorwidgets_global.hpp"

using namespace color_widgets;

// Global dynamic colors
QWidget *preset_dynamic_colors[NUM_PRESET_DYNAMIC_COLORS];
QWidget *preset_dynamic_time[NUM_PRESET_DYNAMIC_TIME];
uint_fast8_t preset_dynamic_timer = 0;

Light_Pattern global_lights_instance;

// Global COM instance
QSerialPort global_com_port;

// Global time updater, ll support
QTimer *global_timer = nullptr;
vfdco_time_t global_time;

QString bliss_descriptions[6] = {
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

FluorescenceApp::FluorescenceApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FluorescenceApp)
{
    ui->setupUi(this);
    this->setWindowTitle("Fluorescence App by The VFD Collective");
    this->setFixedSize(this->size());

    ui->com_connect->setText("►");
    // List all available serial ports to com_select + empty
    ui->com_select->addItem("");
    for(QSerialPortInfo available_ports : QSerialPortInfo::availablePorts()) {
        ui->com_select->addItem(available_ports.portName());
    }
    hide_all_panels();
    ui->panel_welcome->show();

    // Init timer
    global_timer = new QTimer(this);
    connect(global_timer, &QTimer::timeout, this, QOverload<>::of(&FluorescenceApp::update));
    global_timer->start(50);

    // Init custom color wheel & white
    ui->custom_color_wheel->setDisplayFlag(ColorWheel::COLOR_FLAGS, ColorWheel::COLOR_HSL);
    this->custom_global_color.setHsl(0, 255, 127);
    this->custom_global_color_white = 0;

    ui->custom_slider_w->setValue(custom_global_color_white);
    ui->custom_slider_w->setFirstColor(QColor(0, 0, 0, 0));
    ui->custom_slider_w->setLastColor(QColor(254, 204, 102, 255));

    custom_color_update_all_sliders(0);

    // Init dynamic colors & time. Dude this is so stupid but y know it does the job, kinda.
    QWidget *tmp_dynamic_colors[NUM_PRESET_DYNAMIC_COLORS] = {ui->dynamic_c1, ui->dynamic_c2, ui->dynamic_c3, ui->dynamic_c4, ui->dynamic_c5, ui->dynamic_c6};
    QWidget *tmp_dynamic_time[NUM_PRESET_DYNAMIC_TIME] = {
        ui->dynamic_t_1, ui->dynamic_t_2, ui->dynamic_t_3, ui->dynamic_t_4, ui->dynamic_t_5, ui->dynamic_t_6, ui->dynamic_t_7,
        ui->dynamic_t_8, ui->dynamic_t_9, ui->dynamic_t_10, ui->dynamic_t_11, ui->dynamic_t_12, ui->dynamic_t_13, ui->dynamic_t_14,
        ui->dynamic_t_15, ui->dynamic_t_16, ui->dynamic_t_17, ui->dynamic_t_18, ui->dynamic_t_19, ui->dynamic_t_20, ui->dynamic_t_21
    };
    memcpy(preset_dynamic_colors, tmp_dynamic_colors, NUM_PRESET_DYNAMIC_COLORS * sizeof(QWidget *));
    memcpy(preset_dynamic_time, tmp_dynamic_time, NUM_PRESET_DYNAMIC_TIME * sizeof(QWidget *));
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

void FluorescenceApp::update(){
    // Display current time!
    QTime ct = QTime::currentTime();
    QDate cd = QDate::currentDate();
    ui->timesync_time_label->setText(ct.toString("hh:mm:ss"));
    ui->timesync_date_label->setText(cd.toString(Qt::DefaultLocaleLongDate));

    // Update color wheel
    vfdco_date_t d;
    vfdco_get_date_time(&d, &global_time);
    Light_Pattern_Update(&global_lights_instance);
    ++preset_dynamic_timer;
    for(uint_fast8_t i = 0; i < NUM_PRESET_DYNAMIC_TIME; ++i) {
        uint_fast8_t hpdt = preset_dynamic_timer / 8;
        if(i < hpdt) dynamic_cast<ColorPreview *>(preset_dynamic_time[i])->setColor(QColor::fromRgb(16, 128, 128));
        else dynamic_cast<ColorPreview *>(preset_dynamic_time[i])->setColor(QColor::fromRgb(196, 196, 196));
    }
    if(preset_dynamic_timer == (NUM_PRESET_DYNAMIC_TIME * 8)) {
        preset_dynamic_timer = 0;
    }

}

void FluorescenceApp::hide_all_panels() {
    ui->panel_welcome->hide();
    ui->panel_custom_colors->hide();
    ui->panel_presets->hide();
    ui->panel_message->hide();
    ui->panel_timesync->hide();
    ui->panel_settings->hide();
}

void FluorescenceApp::on_com_connect_clicked()
{
    QIODevice::OpenMode open_mode;
    if(ui->com_label_connect->text() == "Connect") {
        global_com_port.close();
        global_com_port.setPortName(ui->com_label_connect->text());
        global_com_port.setBaudRate(QSerialPort::BaudRate::Baud115200);
        global_com_port.setDataBits(QSerialPort::DataBits::Data8);
        global_com_port.setParity(QSerialPort::Parity::NoParity);
        global_com_port.setStopBits(QSerialPort::StopBits::OneStop);

        global_com_port.open(open_mode);

        ui->panel_main_control->setEnabled(true);
        ui->com_label_connect->setText("Disconnect");
        ui->com_connect->setText("◼");
    } else if(ui->com_label_connect->text() == "Disconnect") {
        ui->com_select->setCurrentIndex(0); // Empty
        ui->com_label_connect->setText("Connect");
        ui->com_connect->setText("►");
        ui->panel_main_control->setEnabled(false);

        hide_all_panels();
        ui->panel_welcome->show();
    } else {
        // Oops, you should never get to this point
    }
}

void FluorescenceApp::on_static_rainbow_clicked() { ui->static_rainbow->hide(); }
void FluorescenceApp::on_static_pastelrainbow_clicked(){ ui->static_pastelrainbow->hide(); }

void FluorescenceApp::on_main_preset_clicked()
{
    hide_all_panels();
    ui->panel_presets->show();
}

void FluorescenceApp::on_main_custom_clicked()
{
    hide_all_panels();
    ui->panel_custom_colors->show();
}

void FluorescenceApp::on_main_timesync_clicked()
{
    hide_all_panels();
    ui->panel_timesync->show();
}

void FluorescenceApp::on_main_message_clicked()
{
    hide_all_panels();
    ui->panel_message->show();
}

void FluorescenceApp::on_main_settings_clicked()
{
    hide_all_panels();
    ui->panel_settings->show();
}


void FluorescenceApp::custom_color_update_all_sliders(bool block_color_wheel) {
    // Mutual exclusion
    bool blocked = signalsBlocked();
    blockSignals(true);
    Q_FOREACH(QWidget* w, findChildren<QWidget*>())
        w->blockSignals(true);

    if(!block_color_wheel) ui->custom_color_wheel->setColor(custom_global_color);

    ui->custom_value_h->setValue((double)custom_global_color.hueF() * 359.0f);
    ui->custom_value_s->setValue(custom_global_color.hslSaturationF() * 255.0f);
    ui->custom_value_l->setValue(custom_global_color.lightnessF() * 255.0f);
    ui->custom_value_r->setValue(custom_global_color.red());
    ui->custom_value_g->setValue(custom_global_color.green());
    ui->custom_value_b->setValue(custom_global_color.blue());

    ui->custom_value_w->setValue(custom_global_color_white);

    ui->custom_slider_h->setColorHue(custom_global_color.hslHueF());
    ui->custom_slider_s->setValue(custom_global_color.hslSaturationF() * 255.0f);
    ui->custom_slider_s->setFirstColor(QColor::fromHslF(custom_global_color.hueF(), 0, custom_global_color.lightnessF()));
    ui->custom_slider_s->setLastColor(QColor::fromHslF(custom_global_color.hueF(), 1, custom_global_color.lightnessF()));
    ui->custom_slider_l->setValue(custom_global_color.lightnessF() * 255.0f);
    ui->custom_slider_l->setFirstColor(QColor::fromHsvF(custom_global_color.hueF(), custom_global_color.saturationF(), 0));
    ui->custom_slider_l->setLastColor(QColor::fromHsvF(custom_global_color.hueF(), custom_global_color.saturationF(), 1));

    ui->custom_slider_r->setValue(custom_global_color.red());
    ui->custom_slider_r->setFirstColor(QColor(0, custom_global_color.green(), custom_global_color.blue()));
    ui->custom_slider_r->setLastColor(QColor(255, custom_global_color.green(), custom_global_color.blue()));
    ui->custom_slider_g->setValue(custom_global_color.green());
    ui->custom_slider_g->setFirstColor(QColor(custom_global_color.red(), 0, custom_global_color.blue()));
    ui->custom_slider_g->setLastColor(QColor(custom_global_color.red(), 255, custom_global_color.blue()));
    ui->custom_slider_b->setValue(custom_global_color.blue());
    ui->custom_slider_b->setFirstColor(QColor(custom_global_color.red(), custom_global_color.green(), 0));
    ui->custom_slider_b->setLastColor(QColor(custom_global_color.red(), custom_global_color.green(), 255));

    ui->custom_value_hex->setColor(custom_global_color);

    ui->custom_slider_w->setValue(custom_global_color_white);


    blockSignals(blocked);
    Q_FOREACH(QWidget* w, findChildren<QWidget*>())
        w->blockSignals(false);
}

void FluorescenceApp::on_custom_slider_r_sliderMoved(int position)
{
    if (!signalsBlocked()) {
        custom_global_color.setRed(position);
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_slider_g_sliderMoved(int position)
{
    if (!signalsBlocked()) {
        custom_global_color.setGreen(position);
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_slider_b_sliderMoved(int position)
{
    if (!signalsBlocked()) {
        custom_global_color.setBlue(position);
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_slider_w_sliderMoved(int position)
{
    if (!signalsBlocked()) {
        custom_global_color_white = position;
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_slider_h_sliderMoved(int position)
{
    if (!signalsBlocked()) {
        QColor new_color;
        new_color.setHslF((double)position / 359.0f, custom_global_color.hslSaturationF(), custom_global_color.lightnessF());
        custom_global_color = new_color;
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_slider_s_sliderMoved(int position)
{
    if (!signalsBlocked()) {
        QColor new_color;
        new_color.setHslF(custom_global_color.hueF(), position / 255.0f, custom_global_color.lightnessF());
        custom_global_color = new_color;
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_slider_l_sliderMoved(int position)
{
    if (!signalsBlocked()) {
        QColor new_color;
        new_color.setHslF(custom_global_color.hueF(), custom_global_color.hslSaturationF(), position / 255.0f);
        custom_global_color = new_color;
        custom_color_update_all_sliders(0);
    }
}
void FluorescenceApp::on_custom_value_hex_colorEditingFinished(const QColor &color)
{
    if (!signalsBlocked()) {
        custom_global_color = color;
        custom_color_update_all_sliders(0);
    }
}


void FluorescenceApp::on_custom_value_r_valueChanged(int arg1) { on_custom_slider_r_sliderMoved(arg1); }
void FluorescenceApp::on_custom_value_g_valueChanged(int arg1) { on_custom_slider_g_sliderMoved(arg1); }
void FluorescenceApp::on_custom_value_b_valueChanged(int arg1) { on_custom_slider_b_sliderMoved(arg1); }
void FluorescenceApp::on_custom_value_w_valueChanged(int arg1) { on_custom_slider_w_sliderMoved(arg1); }
void FluorescenceApp::on_custom_value_h_valueChanged(int arg1) { on_custom_slider_h_sliderMoved(arg1); }
void FluorescenceApp::on_custom_value_s_valueChanged(int arg1) { on_custom_slider_s_sliderMoved(arg1); }
void FluorescenceApp::on_custom_value_l_valueChanged(int arg1) { on_custom_slider_l_sliderMoved(arg1); }

void FluorescenceApp::on_custom_color_wheel_colorSelected(const QColor &arg1)
{
    if (!signalsBlocked()) {
        custom_global_color = arg1;
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
