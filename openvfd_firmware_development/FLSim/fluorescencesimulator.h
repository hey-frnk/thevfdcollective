#ifndef FLUORESCENCESIMULATOR_H
#define FLUORESCENCESIMULATOR_H

#include <QMainWindow>
#include <QTimer>
#include <QAbstractItemModel>
#include <QTreeWidget>
#include <QTreeWidgetItem>

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

private:
    QTimer *global_timer = nullptr;
    bool init_once_flag = false;

    QAbstractItemModel *model;

    Ui::FluorescenceSimulator *ui;
    void link_ui_instances();
    void fill_settings_tree();
};
#endif // FLUORESCENCESIMULATOR_H
