#ifndef FLUORESCENCEAPP_H
#define FLUORESCENCEAPP_H

#include <QMainWindow>
#include <QMessageBox>
#include "fl_app_com.h"
#include "src/fl_app_lights.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FluorescenceApp; }
QT_END_NAMESPACE

#define NUM_PRESET_DYNAMIC_COLORS 6
#define NUM_PRESET_DYNAMIC_TIME 21
#define NUM_PRESET_LIGHT_SYNC 6

class FluorescenceApp : public QMainWindow
{
    Q_OBJECT
public:
    FluorescenceApp(QWidget *parent = nullptr);
    ~FluorescenceApp();
    void update();

public slots:
    void app_com_connected_callback();
    void app_com_status_callback(QString status);

private slots:

    void on_com_connect_clicked();

    void on_static_rainbow_clicked();

    void on_static_pastelrainbow_clicked();

    void on_main_preset_clicked();

    void on_main_custom_clicked();

    void on_custom_slider_r_valueChanged(int position);

    void on_custom_slider_g_valueChanged(int position);

    void on_custom_slider_b_valueChanged(int position);

    void on_custom_value_r_valueChanged(int arg1);

    void on_custom_value_g_valueChanged(int arg1);

    void on_custom_value_b_valueChanged(int arg1);

    void on_custom_value_w_valueChanged(int arg1);

    void on_custom_value_h_valueChanged(int arg1);

    void on_custom_value_s_valueChanged(int arg1);

    void on_custom_value_l_valueChanged(int arg1);

    void on_custom_slider_h_valueChanged(int position);

    void on_custom_slider_s_valueChanged(int position);

    void on_custom_slider_l_valueChanged(int position);

    void on_custom_slider_w_valueChanged(int position);

    // void on_custom_value_hex_colorEditingFinished(const QColor &color);

    void on_main_timesync_clicked();

    void on_main_message_clicked();

    // void on_custom_color_wheel_colorSelected(const QColor &arg1);

    void on_dynamic_spectrum_clicked();

    void on_dynamic_bliss_dnc_clicked();

    void on_dynamic_rainbow_clicked();

    void on_dynamic_chase_clicked();

    void on_dynamic_timecode_clicked();

    void on_dynamic_police_clicked();

    void on_panel_dyn_bliss_param_currentIndexChanged(int index);

    void on_main_settings_clicked();

    void on_settings_td_set_clicked();

    void on_settings_bri_set_clicked();

    void on_settings_nsh_enable_stateChanged(int arg1);

    void on_settings_nsh_set_clicked();

    void on_settings_settings_default_clicked();

    void on_settings_settings_save_clicked();

    void on_settings_info_download_clicked();

    void on_lsettings_td_12hr_enable_stateChanged(int arg1);

    void on_lsettings_td_l0_enable_stateChanged(int arg1);

    void on_lsettings_td_intldate_enable_stateChanged(int arg1);

    void on_lsettings_nsh_enable_stateChanged(int arg1);

    void on_lsettings_nsh_scheduled_enable_stateChanged(int arg1);

    void on_lsettings_settings_default_clicked();

    void on_lsettings_settings_save_clicked();

    void on_lsettings_info_download_clicked();

    void on_timesync_button_clicked();

    void on_static_off_clicked();

    void on_static_white_clicked();

    void on_static_red_clicked();

    void on_static_warmwhite_clicked();

    void on_static_green_clicked();

    void on_static_blue_clicked();

    void on_static_yellow_clicked();

    void on_static_orange_clicked();

    void on_static_cyan_clicked();

    void on_static_magenta_clicked();

    void on_static_violet_clicked();

    void on_static_green2blue_clicked();

    void on_static_red2blue_clicked();

    void on_static_red2green_clicked();

    void on_panel_dyn_spectrum_set_clicked();

    void on_panel_dyn_bliss_set_clicked();

    void on_panel_dyn_rnb_set_clicked();

    void on_panel_dyn_chase_set_clicked();

    void on_panel_dyn_tcode_set_clicked();

    void on_panel_dyn_cop_set_clicked();

    void on_panel_dyn_music_set_clicked();

    void on_message_send_clicked();

    void on_lisync_sample_clicked();

    void on_settings_presets_en_set_clicked();

    void on_shuffle_set_clicked();

    void on_dynamic_music_clicked();

    void on_settings_info_update_clicked();

    void on_com_select_currentTextChanged(const QString &arg1);

    void on_custom_color_wheel_mousePressed(const QPoint &);

    void on_custom_color_wheel_cursor_mouseMoved(const QPoint &);

    void on_lsettings_info_update_clicked();

    void on_custom_value_save_clicked();

    void on_custom_value_load_clicked();

    void on_settings_info_app_clicked();

    void on_menu_button_clicked();

    void menu_hide_menu();

    void on_main_welcome_clicked();

    void on_com_reload_clicked();

    void on_timesync_time_label_clicked();

    void on_settings_digit_fade_set_clicked();

public:
    Ui::FluorescenceApp *ui;

    fl_app_com *global_com_instance = nullptr;

    QColor custom_global_color;
    QColor custom_led_colors[6];
    uint8_t custom_global_color_white;
    uint8_t custom_led_white[6];

    uint_fast8_t preset_dynamic_timer = 0;

    uint_fast8_t preset_dynamic_light_sync_timer = 0;
    uint_fast8_t preset_dynamic_light_sync_counter = 0;
    bool preset_dynamic_light_sync_enable = false;

    Light_Pattern global_lights_instance;

    // Global time updater, ll support
    QTimer *global_timer = nullptr;
    vfdco_time_t global_time;

    // Todo replace bool by fancy function
    bool global_is_fw2 = false;

    void hide_all_panels();
    void hide_all_dynamic_control_panels();
    void clear_lights_instance();
    void custom_color_update_all_sliders(bool);
    void error_message(QString message, QMessageBox::Icon i);
    float capture_hue(const QPoint&);
    QPoint map_hue(float angle);

    void fw_update_manual_disconnect();
    void fw_update_manual_dfu_request();

    void preset_ambient_light_update(uint_fast8_t counter);
};
#endif // FLUORESCENCEAPP_H
