#include "fluorescenceapp.h"
#include "ui_fluorescenceapp.h"
#include "fwupdatestm.h"
#include "ui_fwupdatestm.h"

#include <QFileDialog>
#include <QLayout>

const QString fw_suffix_stm = "s";
const QString fw_suffix_avr = "a";
const QString fw_extension = ".flupdate";

const QString fw_text_1 = "Please read carefully before you proceed.\nThe Firmware Updater will now put the device, Fluorescence, into DFU (device firmware update) mode. We will then attempt to call a third-party tool called ";
const QString fw_text_2_stm = "dfu-util";
const QString fw_text_2_avr = "avrdude";
const QString fw_text_3 = " that helps flashing the new firmware to your device.\nWhen clicking <Update>, be aware these steps will be performed to flash the new firmware to your device:\n";
const QString fw_text_4_stm = "(1) Fluorescence is put into DFU mode. The clock display will show ""dFU"".\n\
(2) The flashing process starts, when a console window ""dfu-util"" appears. While dfu-util is running, YOUR DEVICE NEEDS TO BE CONNECTED TO POWER AND TO YOUR COMPUTER, OR ELSE YOU WILL BRICK IT. Once the flashing process is done, dfu-util will close automatically.\n\
(3) Meanwhile, Fluorescence App will restart.\n\
(4) To restart the device, disconnect it from power and reconnect.";
const QString fw_text_4_avr = "(1) Fluorescence is put into DFU mode. The clock display will show ""dFU"".\n\
(2) The flashing process starts, when a console window ""avrdude"" appears. While avrdude is running, YOUR DEVICE NEEDS TO BE CONNECTED TO POWER AND TO YOUR COMPUTER, OR ELSE YOU WILL BRICK IT. Once the flashing process is done, avrdude will close automatically.\n\
(3) Meanwhile, Fluorescence App will restart.\n\
(4) The will restart automatically. In case it does not restart automatically, either press the reset button on the device, or disconnect and reconnect it from power.";
const QString fw_text_5 = "By ticking this box I confirm that\n\
(1) I have read through the information above carefully\n\
(2) I have acknowledged that The VFD Collective DOES NOT\n\
assume any liability for possible damages done to the device\n\
during the firmware update.\n";

FWUpdateSTM::FWUpdateSTM(QWidget *parent, fw_updater_t type) :
    QDialog(parent),
    ui(new Ui::FWUpdateSTM),
    fw_updater_type(type)
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
    QString file_path = QFileDialog::getOpenFileName(this, "Find Firmware Update File", QDir::homePath(), "Fluorescence Update File (*.flupdate)");
    FluorescenceApp* fl_app_inst = qobject_cast<FluorescenceApp*>(parent());

    // Validation
    QFileInfo f_info(file_path);
    QString firmware_name = f_info.fileName();
    if(firmware_name.isEmpty()) {
        fl_app_inst->error_message("Please select a file.", QMessageBox::Icon::Warning);
        return;
    }
    if(!(firmware_name.mid(firmware_name.length() - fw_extension.length(), fw_extension.length()).compare(fw_extension, Qt::CaseInsensitive) == 0)) {
        fl_app_inst->error_message("Oh no. It seems like this is not a firmware file", QMessageBox::Icon::Warning);
        return;
    }
    if(fw_updater_type == FIRMWARE_UPDATE_STM) {
        if(!(firmware_name.contains(fw_suffix_stm))) {
            fl_app_inst->error_message("Oh no. It seems like you have downloaded the wrong firmware file. Please check your clock info and download the matching firmware file.", QMessageBox::Icon::Warning);
            return;
        }
    } else {
        if(!(firmware_name.contains(fw_suffix_avr))) {
            fl_app_inst->error_message("Oh no. It seems like you have downloaded the wrong firmware file. Please check your clock info and download the matching firmware file.", QMessageBox::Icon::Warning);
            return;
        }
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

void FWUpdateSTM::on_updater_run_clicked()
{
    // FluorescenceApp* fl_app_inst = qobject_cast<FluorescenceApp*>(parent());
    // fl_app_inst->global_com_instance->transfer_dfu_request();
    if(fw_updater_type == FIRMWARE_UPDATE_STM) {
        ui->updater_info_text->setPlainText(fw_text_1 + fw_text_2_stm + fw_text_3 + fw_text_4_stm);
    } else {
        ui->updater_info_text->setPlainText(fw_text_1 + fw_text_2_avr + fw_text_3 + fw_text_4_avr);
    }
    ui->updater_agree_text->setText(fw_text_5);

    ui->updater->setFixedSize(801, ui->updater->size().height());
    setFixedSize(801, this->size().height());
}

void FWUpdateSTM::on_updater_agree_stateChanged(int arg1)
{
    (void)arg1;
    if(ui->updater_agree->isChecked()) {
        ui->updater_update->setEnabled(true);
    } else {
        ui->updater_update->setEnabled(false);
    }
}

void FWUpdateSTM::on_updater_update_clicked()
{

}
