#include "fl_app_com.h"
#include "fluorescenceapp.h"

#include "src/fl_app_lights.h"
#include "../FluorescenceV3/Commons/vfdco_config.h"

#include <QTranslator>

fl_app_com::fl_app_com() :
    is_bluetooth_communication(true),
    incoming_characteristic(nullptr),
    controller(nullptr),
    localDevice(new QBluetoothLocalDevice),
    discoveryAgent(nullptr),
    currentDevice(nullptr),
    currentService(nullptr),
    currentCharacteristic(nullptr)
{
    buf_rx = new uint8_t[BUF_RX_SIZE];

    // Check if Bluetooth is available on this device
    if (localDevice->isValid()) {

        qDebug() << "Bluetooth is available on this device";

        //Turn BT on, make it visible to others
        localDevice->powerOn();
        localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);

        //Read local device name & address
        qDebug() << "Local Device:" << localDevice->name() << "(" << "Address:" << localDevice->address() << ")";

        // Create a discovery agent and connect to its signals
        if(discoveryAgent) delete discoveryAgent;
        discoveryAgent = new QBluetoothDeviceDiscoveryAgent;
        connect(discoveryAgent, SIGNAL(finished()), this, SLOT(bt_discovery_finished()));

        // Start a discovery
        discoveryAgent->setLowEnergyDiscoveryTimeout(5000);
        discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
        qDebug() << "Device discover started";
    }
    else
    {
        qDebug() << "Bluetooth is not available on this device";
    }

    // Callback will determine legacy
}

fl_app_com::fl_app_com(const QString portname) :
    is_bluetooth_communication(false),
    incoming_characteristic(nullptr),
    controller(nullptr),
    localDevice(nullptr),
    discoveryAgent(nullptr),
    currentDevice(nullptr),
    currentService(nullptr),
    currentCharacteristic(nullptr)
{
    buf_rx = new uint8_t[BUF_RX_SIZE];

    qDebug() << portname;
    serial_port.close();
    serial_port.setPortName(portname);
    serial_port.setBaudRate(QSerialPort::Baud115200);
    serial_port.setDataBits(QSerialPort::Data8);
    serial_port.setParity(QSerialPort::NoParity);
    serial_port.setStopBits(QSerialPort::OneStop);
    serial_port.setFlowControl(QSerialPort::NoFlowControl);
    if(!serial_port.open(QIODevice::ReadWrite)){
        qDebug() << serial_port.error();
        return;
    }
    qDebug() << serial_port.error();

    // If WCH CH340G, clock will reboot. Wait appropriately
    if(portname.contains("wch")) QThread::msleep(8000);

    // Try if port is legacy
    determine_legacy();
}

fl_app_com::~fl_app_com() {
    if(is_bluetooth_communication) {
        controller->disconnectFromDevice();
        delete currentCharacteristic;
        currentCharacteristic = nullptr;
        delete currentService;
        currentService = nullptr;
        delete currentDevice;
        currentDevice = nullptr;
        serviceList.clear();
    }
    else serial_port.close();
    delete[] buf_rx;
    delete[] buf_tx;
}

void fl_app_com::transfer_serial0(uint8_t *clr_arr)
{
    transfer_serial(clr_arr, 0);
}

void fl_app_com::transfer_serial1(uint8_t *clr_arr)
{
    transfer_serial(clr_arr, 1);
}

void fl_app_com::transfer_light_pattern(uint8_t instance, uint8_t param0, uint8_t param1)
{
    clear_buffer();
    if(!is_legacy_protocol) {
        set_command_byte(0x04);
        buf_tx[COM_PROTOCOL_CONTROL_OFFSET] = instance;
        buf_tx[COM_PROTOCOL_PARAM0_OFFSET] = param0;
        buf_tx[COM_PROTOCOL_PARAM1_OFFSET] = param1;
    } else {
        set_command_byte(0x20);
        buf_tx[COM_PROTOCOL_CONTROL_OFFSET] = instance;
        buf_tx[COM_PROTOCOL_PARAM0_OFFSET] = param0;
    }
    transfer();
}

void fl_app_com::transfer_enable_presets(uint8_t enabled_instances)
{
    clear_buffer();
    if(!is_legacy_protocol) {
        set_command_byte(0x05);
        buf_tx[COM_PROTOCOL_DATA_OFFSET] = enabled_instances;
    }
    transfer();
}

void fl_app_com::transfer_random_set(uint8_t enabled_instances, random_speed_t speed)
{
    clear_buffer();
    if(!is_legacy_protocol) {
        set_command_byte(0x06);
        buf_tx[COM_PROTOCOL_DATA_OFFSET] = enabled_instances;
        buf_tx[COM_PROTOCOL_DATA_OFFSET + 1] = speed;
    }
    transfer();
}

void fl_app_com::transfer_gui_set(uint8_t instance, uint8_t param0)
{
    clear_buffer();
    set_command_byte(0x10);
    buf_tx[COM_PROTOCOL_CONTROL_OFFSET] = instance;
    buf_tx[COM_PROTOCOL_PARAM0_OFFSET] = param0;
    transfer();
}

void fl_app_com::legacy_transfer_clock_flags(uint8_t clock_flag, uint8_t clock_flag_value)
{
    clear_buffer();
    set_command_byte(0x12);
    buf_tx[COM_PROTOCOL_CONTROL_OFFSET] = clock_flag;
    buf_tx[COM_PROTOCOL_PARAM0_OFFSET] = clock_flag_value;
    transfer();
}

QString fl_app_com::transfer_time_date(vfdco_time_t t, vfdco_date_t d)
{
    clear_buffer();
    set_command_byte(is_legacy_protocol ? 0x10 : 0x20);
    buf_tx[COM_PROTOCOL_DATA_OFFSET]     = t.s;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 1] = t.m;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 2] = t.h;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 3] = d.d;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 4] = d.m;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 5] = d.y;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 6] = 0x23;
    transfer();

    // Wait to receive
    QThread::msleep(100);
    return receive_and_extract_data(1000);
}

void fl_app_com::transfer_brightness(uint8_t disp_or_led, uint8_t dim_factor)
{
    clear_buffer();
    set_command_byte(0x21);
    buf_tx[COM_PROTOCOL_CONTROL_OFFSET] = disp_or_led;
    buf_tx[COM_PROTOCOL_PARAM0_OFFSET]  = dim_factor;
    transfer();
}

void fl_app_com::transfer_night_shift(vfdco_time_t start, vfdco_time_t stop)
{
    clear_buffer();
    set_command_byte(0x22);
    buf_tx[COM_PROTOCOL_DATA_OFFSET]     = start.h;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 1] = start.m;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 2] = stop.h;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 3] = stop.m;
    transfer();
}

void fl_app_com::legacy_transfer_night_shift(vfdco_time_t start, vfdco_time_t stop, uint8_t en1, uint8_t en2)
{
    clear_buffer();
    set_command_byte(0x11);
    buf_tx[COM_PROTOCOL_DATA_OFFSET] = 0x23;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 1] = start.m;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 2] = start.h;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 3] = stop.m;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 3] = stop.h;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 5] = en1;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 6] = en2;
    buf_tx[COM_PROTOCOL_DATA_OFFSET + 7] = 0x23;
    transfer();
}

void fl_app_com::transfer_welcome_set(uint8_t *message)
{
    clear_buffer();
    set_command_byte(is_legacy_protocol ? 0x21 : 0x25);
    memcpy(buf_tx + COM_PROTOCOL_DATA_OFFSET, message, CONFIG_NUM_DIGITS);
    transfer();
}

void fl_app_com::transfer_message(uint8_t *m1, uint8_t *m2, uint8_t *m3, uint8_t *m4, uint8_t duration)
{
    clear_buffer();
    if(!is_legacy_protocol) {
        set_command_byte(0x26);
        if(m4) memcpy(buf_tx + COM_PROTOCOL_DATA_OFFSET + CONFIG_NUM_DIGITS * 3, m4, CONFIG_NUM_DIGITS);
    } else {
        set_command_byte(0x1F);
        buf_tx[20] = duration;
        buf_tx[21] = m2 ? 1 : 0;
        buf_tx[22] = m3 ? 1 : 0;
    }
    memcpy(buf_tx + COM_PROTOCOL_DATA_OFFSET, m1, CONFIG_NUM_DIGITS);
    if(m2) memcpy(buf_tx + COM_PROTOCOL_DATA_OFFSET + CONFIG_NUM_DIGITS, m2, CONFIG_NUM_DIGITS);
    if(m3) memcpy(buf_tx + COM_PROTOCOL_DATA_OFFSET + CONFIG_NUM_DIGITS * 2, m3, CONFIG_NUM_DIGITS);
    transfer();
}

QString fl_app_com::transfer_clock_control(com_protocol_clock_control_t control)
{
    clear_buffer();
    if(!is_legacy_protocol) {
        set_command_byte(0x30);
        buf_tx[COM_PROTOCOL_CONTROL_OFFSET] = (uint8_t)control;
    } else {
        switch(control) {
            case COM_PROTOCOL_FW_VERSION_REQ: set_command_byte(0x22); break;
            case COM_PROTOCOL_SETTINGS_SAVE_REQ: set_command_byte(0x33); break;
            case COM_PROTOCOL_DEFAULT_REQ: set_command_byte(0x34); break;
            default: QString("");
        }
    }
    transfer();
    // Wait to receive
    QThread::msleep(100);
    // Response
    if(control == COM_PROTOCOL_FW_VERSION_REQ || control == COM_PROTOCOL_HW_VERSION_REQ || control == COM_PROTOCOL_ENABLED_INSTANCES_REQ) {
        return receive_and_extract_data(1000);
    } else return QString("");
}

void fl_app_com::transfer_dfu_request()
{
    memset(buf_tx, 0x30, BUF_TX_SIZE); // All 0x30!
    if(!is_bluetooth_communication) {
        serial_port.write((char const *)buf_tx, BUF_TX_SIZE);
        serial_port.waitForBytesWritten(10);
    }
}

bool fl_app_com::legacy_protocol()
{
    return is_legacy_protocol;
}

fl_app_status_t fl_app_com::getStatus()
{
    return status;
}

void fl_app_com::bt_discovery_finished() {
    qDebug() << "Device discovery finished";

    const QList<QBluetoothDeviceInfo> foundDevices = discoveryAgent->discoveredDevices();
    for (auto nextDevice : foundDevices) {
        if (nextDevice.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
            qDebug() << "Device found: " << nextDevice.address() << ", Name: " << nextDevice.name() << ", Strength: " << nextDevice.rssi();;
            if(!nextDevice.name().isEmpty()) deviceList.append(nextDevice);
            if(nextDevice.name().compare(bt_device_name) == 0) {
                qDebug() << "Fluorescence Found!";
                break;
            }
        }
    }
    if(deviceList.isEmpty()) {
        qDebug() << "No device matching Fluorescence!";
        return;
    }

    // Find selected device name in device list and scan for services
    QString toFind(bt_device_name);
    auto selectedDeviceIterator = std::find_if(deviceList.begin(), deviceList.end(), [&toFind] (const QBluetoothDeviceInfo& info) {
        return QString::compare(toFind, info.name()) == 0;
    });
    if (selectedDeviceIterator != deviceList.end()) {
        int vec_idx = selectedDeviceIterator - deviceList.begin();
        auto selectedDevice = new QBluetoothDeviceInfo(deviceList.at(vec_idx));

        if(!selectedDevice->isValid()) qDebug() << "Device is not valid!!";

        if(controller) delete controller;
        controller = nullptr;

        // Connecting signals and slots for connecting to LE services.
        controller = QLowEnergyController::createCentral(*selectedDevice);

        connect(controller, &QLowEnergyController::connected,
                this, &fl_app_com::bt_connect_device_connected);
        connect(controller, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
                this, &fl_app_com::bt_connect_device_error);
        connect(controller, &QLowEnergyController::disconnected,
                this, &fl_app_com::bt_connect_device_disconnected);
        connect(controller, &QLowEnergyController::serviceDiscovered,
                this, &fl_app_com::bt_connect_services_add);
        connect(controller, &QLowEnergyController::discoveryFinished,
                this, &fl_app_com::bt_connect_services_scan_complete);

        //controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
        controller->connectToDevice();
        currentDevice = selectedDevice;
    }

}

// 1. Connected? Discover Services
void fl_app_com::bt_connect_device_connected() {
    controller->discoverServices();
}
void fl_app_com::bt_connect_device_disconnected() {
    qWarning() << "Disconnect from device";
}
void fl_app_com::bt_connect_device_error(QLowEnergyController::Error) {
    qWarning() << "Error: " << controller->errorString();
    qDebug() << QString("Back\n(%1)").arg(controller->errorString());
}

// 2. Callback each time BLE service is found
void fl_app_com::bt_connect_services_add(const QBluetoothUuid &serviceUuid) {
    QLowEnergyService *service = controller->createServiceObject(serviceUuid);
    if(!service) {
        qWarning() << "Cannot create service for uuid";
        return;
    }
    serviceList.append(service);
}

// 3. Scan done, connect to main service.
void fl_app_com::bt_connect_services_scan_complete() {
    if(serviceList.isEmpty()) return;
    qDebug() << "Service scan done. Listing services found (" << serviceList.size() << "): ";

    QLowEnergyService *service = serviceList.first();

    if (service->state() == QLowEnergyService::DiscoveryRequired) {
        connect(service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
                        this, SLOT(bt_connect_services_details_discovered(QLowEnergyService::ServiceState)));
        service->discoverDetails();
        currentService = service;
    }
}

// 4. After connection to service, discover characteristics
void fl_app_com::bt_connect_services_details_discovered(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::ServiceDiscovered) {
        if (newState != QLowEnergyService::DiscoveringServices) {
            QMetaObject::invokeMethod(this, "characteristicsUpdated", Qt::QueuedConnection);
        }
        return;
    }

    qDebug() << "Services Name: " << currentService->serviceName();
    qDebug() << "Services UUID: " << currentService->serviceUuid();

    QString svcUUID = currentService->serviceUuid().toString().trimmed();
    svcUUID.replace(8, 1, '1');
    qDebug() << "Custom Service UUID: " << svcUUID;

    // Setup characteristic
    QLowEnergyCharacteristic *link = new QLowEnergyCharacteristic(currentService->characteristic(QBluetoothUuid::fromString(svcUUID)));
    qDebug() << currentService->state();
    currentCharacteristic = link;

    // Setup Notification: Descriptor
    QLowEnergyDescriptor notification = link->descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
    if (!notification.isValid()) qDebug() << "The notification descriptor is invalid";
    currentService->writeDescriptor(notification, QByteArray::fromHex("0x1"));

    // Connect notification
    connect(currentService,
            SIGNAL(characteristicChanged(QLowEnergyCharacteristic, QByteArray)),
            this, SLOT(characteristicChanged(QLowEnergyCharacteristic, QByteArray)));

    qDebug() << "Connected to Fluorescence";

    // From here on, do initialization
    buf_rx = new uint8_t[BUF_RX_SIZE];

    // Try if port is legacy
    buf_tx = new uint8_t[BUF_TX_SIZE_LEGACY];

    determine_legacy();
    emit app_com_connected();
}

void fl_app_com::characteristicChanged(QLowEnergyCharacteristic ch, QByteArray arr) {
    qDebug() << "Incoming: " << arr;
    if(arr.length() > 0) {
        if(incoming_characteristic) delete incoming_characteristic;
        incoming_characteristic = new QByteArray(arr);
    }
    Q_UNUSED(ch);
}

void fl_app_com::determine_legacy()
{
    // Try if port is legacy
    buf_tx = new uint8_t[BUF_TX_SIZE_LEGACY];
    _transfer = &fl_app_com::legacy_transfer;
    QString fw_str = transfer_clock_control(COM_PROTOCOL_FW_VERSION_REQ);
    // qDebug() << "FW_STR: " << fw_str;
    if(fw_str.at(0) == "v" && fw_str.at(1) == "2") {
        // Legacy protocol!
        is_legacy_protocol = true;
        status = FL_APP_COM_STATUS_OK;
    } else {
        // Not legacy. try if protocol is 3.0
        delete[] buf_tx;
        buf_tx = new uint8_t[BUF_TX_SIZE];
        _transfer = &fl_app_com::regular_transfer;

        // qDebug() << "Testing regular protocol";
        QString fw_str = transfer_clock_control(COM_PROTOCOL_FW_VERSION_REQ);
        // qDebug() << "FW_STR: " << fw_str;

        if(fw_str.at(0) == "3" && fw_str.at(1) == ".") {
            is_legacy_protocol = false;
            status = FL_APP_COM_STATUS_OK;
        } else {
            is_legacy_protocol = false;
            status = FL_APP_COM_STATUS_OK;
            // status = FL_APP_COM_STATUS_CONNECTION_FAILED;
        }
    }
}

void fl_app_com::clear_buffer()
{
    // Clear local
    if(!is_legacy_protocol) memset(buf_tx, 0x00, BUF_TX_SIZE);
    else memset(buf_tx, 0x00, BUF_TX_SIZE_LEGACY);
    memset(buf_rx, 0x00, BUF_RX_SIZE);
    // Clear global
    if(!is_bluetooth_communication) serial_port.clear();
}

void fl_app_com::set_command_byte(uint8_t value)
{
    buf_tx[COM_PROTOCOL_COMMAND_OFFSET] = value;
}

void fl_app_com::transfer_serial(uint8_t *clr_arr, uint8_t mode)
{
    clear_buffer();
    if(!is_legacy_protocol) {
        set_command_byte(mode ? 0x01 : 0x00);
        memcpy(buf_tx + COM_PROTOCOL_DATA_OFFSET, clr_arr, CONFIG_NUM_BYTES);
        transfer();
    } else {
        set_command_byte(mode ? 0x02 : 0x01);
        uint8_t *buf_tx_alt = buf_tx + COM_PROTOCOL_DATA_OFFSET;
        for(uint_fast8_t i = 0; i < CONFIG_NUM_BYTES; ++i) {
            buf_tx_alt[3 * i] = clr_arr[4 * i + 1]; // G
            buf_tx_alt[3 * i + 1] = clr_arr[4 * i]; // R
            buf_tx_alt[3 * i + 2] = clr_arr[4 * i + 2]; // B
        }
        legacy_transfer();
    }
}

void fl_app_com::transfer()
{
    (this->*_transfer)();
}

void fl_app_com::regular_transfer()
{
    buf_tx[0] = 0x24;
    buf_tx[BUF_TX_SIZE - 1] = 0x25;
    if(is_bluetooth_communication) {
        if(currentService->state() == QLowEnergyService::ServiceDiscovered){
            // Send
            currentService->writeCharacteristic(
                *currentCharacteristic,
                QByteArray((const char *)buf_tx, BUF_TX_SIZE),
                QLowEnergyService::WriteWithoutResponse
            );
        } else {
            qDebug() << "Not connected to service!";
        }
    } else {
        serial_port.write((char const *)buf_tx, BUF_TX_SIZE);
        serial_port.waitForBytesWritten(10);
    }

    for(int i = 0; i < BUF_TX_SIZE; ++i) qDebug() << buf_tx[i];
    qDebug() << "";
}

void fl_app_com::legacy_transfer()
{
    buf_tx[0] = 0x23;
    buf_tx[BUF_TX_SIZE - 1] = 0x24;

    if(is_bluetooth_communication) {
        if(currentService->state() == QLowEnergyService::ServiceDiscovered){
            // Send
            currentService->writeCharacteristic(
                *currentCharacteristic,
                QByteArray((const char *)buf_tx, BUF_TX_SIZE_LEGACY),
                QLowEnergyService::WriteWithoutResponse
            );
        } else {
            qDebug() << "Not connected to service!";
        }
    } else {
        serial_port.write((char const *)buf_tx, BUF_TX_SIZE_LEGACY);
        serial_port.waitForBytesWritten(10);
    }

    for(int i = 0; i < BUF_TX_SIZE; ++i) qDebug() << buf_tx[i];
    qDebug() << "";
}

QString fl_app_com::receive_and_extract_data(int wait_timeout)
{
    QByteArray byte_arr;
    if(is_bluetooth_communication) {
        QThread::msleep(500);
        if(incoming_characteristic) {
            byte_arr = QByteArray(*incoming_characteristic);
            incoming_characteristic = nullptr;
        } else {
            byte_arr = QByteArray("");
            incoming_characteristic = nullptr;
        }
    } else {
        byte_arr = receive_raw(wait_timeout);
    }
    // Msg format length
    if(byte_arr.length() != 10) return "len_err";
    // Msg format check
    if(byte_arr.at(0) == 0x23 && byte_arr.at(9) == 0x24) {
        if(!is_legacy_protocol) return "protocol_err";
    } else if(byte_arr.at(0) == 0x24 && byte_arr.at(9) == 0x25) {
        if(is_legacy_protocol) return "protocol_err";
    } else {
        return "fmt_err";
    }
    // Cut byte arr & return QString
    return QString::fromStdString(byte_arr.mid(2, 8).toStdString());
}

QByteArray fl_app_com::receive_raw(int wait_timeout)
{
    if (serial_port.waitForReadyRead(wait_timeout)) {
        // read request
        QByteArray request_data = serial_port.readAll();
        while (serial_port.waitForReadyRead(10)) request_data += serial_port.readAll();
        return request_data;
    } else {
        return QByteArray("");
    }
    serial_port.clear();
}
