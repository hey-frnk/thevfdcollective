#include "fluorescenceapp.h"
#include "ui_fluorescenceapp.h"
#include "fwupdate.h"
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
(2) The flashing process starts, when ""dfu-util"" appears. While dfu-util is running, YOUR DEVICE NEEDS TO BE CONNECTED TO POWER AND TO YOUR COMPUTER, OR ELSE YOU WILL BRICK IT. DO NOT UNPLUG EITHER. Once the flashing process is done, dfu-util will notify you and you can close the updater.\n\
(3) Meanwhile, Fluorescence App disconnects from Fluorescence.\n\
(4) If the device doesn't restart automatically, press the round shaped restart button in the back.";
const QString fw_text_4_avr = "(1) Fluorescence is put into DFU mode. The clock display will show ""dFU"".\n\
(2) The flashing process starts, when ""avrdude"" appears. While avrdude is running, YOUR DEVICE NEEDS TO BE CONNECTED TO POWER AND TO YOUR COMPUTER, OR ELSE YOU WILL BRICK IT. DO NOT UNPLUG EITHER. Once the flashing process is done, avrdude will notify you and you can close the updater.\n\
(3) Meanwhile, Fluorescence App disconnects from Fluorescence.\n\
(4) If the device doesn't restart automatically, press the round shaped restart button in the back.";
const QString fw_text_5 = "By ticking this box I confirm that\n\
(1) I have read through the information above carefully\n\
(2) I have acknowledged that The VFD Collective DOES NOT\n\
assume any liability for possible damages done to the device\n\
during the firmware update.\n";

FWUpdate::FWUpdate(QWidget *parent, QString fw_string, QString port_name) :
    QDialog(parent),
    ui(new Ui::FWUpdate),
    fw_string(fw_string),
    port_name(port_name)
{
    ui->setupUi(this);

    ui->updater->hide();
    ui->welcome->show();
}

FWUpdate::~FWUpdate()
{
    delete ui;
}

void FWUpdate::on_welcome_next_clicked()
{
    // Open File
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    // # just list mode is quite sufficient for choosing a diectory
    fileDialog.setViewMode(QFileDialog::List);
    // # only want to to show directories
    fileDialog.setOption(QFileDialog::ShowDirsOnly);
    // # native dialog, at least under Ubuntu GNOME is a bit naff for choosing a directory
    // # (shows files but greyed out), so going for Qt's own cross-plaform chooser
    fileDialog.setOption(QFileDialog::DontUseNativeDialog);
    // # get rid of (or at least grey out) file-types selector
    fileDialog.setOption(QFileDialog::HideNameFilterDetails);
    // # DontResolveSymlinks seemingly recommended by http://doc.qt.io/qt-5/qfiledialog.html#getExistingDirectory
    // # but I found it didn't make any difference (symlinks resolved anyway)
    fileDialog.setOption(QFileDialog::DontResolveSymlinks);
    if(fileDialog.exec()) {
        QString updater_directory_path = fileDialog.selectedFiles().at(0);
        qDebug() << updater_directory_path;
        FluorescenceApp* fl_app_inst = qobject_cast<FluorescenceApp*>(parent());

        // Updater directory contains fl_updater_info.flinfo, x_yy.flupdate, Platform dependent exectuable (Win + Mac)
        // 1. Open fl_updater_info.flinfo, read all text retrieves file name of firmware file. 1st: firmware name, 2nd: updater executable name
        // 2. Validate retrieved file name with hardware version

        // Validation
        QString f_info_str = updater_directory_path + QDir::separator();
        QFileInfo f_info;

        f_info = QFileInfo(f_info_str + "fl_updater_info.flinfo");
        if(!f_info.exists()) {
            fl_app_inst->error_message("The firmware update folder is corrupt: One or more files are missing. Make sure the update folder is downloaded from TheVFDCollective.com.", QMessageBox::Icon::Warning);
            return;
        }
        fw_updater_path_detected = f_info_str;

        // Get Firmware file name
        QFile fl_updater_info(f_info.filePath());
        fl_updater_info.open(QIODevice::ReadOnly);
        //Open file for reading
        QStringList fl_updater_info_content;

        while(!fl_updater_info.atEnd())
        fl_updater_info_content.append(fl_updater_info.readLine().trimmed());

        qDebug() << fl_updater_info_content;

        QFileInfo fl_updater_file_info = QFileInfo(f_info_str + fl_updater_info_content.at(0));
        QString fl_updater_file_name = fl_updater_file_info.fileName();
        qDebug() << fl_updater_file_name;

        // Files exist?
        // if(!fl_updater_file_info.exists()) { fl_app_inst->error_message("The firmware update folder is corrupt: The firmware file is missing. Make sure the update folder is downloaded from TheVFDCollective.com.", QMessageBox::Icon::Warning); return; }
        for(auto s : fl_updater_info_content) {
            qDebug() << "Checking: " << f_info_str + s;
            if(!QFileInfo::exists(f_info_str + s)) {
                fl_app_inst->error_message("The firmware update folder is corrupt: The firmware file or the updater application is missing. Make sure the update folder is downloaded from TheVFDCollective.com, and unmodified.", QMessageBox::Icon::Warning);
                return;
            }
        }

        fw_updater_firmware_file_detected = fl_updater_file_info.absoluteFilePath();

        // Check extension, detect firmware. Replace _ with .
        QString fl_updater_file_display = QString(fl_updater_file_name);
        fl_updater_file_display.replace("_", ".");
        fl_updater_file_display.remove(fw_extension, Qt::CaseInsensitive);

        qDebug() << fl_updater_file_display;
        // Detect firmware suffix
        if(fw_string.contains(fw_suffix_stm)) {
            // STM
            qDebug() << "STM firmware suffix check";
            fw_updater_type_detected = FIRMWARE_UPDATE_STM;
            if(!(fl_updater_file_display.contains(fw_suffix_stm))) {
                fl_app_inst->error_message("Oh no. It seems like you have downloaded the wrong firmware file. You need to download the firmware for STM32! Please download the matching firmware and try again.", QMessageBox::Icon::Warning);
                return;
            }
        } else if(fw_string.contains(fw_suffix_avr)) {
            // AVR
            qDebug() << "AVR firmware suffix check";
            fw_updater_type_detected = FIRMWARE_UPDATE_AVR;
            if(!(fl_updater_file_display.contains(fw_suffix_avr))) {
                fl_app_inst->error_message("Oh no. It seems like you have downloaded the wrong firmware file. You need to download the firmware for AVR! Please download the matching firmware and try again.", QMessageBox::Icon::Warning);
                return;
            }
        }

        // Check extension
        if(!(fl_updater_file_name.mid(fl_updater_file_name.length() - fw_extension.length(), fw_extension.length()).compare(fw_extension, Qt::CaseInsensitive) == 0)) {
            fl_app_inst->error_message("Oh no. It seems like this is not a firmware file", QMessageBox::Icon::Warning);
            return;
        }

        ui->updater_new->setText(fl_updater_file_display);
        ui->updater_curr->setText(fw_string);

        ui->welcome->hide();
        ui->updater->show();
    }
}

void FWUpdate::on_updater_run_clicked()
{
    // FluorescenceApp* fl_app_inst = qobject_cast<FluorescenceApp*>(parent());
    // fl_app_inst->global_com_instance->transfer_dfu_request();
    if(fw_updater_type_detected == FIRMWARE_UPDATE_STM) {
        ui->updater_info_text->setPlainText(fw_text_1 + fw_text_2_stm + fw_text_3 + fw_text_4_stm);
    } else {
        ui->updater_info_text->setPlainText(fw_text_1 + fw_text_2_avr + fw_text_3 + fw_text_4_avr);
    }
    ui->updater_agree_text->setText(fw_text_5);

    ui->updater->setFixedSize(801, ui->updater->size().height());
    setFixedSize(801, this->size().height());
}

void FWUpdate::on_updater_agree_stateChanged(int arg1)
{
    (void)arg1;
    if(ui->updater_agree->isChecked()) {
        ui->updater_update->setEnabled(true);
    } else {
        ui->updater_update->setEnabled(false);
    }
}

void FWUpdate::on_updater_update_clicked()
{
    if(ui->updater_update->text() == "Close") {
        this->close();
        return;
    }

    FluorescenceApp* fl_app_inst = qobject_cast<FluorescenceApp*>(parent());
    if(!ui->updater_agree->isChecked()) {
        fl_app_inst->error_message("Please agree to the terms and conditions of updating the firmware!", QMessageBox::Icon::Warning);
        return;
    } else {
        ui->updater_update->setEnabled(false);
        ui->updater_run->setEnabled(false);

        QString updater_command;

        if(fw_updater_type_detected == FIRMWARE_UPDATE_STM) {
            // fl_app_inst->error_message("Hi i'm dfu-util", QMessageBox::Icon::Warning);
            QString updater_command_path;

            #ifdef Q_OS_MACOS // macOS specific code
            updater_command_path = fw_updater_path_detected + "dfu_util_macos" + QDir::separator() + "dfu-util";
            #endif
            #ifdef Q_OS_WIN64 // Windows specific code
            updater_command_path = fw_updater_path_detected + "dfu_util_macos" + QDir::separator() + "dfu-util";
            #endif

            updater_command = updater_command_path + " -a 0 -s 0x08000000:leave -D \"" + fw_updater_firmware_file_detected + "\" -v -v -t 2048";

            fl_app_inst->fw_update_manual_dfu_request();
        } else if(fw_updater_type_detected == FIRMWARE_UPDATE_AVR){
            QString updater_command_path;
            QString updater_command_path_conf;
            QString updater_command_port_name;

            #ifdef Q_OS_MACOS // macOS specific code
            updater_command_path = fw_updater_path_detected + "avrdude_macos" + QDir::separator() + "avrdude";
            updater_command_path_conf = "\"" + fw_updater_path_detected + "avrdude_macos" + QDir::separator() + "avrdude.conf" + "\"";
            updater_command_port_name = "\"/dev/" + port_name + "\"";
            #endif
            #ifdef Q_OS_WIN64 // Windows specific code
            updater_command_path = fw_updater_path_detected + "avrdude_win64" + QDir::separator() + "avrdude.exe";
            updater_command_path_conf = "\"" + fw_updater_path_detected + "avrdude_win64" + QDir::separator() + "avrdude.conf" + "\"";
            updater_command_port_name = "\"" + port_name + "\"";
            #endif

            updater_command = updater_command_path + " -C " + updater_command_path_conf + " -v -p atmega328p -c arduino -P " + updater_command_port_name + " -b115200 -D -U flash:w:\"" + fw_updater_firmware_file_detected + "\":i -Ulfuse:v:0x00:m";
        } else {
            return;
        }

        // Updater command compiled
        qDebug().noquote() << updater_command;

        // Start flashing!
        fl_app_inst->fw_update_manual_disconnect();
        QThread::msleep(500);

        execute_updater(updater_command, fw_updater_type_detected);
    }
}

void FWUpdate::ReadOut()
{
    QProcess *p = dynamic_cast<QProcess *>(sender());
    if(p) ui->updater_info_text->appendPlainText(p->readAllStandardOutput());
}
void FWUpdate::ReadErr()
{
    QProcess *p = dynamic_cast<QProcess *>(sender());
    if(p) ui->updater_info_text->appendPlainText(p->readAllStandardError());
}

void FWUpdate::UpdateFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitStatus);
    QString filename = fw_updater_path_detected + QDir::separator() + QDateTime::currentDateTime().toString() + "_update.log";
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << ui->updater_info_text->toPlainText() << endl;
    }

    if(exitCode == 0) {
        ui->updater_info_text->appendPlainText("Update finished. Click 'finish' to close the updater!");
    } else {
        ui->updater_info_text->appendPlainText("Updater reported a code: (" + QString("%1").arg(exitCode) + "). Click 'finish' to close the updater!");
    }
    ui->updater_update->setText("Close");
    ui->updater_update->setEnabled(true);
}

void FWUpdate::execute_updater(QString command, fw_updater_t updater_type)
{
    if(updater_type == FIRMWARE_UPDATE_UNDETERMINED) return;

    QProcess *p = new QProcess(this);
    if(p) {
      p->setEnvironment(QProcess::systemEnvironment());
      p->setProcessChannelMode(QProcess::MergedChannels);

      p->start(command);
      p->waitForStarted();

      if(updater_type == FIRMWARE_UPDATE_AVR) ui->updater_info_text->setPlainText(fw_text_2_avr + " is updating Fluorescence\n");
      else if(updater_type == FIRMWARE_UPDATE_STM) ui->updater_info_text->setPlainText(fw_text_2_stm + " is updating Fluorescence\n");
      ui->updater_update->setText("Updating...");

      connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadOut()));
      connect(p, SIGNAL(readyReadStandardError()), this, SLOT(ReadErr()));
      connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(UpdateFinished(int, QProcess::ExitStatus)));
    }
}
