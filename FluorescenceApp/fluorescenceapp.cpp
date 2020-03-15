#include "fluorescenceapp.h"
#include "ui_fluorescenceapp.h"

#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsRectItem>

#include "QtColorWidgets/colorwidgets_global.hpp"

#include "src/fl_app_lights.h"

using namespace color_widgets;

// Global COM instance
QSerialPort global_com_port;

// Global time updater
QTimer *global_timer = nullptr;

FluorescenceApp::FluorescenceApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FluorescenceApp)
{
    ui->setupUi(this);
    this->setWindowTitle("Fluorescence App");

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
    global_timer->start(1000);

    // Init custom color wheel & white
    ui->custom_color_wheel->setDisplayFlag(ColorWheel::COLOR_FLAGS, ColorWheel::COLOR_HSL);
    custom_global_color.setHsl(0, 255, 127);
    custom_global_color_white = 0;

    ui->custom_slider_w->setValue(custom_global_color_white);
    ui->custom_slider_w->setFirstColor(QColor(0, 0, 0, 0));
    ui->custom_slider_w->setLastColor(QColor(254, 204, 102, 255));

    custom_color_update_all_sliders(0);
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
}

void FluorescenceApp::hide_all_panels() {
    ui->panel_welcome->hide();
    ui->panel_custom_colors->hide();
    ui->panel_presets->hide();
    ui->panel_message->hide();
    ui->panel_timesync->hide();
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

void FluorescenceApp::on_custom_color_wheel_colorChanged(const QColor &arg1)
{
    if (!signalsBlocked()) {
        custom_global_color = arg1;
        custom_color_update_all_sliders(1);
    }
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
