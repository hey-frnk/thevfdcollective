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
    main.cpp \
    fluorescencesimulator.cpp \
    vfdco_sim_com.cpp \
    vfdco_sim_display.cpp \
    vfdco_sim_hid.cpp \
    vfdco_sim_led.cpp \
    vfdco_sim_serialization.cpp \
    vfdco_sim_time.cpp

HEADERS += \
    ../vfdco_clock_routines.h \
    ../vfdco_com.h \
    ../vfdco_config.h \
    ../vfdco_config_sim.h \
    ../vfdco_display.h \
    ../vfdco_gui.h \
    ../vfdco_hid.h \
    ../vfdco_led.h \
    ../vfdco_lights.h \
    ../vfdco_serialization.h \
    ../vfdco_time.h \
    fluorescencesimulator.h \
    magic_enum.hpp

FORMS += \
    fluorescencesimulator.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
