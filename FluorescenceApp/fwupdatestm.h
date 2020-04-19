#ifndef FWUPDATESTM_H
#define FWUPDATESTM_H

#include <QDialog>

namespace Ui {
class FWUpdateSTM;
}

class FWUpdateSTM : public QDialog
{
    Q_OBJECT

public:
    explicit FWUpdateSTM(QWidget *parent = nullptr);
    ~FWUpdateSTM();

private slots:
    void on_welcome_next_clicked();

    void on_updater_geekmode_stateChanged(int arg1);

    void on_updater_run_clicked();

private:
    Ui::FWUpdateSTM *ui;

    QString firmware_file_path;
};

#endif // FWUPDATESTM_H
