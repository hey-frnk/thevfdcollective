#include "fluorescencesimulator.h"
#include "ui_fluorescencesimulator.h"

#include <QDateTime>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

#include "../vfdco_config.h"
#include "../vfdco_clock_routines.h"
#include "../vfdco_hid.h"
#include "../vfdco_time.h"
#include "../vfdco_lights.h"
#include "../vfdco_gui.h"
#include "magic_enum.hpp"

// Mappings
QLabel *display[CONFIG_NUM_DIGITS][8] = {{nullptr}};
QLabel *display_bin[CONFIG_NUM_DIGITS] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

QLabel *led[CONFIG_NUM_PIXELS] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
QLabel *led_rgbw[CONFIG_NUM_PIXELS] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
QLabel *led_hexw[CONFIG_NUM_PIXELS] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
QLabel *led_hsl[CONFIG_NUM_PIXELS] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

// Color palette
QColor active_color("#339999");
QColor inactive_color("#DDDDDD");

#define PROTOCOL_DATA_OFFSET_ROWS 1
#define PROTOCOL_DATA_OFFSET_COLS 2
#define PROTOCOL_DATA_LENGTH 27
#define PROTOCOL_DATA_COMMAND_OFFSET 1

extern uint8_t *const serialized_settings[NUM_SERIALIZABLE];

typedef struct setting_property {
    QString identifier;
    QString description;
    uint8_t setting_offset;
    uint8_t setting_size;
} setting_property;

typedef struct settings_arr_property {
    QString settings_identifier;
    uint8_t index;
    uint8_t size;
    std::vector<setting_property *> settings;
} settings_arr_property;

// Fill settings tree
settings_arr_property settings_arr_names[NUM_SERIALIZABLE] = {
    // Generate setting arrays vfdco_config.h
    #define CREATE_SERIALIZED_GLOBAL_TREESTR(_globalindex, _size, _enum_map, _serializable_identifier) {.settings_identifier = #_serializable_identifier, .index = _globalindex, .size = _size },
    CREATE_SERIALIZED_GLOBAL(CREATE_SERIALIZED_GLOBAL_TREESTR)
    #undef CREATE_SERIALIZED_GLOBAL_TREESTR

    #define CREATE_SERIALIZED_GUI_TREESTR(_globalindex, _size, _enum_map, _serializable_identifier) {.settings_identifier = #_serializable_identifier, .index = _globalindex, .size = _size },
    CREATE_SERIALIZED_GUI(CREATE_SERIALIZED_GUI_TREESTR)
    #undef CREATE_SERIALIZED_GUI_TREESTR

    #define CREATE_SERIALIZED_LIGHTS_TREESTR(_globalindex, _size, _enum_map, _serializable_identifier) {.settings_identifier = #_serializable_identifier, .index = _globalindex, .size = _size },
    CREATE_SERIALIZED_LIGHTS(CREATE_SERIALIZED_LIGHTS_TREESTR)
    #undef CREATE_SERIALIZED_LIGHTS_TREESTR
};
setting_property serialized_setting_names[] = {
    // Generate individual settings from vfdco_config.h
    #define CREATE_SETTING_GLOBAL_TREESTR(_offset, _size, _setting_identifier, _description) {.identifier = #_setting_identifier, .description = _description, .setting_offset = _offset, .setting_size = _size},
    CREATE_SERIALIZED_GLOBAL_POSITIONS(CREATE_SETTING_GLOBAL_TREESTR)
    #undef CREATE_SETTING_GLOBAL_TREESTR

    #define CREATE_SETTING_GUI_TREESTR(_offset, _size, _setting_identifier, _description) {.identifier = #_setting_identifier, .description = _description, .setting_offset = _offset, .setting_size = _size},
    CREATE_SERIALIZED_GUI_POSITIONS(CREATE_SETTING_GUI_TREESTR)
    #undef CREATE_SETTING_GUI_TREESTR

    #define CREATE_SETTING_LIGHT_PATTERN_TREESTR(_offset, _size, _setting_identifier, _description) {.identifier = #_setting_identifier, .description = _description, .setting_offset = _offset, .setting_size = _size},
    CREATE_SERIALIZED_LIGHTS_POSITIONS(CREATE_SETTING_LIGHT_PATTERN_TREESTR)
    #undef CREATE_SETTING_LIGHT_PATTERN_TREESTR
};

// Timers
int clk_div = 1;
time_event_t global_time_event;

// from vfdco_clock_routines: get instances & instance counter
extern Light_Pattern global_light_instance;
extern light_pattern_instance_t global_light_instance_counter;
extern GUI_Format global_gui_instance;
extern gui_instance_t global_gui_instance_counter;

FluorescenceSimulator::FluorescenceSimulator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FluorescenceSimulator)
{
    ui->setupUi(this);

    // Init timer
    global_timer = new QTimer(this);
    global_time_event = Time_Event_Init(100);
    connect(global_timer, &QTimer::timeout, this, QOverload<>::of(&FluorescenceSimulator::update));

    link_ui_instances();
}

FluorescenceSimulator::~FluorescenceSimulator()
{
    delete global_timer;
    delete ui;
}

void FluorescenceSimulator::update()
{
    vfdco_clock_routine();
    if(Time_Event_Update(&global_time_event)) {
        QDateTime cdt = QDateTime::currentDateTime();

        // Print updating instance
        auto _gui_active = magic_enum::enum_name(global_gui_instance_counter);
        auto _light_active = magic_enum::enum_name(global_light_instance_counter);
        ui->widget_gui->setTitle("GUI & Display Monitor. Active Instance: " + QString::fromStdString(std::string(_gui_active)) + ". RTC Respond: " + cdt.currentDateTime().toString());
        ui->widget_lights->setTitle("Lights Monitor. Active Instance: " + QString::fromStdString(std::string(_light_active)));

        // Get raw data by type punning
        QByteArray _gui_data = QByteArray((char *)&global_gui_instance, sizeof(GUI_Format));
        QByteArray _lights_data = QByteArray((char *)&global_light_instance, sizeof(Light_Pattern));
        QString _gui_str, _lights_str;
        if(ui->raw_hex->isChecked()) {
            for(char c : _gui_data) _gui_str += QString("%1 ").arg((uint8_t)c, 2, 16, QChar('0')).toUpper();
            for(char c : _lights_data) _lights_str += QString("%1 ").arg((uint8_t)c, 2, 16, QChar('0')).toUpper();
        } else {
            for(char c : _gui_data) _gui_str += QString("%1 ").arg((uint8_t)c, 3, 10, QChar('0'));
            for(char c : _lights_data) _lights_str += QString("%1 ").arg((uint8_t)c, 3, 10, QChar('0'));
        }
        ui->raw_val_gui->setPlainText(_gui_str);
        ui->raw_val_lights->setPlainText(_lights_str);

        if(ui->settings_tree->currentItem()) {
            QModelIndex _current_item_index = ui->settings_tree->currentIndex();
            uint8_t current_item_index = _current_item_index.row();
            // If it is a child
            if(!ui->settings_tree->currentItem()->childCount()) {
                uint8_t parent_item_index = _current_item_index.parent().row();
                // Get description & identifier of property
                setting_property *p = settings_arr_names[parent_item_index].settings.at(current_item_index);
                uint8_t _target_size = p->setting_size;
                ui->settings_type_val->setText(QString::number(_target_size) + "x uint8_t. Setting.");
                ui->settings_selected_val->setText(p->description);
                char _tmp_data[_target_size];
                memcpy(_tmp_data, serialized_settings[parent_item_index] + p->setting_offset, _target_size * sizeof(uint8_t));
                QByteArray _settings_arr = QByteArray(_tmp_data, _target_size);
                QString _settings_arr_tostring;
                if(ui->settings_val_hex->isChecked())           for(char c : _settings_arr) _settings_arr_tostring += QString("%1 ").arg((uint8_t)c, 2, 16, QChar('0')).toUpper();
                else if(ui->settings_val_dec->isChecked())      for(char c : _settings_arr) _settings_arr_tostring += QString("%1 ").arg((uint8_t)c, 3, 10, QChar('0'));
                else if(ui->settings_val_ascii->isChecked())    _settings_arr_tostring = QString::fromLatin1(_settings_arr.data());
                else                                            for(char c : _settings_arr) _settings_arr_tostring += QString("%1 ").arg((uint8_t)c, 8, 2, QChar('0'));
                ui->settings_val_val->setPlainText(_settings_arr_tostring);
            } else {
                // Set text to data of whole setting
                uint8_t _target_size = settings_arr_names[current_item_index].size;
                ui->settings_type_val->setText(QString::number(_target_size) + "x uint8_t. Setting array.");
                ui->settings_selected_val->setText(settings_arr_names[current_item_index].settings_identifier);
                char _tmp_data[_target_size];
                memcpy(_tmp_data, serialized_settings[current_item_index], _target_size * sizeof(uint8_t));
                QByteArray _settings_arr = QByteArray(_tmp_data, _target_size);
                QString _settings_arr_tostring;
                if(ui->settings_val_hex->isChecked())           for(char c : _settings_arr) _settings_arr_tostring += QString("%1 ").arg((uint8_t)c, 2, 16, QChar('0')).toUpper();
                else if(ui->settings_val_dec->isChecked())      for(char c : _settings_arr) _settings_arr_tostring += QString("%1 ").arg((uint8_t)c, 3, 10, QChar('0'));
                else if(ui->settings_val_ascii->isChecked())    _settings_arr_tostring = QString::fromLatin1(_settings_arr.data());
                else                                            for(char c : _settings_arr) _settings_arr_tostring += QString("%1 ").arg((uint8_t)c, 8, 2, QChar('0'));
                ui->settings_val_val->setPlainText(_settings_arr_tostring);
            }
        }
    }
}

void FluorescenceSimulator::link_ui_instances()
{
    // Link VFD Display
    QLabel *_row_hrd[8] = {ui->hr_d_a, ui->hr_d_b, ui->hr_d_c, ui->hr_d_d, ui->hr_d_e, ui->hr_d_f, ui->hr_d_g, ui->hr_d_hdp};
    QLabel *_row_hr[8] = {ui->hr_a, ui->hr_b, ui->hr_c, ui->hr_d, ui->hr_e, ui->hr_f, ui->hr_g, ui->hr_hdp};
    QLabel *_row_mind[8] = {ui->min_d_a, ui->min_d_b, ui->min_d_c, ui->min_d_d, ui->min_d_e, ui->min_d_f, ui->min_d_g, ui->min_d_hdp};
    QLabel *_row_min[8] = {ui->min_a, ui->min_b, ui->min_c, ui->min_d, ui->min_e, ui->min_f, ui->min_g, ui->min_hdp};
    QLabel *_row_secd[8] = {ui->sec_d_a, ui->sec_d_b, ui->sec_d_c, ui->sec_d_d, ui->sec_d_e, ui->sec_d_f, ui->sec_d_g, ui->sec_d_hdp};
    QLabel *_row_sec[8] = {ui->sec_a, ui->sec_b, ui->sec_c, ui->sec_d, ui->sec_e, ui->sec_f, ui->sec_g, ui->sec_hdp};
    memcpy(display[0], _row_hrd, 8 * sizeof(QLabel **));
    memcpy(display[1], _row_hr, 8 * sizeof(QLabel **));
    memcpy(display[2], _row_mind, 8 * sizeof(QLabel **));
    memcpy(display[3], _row_min, 8 * sizeof(QLabel **));
    memcpy(display[4], _row_secd, 8 * sizeof(QLabel **));
    memcpy(display[5], _row_sec, 8 * sizeof(QLabel **));
    // Link VFD Display Labels
    QLabel *_display_bin[CONFIG_NUM_DIGITS] = {ui->gui_bin_1, ui->gui_bin_2, ui->gui_bin_3, ui->gui_bin_4, ui->gui_bin_5, ui->gui_bin_6};
    memcpy(display_bin, _display_bin, CONFIG_NUM_DIGITS * sizeof(QLabel *));

    // Link LEDs
    QLabel *_led[CONFIG_NUM_PIXELS] = {ui->led1, ui->led2, ui->led3, ui->led4, ui->led5, ui->led6};
    memcpy(led, _led, CONFIG_NUM_PIXELS * sizeof(QLabel *));
    // Link LED RGBW, HEXW, HSL labels
    QLabel *_led_rgbw[CONFIG_NUM_PIXELS] = {ui->led_rgbw_1, ui->led_rgbw_2, ui->led_rgbw_3, ui->led_rgbw_4, ui->led_rgbw_5, ui->led_rgbw_6};
    memcpy(led_rgbw, _led_rgbw, CONFIG_NUM_PIXELS * sizeof(QLabel *));
    QLabel *_led_hexw[CONFIG_NUM_PIXELS] = {ui->led_hex_1, ui->led_hex_2, ui->led_hex_3, ui->led_hex_4, ui->led_hex_5, ui->led_hex_6};
    memcpy(led_hexw, _led_hexw, CONFIG_NUM_PIXELS * sizeof(QLabel *));
    QLabel *_led_hsl[CONFIG_NUM_PIXELS] = {ui->led_hsl_1, ui->led_hsl_2, ui->led_hsl_3, ui->led_hsl_4, ui->led_hsl_5, ui->led_hsl_6};
    memcpy(led_hsl, _led_hsl, CONFIG_NUM_PIXELS * sizeof(QLabel *));
}

void FluorescenceSimulator::fill_settings_tree()
{
    ui->settings_tree->setColumnCount(1);
    ui->settings_tree->setHeaderLabel("uint8_t *const serialized_settings[" + QString("%1").arg(NUM_SERIALIZABLE, 1, 10) + "]");

    uint16_t properties_counter = 0;
    for(settings_arr_property &property : settings_arr_names) {
        auto _item = new QTreeWidgetItem(QStringList() << property.settings_identifier);
        ui->settings_tree->addTopLevelItem(_item);
        uint8_t property_offset = 0, property_counter = 0;
        while(property_offset < property.size) {
            auto _subitem = new QTreeWidgetItem(QStringList() << serialized_setting_names[properties_counter + property_counter].identifier);
            property_offset += serialized_setting_names[properties_counter + property_counter].setting_size;
            property.settings.push_back(&serialized_setting_names[properties_counter + property_counter]);
            _item->addChild(_subitem);

            ++property_counter;
        }
        properties_counter += property_counter;
    }

    // Expand by default
    ui->settings_tree->expandAll();
}

const QString table_default_data[5][6] = {
    {"0x24", "0", "0", "0", "0", "0"},
    {"0", "0", "0", "0", "0", "0"},
    {"0", "0", "0", "0", "0", "0"},
    {"0", "0", "0", "0", "0", "0"},
    {"0", "0", "0x25", "", "", ""},
};

void FluorescenceSimulator::fill_com_table()
{
    bool blocked = signalsBlocked();
    blockSignals(true);
    Q_FOREACH(QWidget* w, findChildren<QWidget*>()) w->blockSignals(true);

    read_in_protocol(":/protocol.csv"); // From resources

    QTableWidget *com_data = ui->com_data;
    com_data->setRowCount(5);
    com_data->setColumnCount(6);

    auto max_table_height = com_data->height() - com_data->horizontalHeader()->height() - 1;
    auto max_table_width = com_data->width() - com_data->verticalHeader()->width() - 2;

    for(auto i = 0; i < com_data->rowCount(); ++i) {
        com_data->setRowHeight(i, max_table_height / com_data->rowCount());
        // Fill table with default content
        for(auto j = 0; j < com_data->columnCount(); ++j) com_data->setItem(i, j, new QTableWidgetItem(""));
    }
    for(auto i = 0; i < com_data->columnCount(); ++i) com_data->setColumnWidth(i, max_table_width / com_data->columnCount());
    auto protocol_row = protocol_file.at(PROTOCOL_DATA_OFFSET_ROWS);
    fill_com_table_items(PROTOCOL_DATA_OFFSET_ROWS);
    ui->com_command_val->setText("[" + protocol_row[1] + "]: " + protocol_row[0] + ", " + (protocol_row.length() == 30 ? protocol_row[29] : "no parameter description"));

    // Disable start & stop bit editable as well as the excess cells
    com_data->item(0, 0)->setFlags(Qt::ItemFlag::ItemIsEditable);
    for(auto i = 0; i < 4; ++i) com_data->item(4, 2 + i)->setFlags(Qt::ItemIsEditable);

    blockSignals(blocked);
    Q_FOREACH(QWidget* w, findChildren<QWidget*>()) w->blockSignals(false);
}

void FluorescenceSimulator::fill_com_table_items(int row_of_access)
{
    bool blocked = signalsBlocked();
    blockSignals(true);
    Q_FOREACH(QWidget* w, findChildren<QWidget*>()) w->blockSignals(true);

    QTableWidget *com_data = ui->com_data;
    for(auto i = 0; i < com_data->rowCount(); ++i) {
        for(auto j = 0; j < com_data->columnCount(); ++j) {
            auto access_point = i * com_data->columnCount() + j;
            QTableWidgetItem *_item = com_data->item(i, j);
            QString _data = (access_point < PROTOCOL_DATA_LENGTH) ? protocol_file.at(row_of_access)[PROTOCOL_DATA_OFFSET_COLS + access_point] : "";
            _item->setText(_data);
            if(_data.compare("reserved 0") == 0) _item->setFlags(_item->flags() & ~Qt::ItemIsEditable);
            else _item->setFlags(_item->flags() | Qt::ItemIsEditable);
        }
    }


    blockSignals(blocked);
    Q_FOREACH(QWidget* w, findChildren<QWidget*>()) w->blockSignals(false);
}

void FluorescenceSimulator::error_message(QString message)
{
    QMessageBox err_zero;
    err_zero.setText(message);
    err_zero.setIcon(QMessageBox::Critical);
    err_zero.exec();
}

void FluorescenceSimulator::read_in_protocol(QString file_name)
{
    QFile csv_file(file_name);
    if(!csv_file.open(QIODevice::ReadOnly)) exit(-1);
    QTextStream csv_read_in = QTextStream(&csv_file);

    while(!csv_read_in.atEnd()) {
        // Read in every line into vector
        QStringList str_list;
        str_list << csv_read_in.readLine().split(",", QString::SkipEmptyParts);
        // qDebug() << "reading:" << str_list;
        // qDebug() << str_list.length();
        protocol_file.push_back(str_list);
    }
    // Erase header
}


void FluorescenceSimulator::on_sim_start_clicked()
{
    if(ui->sim_start->text() == "Start") {
        ui->sim_start->setText("Stop");

        if(!init_once_flag) {
            clk_div = ui->sim_clk_div->value();
            vfdco_clock_initializer();
            fill_settings_tree();
            fill_com_table();
            init_once_flag = true;
        }

        global_timer->start(1);

        ui->widget_controls->setEnabled(true);
        ui->sim_clk_div->setEnabled(false);
        ui->sim_reset->setEnabled(false);
    } else {

        ui->sim_start->setText("Start");
        global_timer->stop();

        ui->widget_controls->setEnabled(false);
        ui->sim_clk_div->setEnabled(true);
        ui->sim_reset->setEnabled(true);
    }
}

void FluorescenceSimulator::on_button_f1_clicked() { _vfdco_hid_button_set(150, BUTTON_F1); }
void FluorescenceSimulator::on_button_f2_clicked() { _vfdco_hid_button_set(150, BUTTON_F2); }
void FluorescenceSimulator::on_button_f3_clicked() { _vfdco_hid_button_set(150, BUTTON_F3); }
void FluorescenceSimulator::on_button_f4_clicked() { _vfdco_hid_button_set(150, BUTTON_F4); }
void FluorescenceSimulator::on_button_f1Var_clicked() { _vfdco_hid_button_set(800, BUTTON_F1); }
void FluorescenceSimulator::on_button_f2Var_clicked() { _vfdco_hid_button_set(800, BUTTON_F2); }
void FluorescenceSimulator::on_button_f3Var_clicked() { _vfdco_hid_button_set(800, BUTTON_F3); }
void FluorescenceSimulator::on_button_f4Var_clicked() { _vfdco_hid_button_set(800, BUTTON_F4); }

void FluorescenceSimulator::on_sim_reset_clicked()
{
    init_once_flag = false;
    ui->settings_tree->clear();
    ui->com_data->clear();
}

void FluorescenceSimulator::on_com_data_cellChanged(int row, int column)
{
    if (!signalsBlocked()) {
        QString item_text = ui->com_data->item(row, column)->text();

        // Check validity
        bool valid;
        item_text.toUShort(&valid, 0);
        if(!valid) {
            error_message("The entered value is likely not a valid number. Please try again.");
            return;
        }

        if(row == 0 && column == 1) {
            auto element = std::find_if(protocol_file.begin(), protocol_file.end(), [&item_text] (const QStringList& list) {
                return QString::compare((list.length() > PROTOCOL_DATA_LENGTH) ? list[PROTOCOL_DATA_OFFSET_COLS + PROTOCOL_DATA_COMMAND_OFFSET] : QString(""), item_text) == 0;
            });
            if (element != protocol_file.end()) {
                // qDebug() << "found!" + QString::number(element - protocol_file.begin());
                int vec_idx = element - protocol_file.begin();
                fill_com_table_items(vec_idx);
                auto protocol_row = protocol_file.at(vec_idx);
                ui->com_command_val->setText("[" + protocol_row[1] + "]: " + protocol_row[0] + ", " + (protocol_row.length() == 30 ? protocol_row[29] : "no parameter description"));
                current_protocol_row = vec_idx;
            } else {
                error_message("Oh deer 🦌, this is not a valid command. Please try again.");
            }
        }
    }
}

void FluorescenceSimulator::on_com_data_cellPressed(int row, int column)
{
    if(!signalsBlocked()) {
        QString item_text = ui->com_data->item(row, column)->text();
        ui->com_param_val->setText(item_text);
        if(item_text.compare("reserved 0") == 0) ui->com_param_val->setStyleSheet("color:#FF0000;");
        else ui->com_param_val->setStyleSheet("");
    }
}

void FluorescenceSimulator::on_com_send_clicked()
{
    QTableWidget *com_data = ui->com_data;
    std::vector<uint8_t> com_parsed_data;
    for(auto i = 0; i < com_data->rowCount(); ++i) {
        for(auto j = 0; j < com_data->columnCount(); ++j) {
            if(!(i * com_data->columnCount() + j < PROTOCOL_DATA_LENGTH)) goto label_complete; // https://xkcd.com/292/
            // Parse table data
            QString item_text = com_data->item(i, j)->text();
            if(item_text.compare("reserved 0") == 0) {
                com_parsed_data.push_back(0);
            } else {
                bool valid;
                uint8_t _hex_val = item_text.toUShort(&valid, 0);
                if(!valid) {
                    error_message("There was an error in the protocol. Please check the format and try again!");
                    return;
                }
                else com_parsed_data.push_back(_hex_val);
            }
        }
    }

    label_complete:
    if(com_parsed_data.size() != PROTOCOL_DATA_LENGTH) { // Check length
        error_message("There was an error in the protocol. Please check the format length and try again!");
    } else {
        uint8_t protocol_raw[PROTOCOL_DATA_LENGTH] = {0};
        for(uint_fast8_t i = 0; i < PROTOCOL_DATA_LENGTH; ++i) protocol_raw[i] = com_parsed_data[i];
    }
    // for(auto i : com_parsed_data) qDebug() << QString("%1").arg(i, 3, 16, QChar('0'));
    return;
}