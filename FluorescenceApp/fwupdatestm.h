#ifndef FWUPDATESTM_H
#define FWUPDATESTM_H

#include <QDialog>

typedef enum {
  FIRMWARE_UPDATE_STM,
  FIRMWARE_UPDATE_AVR
} fw_updater_t;

namespace Ui {
class FWUpdateSTM;
}

class FWUpdateSTM : public QDialog
{
    Q_OBJECT

public:
    explicit FWUpdateSTM(QWidget *parent = nullptr, fw_updater_t type = FIRMWARE_UPDATE_STM);
    ~FWUpdateSTM();

private slots:
    void on_welcome_next_clicked();

    void on_updater_run_clicked();

    void on_updater_agree_stateChanged(int arg1);

    void on_updater_update_clicked();

private:
    Ui::FWUpdateSTM *ui;
    fw_updater_t fw_updater_type;
    QString firmware_file_path;
};

#endif // FWUPDATESTM_H
