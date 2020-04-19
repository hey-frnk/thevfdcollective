#include "fluorescenceapp.h"
#include "ui_fluorescenceapp.h"
#include "fwupdatestm.h"
#include "ui_fwupdatestm.h"

#include <QFileDialog>

const QString fw_suffix = "s";
const QString fw_extension = ".hex";

FWUpdateSTM::FWUpdateSTM(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FWUpdateSTM)
{
    ui->setupUi(this);

    ui->updater->hide();
    ui->welcome->show();
}

FWUpdateSTM::~FWUpdateSTM()
{
    delete ui;
}

void FWUpdateSTM::on_welcome_next_clicked()
{
    // Open File
    QString file_path = QFileDialog::getOpenFileName(this, "Find Firmware Update File", QDir::homePath(), "Firmware HEX-File (*.hex)");
    FluorescenceApp* fl_app_inst = qobject_cast<FluorescenceApp*>(parent());

    // Validation
    QFileInfo f_info(file_path);
    QString firmware_name = f_info.fileName();
    if(firmware_name.isEmpty()) {
        fl_app_inst->error_message("Please select a file.", QMessageBox::Icon::Warning);
        return;
    }
    if(!(firmware_name.mid(firmware_name.length() - 4, 4).compare(fw_extension, Qt::CaseInsensitive) == 0)) {
        fl_app_inst->error_message("Oh no. It seems like this is not a firmware file", QMessageBox::Icon::Warning);
        return;
    }
    if(!(firmware_name.contains("s"))) {
        fl_app_inst->error_message("Oh no. It seems like you have downloaded the wrong firmware file. Please check your clock info and download the matching firmware file.", QMessageBox::Icon::Warning);
        return;
    }

    // Replace _ with .
    firmware_name.replace("_", ".");
    firmware_name.remove(fw_extension, Qt::CaseInsensitive);

    firmware_file_path = file_path;

    // ui->updater_curr->setText(fl_app_inst->ui->settings_info_fw->text());
    ui->updater_new->setText(firmware_name);

    ui->welcome->hide();
    ui->updater->show();
}



void FWUpdateSTM::on_updater_geekmode_stateChanged(int arg1)
{
    (void)arg1;
    if(ui->updater_geekmode->isChecked()) {
        ui->updater->setFixedSize(751, ui->updater->size().height());
        setFixedSize(751, this->size().height());
    } else {
        ui->updater->setFixedSize(321, ui->updater->size().height());
        setFixedSize(321, this->size().height());
    }
}

void FWUpdateSTM::on_updater_run_clicked()
{
    FluorescenceApp* fl_app_inst = qobject_cast<FluorescenceApp*>(parent());
    fl_app_inst->global_com_instance->transfer_dfu_request();
}
