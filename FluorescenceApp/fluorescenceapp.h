#ifndef FLUORESCENCEAPP_H
#define FLUORESCENCEAPP_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class FluorescenceApp; }
QT_END_NAMESPACE

#define NUM_PRESET_DYNAMIC_COLORS 6
#define NUM_PRESET_DYNAMIC_TIME 21

class FluorescenceApp : public QMainWindow
{
    Q_OBJECT

public:
    FluorescenceApp(QWidget *parent = nullptr);
    ~FluorescenceApp();
    void update();

private slots:

    void on_com_connect_clicked();

    void on_static_rainbow_clicked();

    void on_static_pastelrainbow_clicked();

    void on_main_preset_clicked();

    void on_main_custom_clicked();

    void on_custom_slider_r_sliderMoved(int position);

    void on_custom_slider_g_sliderMoved(int position);

    void on_custom_slider_b_sliderMoved(int position);

    void on_custom_value_r_valueChanged(int arg1);

    void on_custom_value_g_valueChanged(int arg1);

    void on_custom_value_b_valueChanged(int arg1);

    void on_custom_value_w_valueChanged(int arg1);

    void on_custom_value_h_valueChanged(int arg1);

    void on_custom_value_s_valueChanged(int arg1);

    void on_custom_value_l_valueChanged(int arg1);

    void on_custom_slider_h_sliderMoved(int position);

    void on_custom_slider_s_sliderMoved(int position);

    void on_custom_slider_l_sliderMoved(int position);

    void on_custom_slider_w_sliderMoved(int position);

    void on_custom_value_hex_colorEditingFinished(const QColor &color);

    void on_main_timesync_clicked();

    void on_main_message_clicked();

    void on_custom_color_wheel_colorSelected(const QColor &arg1);

    void on_dynamic_spectrum_clicked();

    void on_dynamic_bliss_dnc_clicked();

    void on_dynamic_rainbow_clicked();

    void on_dynamic_chase_clicked();

    void on_dynamic_timecode_clicked();

    void on_dynamic_police_clicked();

    void on_panel_dyn_bliss_param_currentIndexChanged(int index);

    void on_main_settings_clicked();

private:
    Ui::FluorescenceApp *ui;

    QColor custom_global_color;
    uint8_t custom_global_color_white;

    void hide_all_panels();
    void hide_all_dynamic_control_panels();
    void clear_lights_instance();
    void custom_color_update_all_sliders(bool);
};
#endif // FLUORESCENCEAPP_H
