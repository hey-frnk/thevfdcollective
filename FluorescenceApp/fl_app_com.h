#ifndef FL_APP_COM_H
#define FL_APP_COM_H

#include <QMainWindow>
#include <QObject>
#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "src/fl_app_time.h"

#define BUF_RX_SIZE 10
#define BUF_TX_SIZE_LEGACY 24
#define BUF_TX_SIZE 27

#define FL_APP_COM_CONTROL_BYTE 1
#define FL_APP_COM_DATA_OFFSET 2

typedef enum {
    FL_APP_COM_CONTROL_HW_VERSION_REQ = 0,
    FL_APP_COM_CONTROL_FW_VERSION_REQ = 1,
    FL_APP_COM_CONTROL_SETTINGS_SAVE_REQ = 2,
    FL_APP_COM_CONTROL_DEFAULT_REQ = 3
} fl_app_com_control_t;

typedef enum {
    FL_APP_COM_STATUS_OK,
    FL_APP_COM_STATUS_CONNECTION_FAILED
} fl_app_status_t;

class fl_app_com
{
public:
    fl_app_com(const QString portname);
    ~fl_app_com();

    void transfer_serial0(uint8_t *clr_arr); // Legacy: No warm white LED
    void transfer_serial1(uint8_t *clr_arr); // Legacy: No warm white LED

    void transfer_light_pattern(uint8_t instance, uint8_t param0, uint8_t param1); // Legacy: Only param0 relevant

    void transfer_gui_set(uint8_t instance, uint8_t param0); // Legacy: Unsupported
    void legacy_transfer_clock_flags(uint8_t clock_flag, uint8_t clock_flag_value);

    QString transfer_time_date(vfdco_time_t t, vfdco_date_t d); // Legacy: Fully compatible

    void transfer_brightness(uint8_t disp_or_led, uint8_t dim_factor); // Legacy: Unsupported
    void transfer_night_shift(vfdco_time_t start, vfdco_time_t stop); // Legacy: Unsupported
    void legacy_transfer_night_shift(vfdco_time_t start, vfdco_time_t stop, uint8_t en1, uint8_t en2); // Legacy: Only

    void transfer_welcome_set(uint8_t *message); // Legacy: Fully compatible
    void transfer_message(uint8_t *m1, uint8_t *m2, uint8_t *m3, uint8_t *m4, uint8_t duration); // Legacy: Up to m3
    QString transfer_clock_control(fl_app_com_control_t control); // Legacy: No HW request

    bool legacy_protocol();
    fl_app_status_t getStatus();
private:
    fl_app_status_t status;

    bool is_legacy_protocol;
    uint8_t *buf_rx;
    uint8_t *buf_tx;
    QSerialPort serial_port;

    void clear_buffer(void);
    void set_control_byte(uint8_t value);

    void transfer_serial(uint8_t *clr_arr, uint8_t mode); // Legacy: No warm white LED

    void (fl_app_com::*_transfer)(void);
    void transfer(void);
    void regular_transfer(void);
    void legacy_transfer(void);
    QString receive(int wait_timeout);
    QByteArray receive_raw(int wait_timeout);
};

#endif // FL_APP_COM_H
