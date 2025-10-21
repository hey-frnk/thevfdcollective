#include "../Commons/vfdco_audio.h"
#include "../Commons/vfdco_util.h"

#include <QSoundEffect>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#ifdef _MIC_IMPLEMENTATION
#error "An implementation of the microphone driver already exists!"
#endif
#define _MIC_IMPLEMENTATION

static QSoundEffect beepSound;
static QTimer pollTimer;


// Initializer
void vfdco_audio_init() {
    // Load the sound once
    for (const QAudioDevice &device : QMediaDevices::audioOutputs()) {
        qDebug() << "Audio device:" << device.description();
    }

    beepSound.setAudioDevice(QMediaDevices::defaultAudioOutput());
    beepSound.setSource(QUrl("qrc:/alarm.wav")); // Qt resource path or real file
    beepSound.setLoopCount(1); // Play once
    beepSound.setVolume(0.8);  // Between 0.0 and 1.0

    QObject::connect(&pollTimer, &QTimer::timeout, []() {
        if (beepSound.loopsRemaining() == 0 && beepSound.isPlaying() == false) {
            qDebug() << "Beep finished!";
            pollTimer.stop();
        }
    });
    pollTimer.start(10); // Poll every 10 ms
}

uint8_t _vfdco_audio_mic_random06() {
    return vfdco_util_random(3) % 7;
}

// Read in level
uint8_t vfdco_audio_mic_read_level(void) {
    uint8_t y0 = _vfdco_audio_mic_random06();
    // uint8_t y1 = ((y0 << 1) + y0 + _vfdco_mic_random06()) >> 2;
    // uint8_t y2 = ((y1 << 1) + y1 + _vfdco_mic_random06()) >> 2;
    return _vfdco_audio_mic_random06() > 4 ? y0 : 0;
}

void vfdco_audio_speaker_play(uint16_t frequency_hz) {
    Q_UNUSED(frequency_hz); // For now, frequency is ignored
    qDebug() << "Sound URL:" << beepSound.source();

    if (beepSound.isPlaying() == false) {
        beepSound.setLoopCount(1); // Play once
        beepSound.setVolume(0.8);  // Between 0.0 and 1.0
        beepSound.play();
    }
}

void vfdco_audio_speaker_stop() {
    beepSound.stop();
}

uint_fast8_t vfdco_audio_speaker_is_active() {
    return beepSound.isPlaying();
}
