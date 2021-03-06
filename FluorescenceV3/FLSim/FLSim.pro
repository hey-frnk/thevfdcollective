QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../Commons/vfdco_clock_routines.c \
    ../Commons/vfdco_gui.c \
    ../Commons/vfdco_lights.c \
    ../Commons/vfdco_util.c \
    main.cpp \
    fluorescencesimulator.cpp \
    vfdco_sim_com.cpp \
    vfdco_sim_display.cpp \
    vfdco_sim_hid.cpp \
    vfdco_sim_led.cpp \
    vfdco_sim_serialization.cpp \
    vfdco_sim_time.cpp \
    vfdco_sim_virtual_mic.cpp

HEADERS += \
    ../Commons/vfdco_clock_routines.h \
    ../Commons/vfdco_gui.h \
    ../Commons/vfdco_lights.h \
    ../Commons/vfdco_com.h \
    ../Commons/vfdco_config.h \
    ../Commons/vfdco_config_sim.h \
    ../Commons/vfdco_display.h \
    ../Commons/vfdco_hid.h \
    ../Commons/vfdco_led.h \
    ../Commons/vfdco_mic.h \
    ../Commons/vfdco_serialization.h \
    ../Commons/vfdco_time.h \
    ../Commons/vfdco_util.h \
    fluorescencesimulator.h \
    magic_enum.hpp

FORMS += \
    fluorescencesimulator.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    protocol.csv
