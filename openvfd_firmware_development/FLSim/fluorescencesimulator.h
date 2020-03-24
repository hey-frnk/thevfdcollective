#ifndef FLUORESCENCESIMULATOR_H
#define FLUORESCENCESIMULATOR_H

#include <QMainWindow>
#include <QTimer>
#include <QAbstractItemModel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class FluorescenceSimulator; }
QT_END_NAMESPACE

class FluorescenceSimulator : public QMainWindow
{
    Q_OBJECT

public:
    FluorescenceSimulator(QWidget *parent = nullptr);
    ~FluorescenceSimulator();
    virtual void update();

private slots:
    void on_sim_start_clicked();

    void on_button_f1_clicked();

    void on_button_f2_clicked();

    void on_button_f3_clicked();

    void on_button_f4_clicked();

    void on_button_f1Var_clicked();

    void on_button_f2Var_clicked();

    void on_button_f3Var_clicked();

    void on_button_f4Var_clicked();

    void on_sim_reset_clicked();

    void on_com_data_cellChanged(int row, int column);

    void on_com_data_cellPressed(int row, int column);

    void on_com_send_clicked();

    void on_power_dim_visible_stateChanged(int arg1);

private:
    QTimer *global_timer = nullptr;
    bool init_once_flag = false;

    QAbstractItemModel *model;
    std::vector<QStringList> protocol_file;
    int current_protocol_row = -1;

    Ui::FluorescenceSimulator *ui;
    void link_ui_instances();
    void fill_settings_tree();
    void fill_com_table();
    void fill_com_table_items(int row_of_access);
    void error_message(QString message);
    void read_in_protocol(QString file_name);
};
#endif // FLUORESCENCESIMULATOR_H
