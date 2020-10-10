#ifndef FWUPDATESTM_H
#define FWUPDATESTM_H

#include <QDialog>
#include <QProcess>

typedef enum {
  FIRMWARE_UPDATE_STM,
  FIRMWARE_UPDATE_AVR
} fw_updater_t;

namespace Ui {
class FWUpdate;
}

class FWUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit FWUpdate(QWidget *parent = nullptr, QString fw_string = QString(), QString port_name = QString());
    ~FWUpdate();

signals:
    void readyReadStandardOutput();
    void readyReadStandardError();

private slots:
    void on_welcome_next_clicked();

    void on_updater_run_clicked();

    void on_updater_agree_stateChanged(int arg1);

    void on_updater_update_clicked();

    void ReadOut();
    void ReadErr();
    void UpdateFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Ui::FWUpdate *ui;
    fw_updater_t fw_updater_type_detected;
    QString fw_updater_path_detected, fw_updater_firmware_file_detected;

    QString fw_string, port_name;
};

#endif // FWUPDATESTM_H
