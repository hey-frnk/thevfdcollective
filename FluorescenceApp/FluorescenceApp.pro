QT       += core gui \
            serialport \
            widgets \
            bluetooth

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++11

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
    clickablelabel.cpp \
    fl_app_com.cpp \
    fwupdate.cpp \
    main.cpp \
    fluorescenceapp.cpp \
    movablelabel.cpp \
    src/fl_app_colors.cpp \
    src/fl_app_lights.c \
    src/fl_app_util.c \
    src/fl_app_time.c

HEADERS += \
    ../FluorescenceV3/Commons/vfdco_clock_routine_defines.h \
    ../FluorescenceV3/Commons/vfdco_config.h \
    clickablelabel.h \
    fl_app_com.h \
    fluorescenceapp.h \
    fwupdate.h \
    movablelabel.h \
    src/fl_app_colors.h \
    src/fl_app_lights.h \
    src/fl_app_time.h

FORMS += \
    fluorescenceapp.ui \
    fwupdatestm.ui

TRANSLATIONS += \
    FluorescenceApp_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/green2blue.png \
    Resources/pastelrainbow.png \
    Resources/rainbow_alt.png \
    Resources/red2blue1.png \
    Resources/red2green_alt.png \
    Resources/computer.png \
    Resources/res0.jpg \
    Resources/res1.jpg \
    Resources/res2.jpg \
    Resources/res3.jpg \
    Resources/hsl_wheel.png \
    Resources/cursor.png \
    Resources/fl2_usb.jpg \
    Resources/fl3_usb.jpg

