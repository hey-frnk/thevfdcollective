#ifndef FL_APP_COM_H
#define FL_APP_COM_H

#include "src/fl_app_time.h"
#include "../FluorescenceV3/Commons/vfdco_clock_routine_defines.h"

#include <QObject>
#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QBluetoothSocket>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>

#define BUF_RX_SIZE 10
#define BUF_TX_SIZE_LEGACY 24
#define BUF_TX_SIZE 27

typedef enum {
    FL_APP_COM_STATUS_OK,
    FL_APP_COM_STATUS_CONNECTION_FAILED
} fl_app_status_t;

const QString bt_device_name("FLUORESCENCE");

class fl_app_com : public QObject
{
    Q_OBJECT

public:
    fl_app_com();                          // Bluetooth constructor
    fl_app_com(const QString portname);    // UART (USB) constructor
    ~fl_app_com();

    void transfer_serial0(uint8_t *clr_arr); // Legacy: No warm white LED
    void transfer_serial1(uint8_t *clr_arr); // Legacy: No warm white LED

    void transfer_light_pattern(uint8_t instance, uint8_t param0, uint8_t param1); // Legacy: Only param0 relevant
    void transfer_enable_presets(uint8_t enabled_instances); // Legacy: Unsupported
    void transfer_random_set(uint8_t enabled_instances, random_speed_t speed); // Legacy: Unsupported

    void transfer_gui_set(uint8_t instance, uint8_t param0); // Legacy: Unsupported
    void legacy_transfer_clock_flags(uint8_t clock_flag, uint8_t clock_flag_value);

    QString transfer_time_date(vfdco_time_t t, vfdco_date_t d); // Legacy: Fully compatible

    void transfer_brightness(uint8_t disp_or_led, uint8_t dim_factor); // Legacy: Unsupported
    void transfer_night_shift(vfdco_time_t start, vfdco_time_t stop); // Legacy: Unsupported
    void legacy_transfer_night_shift(vfdco_time_t start, vfdco_time_t stop, uint8_t en1, uint8_t en2); // Legacy: Only

    void transfer_welcome_set(uint8_t *message); // Legacy: Fully compatible
    void transfer_message(uint8_t *m1, uint8_t *m2, uint8_t *m3, uint8_t *m4, uint8_t duration); // Legacy: Up to m3
    QString transfer_clock_control(com_protocol_clock_control_t control); // Legacy: No HW request

    void transfer_dfu_request(void); // Legacy: Unsupported

    bool legacy_protocol();
    fl_app_status_t getStatus();

signals:
    void app_com_connected();
    void bt_incoming_data();
    void bt_status_changed(QString message);

private slots:
    // Bluetooth Discovery
    void bt_discovery_finished();

    // Bluetooth device connection & service
    void bt_connect_device_connected();
    void bt_connect_device_error(QLowEnergyController::Error /*error*/);
    void bt_connect_device_disconnected();
    void bt_connect_services_add(const QBluetoothUuid &serviceUuid);
    void bt_connect_services_scan_complete();
    void bt_connect_services_details_discovered(QLowEnergyService::ServiceState);

    // Incoming data
    void characteristicChanged(QLowEnergyCharacteristic, QByteArray);
private:
    fl_app_status_t status;

    bool is_legacy_protocol, is_bluetooth_communication;
    uint8_t *buf_rx;
    uint8_t *buf_tx;

    QByteArray incoming_characteristic;
    bool incoming_characteristic_present;

    QSerialPort serial_port;

    void determine_legacy();

    void clear_buffer(void);
    void set_command_byte(uint8_t value);

    void transfer_serial(uint8_t *clr_arr, uint8_t mode); // Legacy: No warm white LED

    void (fl_app_com::*_transfer)(void);
    void transfer(void);
    void regular_transfer(void);
    void legacy_transfer(void);
    QString receive_and_extract_data(int wait_timeout);
    QByteArray receive_raw(int wait_timeout);

    QLowEnergyController *controller;
    QBluetoothLocalDevice *localDevice;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;

    // Keep a record of all devices:
    QList<QBluetoothDeviceInfo> deviceList;
    QList<QLowEnergyService *> serviceList;

    QBluetoothDeviceInfo *currentDevice;
    QLowEnergyService *currentService;
    QLowEnergyCharacteristic *currentCharacteristic;
};

#endif // FL_APP_COM_H
