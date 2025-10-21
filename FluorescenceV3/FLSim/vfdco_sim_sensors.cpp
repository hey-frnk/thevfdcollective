#include "../Commons/vfdco_sensors.h"
#include <QTimer>
#include <QDebug>
#include <queue>

/*
| Sensor      | Parameter         | Recommended Rate      | Notes                                            |
| ----------- | ----------------- | --------------------- | ------------------------------------------------ |
| **BME280**  | Temperature       | \~1–2 Hz (500–1000ms) | Sensor updates slowly, filtering useful          |
|             | Humidity          | \~1–2 Hz              | Strongly benefits from averaging                 |
|             | Pressure          | \~10 Hz max (100ms)   | Higher rate OK, but needs filtering to be stable |
| **LSM6DS3** | Tilt (pitch/roll) | \~30–60 Hz (16–33ms)  | Motion-sensitive; apply smoothing/LPF            |
*/

// --- Buffers for Moving Average ---
std::deque<float> tempBuffer;
std::deque<float> humidityBuffer;
std::deque<float> pressureBuffer;
std::deque<float> pitchBuffer;
std::deque<float> rollBuffer;

// --- Global Variables ---
float temperature = 0.0f;
float humidity = 0.0f;
float pressure = 0.0f;
float pitch = 0.0f;
float roll = 0.0f;

const int TEMP_WINDOW = 8;
const int HUM_WINDOW = 12;
const int PRES_WINDOW = 16;
const int TILT_WINDOW = 6;

QTimer bme280_updater;
QTimer lsm6ds3_updater;

// --- Simulated Raw Sensor Readings ---
float simulateTemperature() { return 20.0f + (rand() % 1000 - 500) / 1000.0f; }  // ~20 ±0.5
float simulateHumidity()    { return 50.0f + (rand() % 2000 - 1000) / 1000.0f; } // ~50 ±1.0
float simulatePressure()    { return 1013.25f + (rand() % 400 - 200) / 100.0f; } // ~1013 ±2.0
float simulatePitch()       { return 0.0f + (rand() % 200 - 100) / 10.0f; }      // ~0 ±10 deg
float simulateRoll()        { return 0.0f + (rand() % 200 - 100) / 10.0f; }

// --- Compute Moving Average Helper ---
float movingAvg(std::deque<float>& buffer, float value, int window) {
    buffer.push_back(value);
    if (buffer.size() > window)
        buffer.pop_front();

    float sum = std::accumulate(buffer.begin(), buffer.end(), 0.0f);
    return sum / buffer.size();
}

// --- Sensor Update Callbacks ---
void BME280_Update_Callback() {
    temperature = movingAvg(tempBuffer, simulateTemperature(), TEMP_WINDOW);
    humidity    = movingAvg(humidityBuffer, simulateHumidity(), HUM_WINDOW);
    pressure    = movingAvg(pressureBuffer, simulatePressure(), PRES_WINDOW);

    // qDebug() << "BME280:"
    //          << "Temp:" << temperature
    //          << "Hum:" << humidity
    //          << "Pres:" << pressure
    //          << "Pres, inHg" << (uint16_t)(pressure * 2.95299806);
}

void LSM6DS3_Update_Callback() {
    pitch = movingAvg(pitchBuffer, simulatePitch(), TILT_WINDOW);
    roll  = movingAvg(rollBuffer, simulateRoll(), TILT_WINDOW);

    // qDebug() << "LSM6DS3:"
    //          << "Pitch:" << pitch
    //          << "Roll:" << roll;
}

void vfdco_sensors_init_temperature() {
    // Set intervals
    bme280_updater.setInterval(150);      // 150 ms
    lsm6ds3_updater.setInterval(333);     // 333 ms

    // Connect signals to slots
    QObject::connect(&bme280_updater, &QTimer::timeout, &BME280_Update_Callback);
    QObject::connect(&lsm6ds3_updater, &QTimer::timeout, &LSM6DS3_Update_Callback);

    // Start timers
    bme280_updater.start();
    lsm6ds3_updater.start();
}
void vfdco_sensors_init_humidity() {
    humidity = 0;
}
void vfdco_sensors_init_pressure() {
    pressure = 0;
}
void vfdco_sensors_init_tilt() {
    pitch = 0;
    roll = 0;
}

void vfdco_sensors_get_temperature(vfdco_temperature_t *temp) {
    temp->celsius_x10 = (int16_t)(temperature * 10);
    temp->fahrenheit_x10 = (int16_t)((temperature * 9 / 5 + 32) * 10);
}

void vfdco_sensors_get_humidity(vfdco_humidity_t *hum) {
    hum->humidity_pct = (uint8_t)(humidity);
}

void vfdco_sensors_get_pressure(vfdco_pressure_t *pres) {
    pres->hpa_x10 = (uint16_t)(pressure * 10);
    pres->inhg_x100 = (uint16_t)(pressure * 2.95299806);
}

void vfdco_sensors_get_tilt(vfdco_tilt_t *tilt) {
    tilt->pitch_deg = (int8_t)(pitch);
    tilt->roll_deg = (int8_t)(roll);
}
