QT       += core gui \
            serialport \
            widgets
            # uiplugin

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++11
            # plugin

# TEMPLATE = lib
#PKG_CONFIG = /usr/local/bin/pkg-config
#CONFIG += link_pkgconfig
#PKGCONFIG += opencv

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
    QtColorWidgets/abstract_widget_list.cpp \
    QtColorWidgets/bound_color_selector.cpp \
    QtColorWidgets/color_2d_slider.cpp \
    QtColorWidgets/color_delegate.cpp \
    QtColorWidgets/color_dialog.cpp \
    QtColorWidgets/color_line_edit.cpp \
    QtColorWidgets/color_list_widget.cpp \
    QtColorWidgets/color_names.cpp \
    QtColorWidgets/color_palette.cpp \
    QtColorWidgets/color_palette_model.cpp \
    QtColorWidgets/color_palette_widget.cpp \
    QtColorWidgets/color_preview.cpp \
    QtColorWidgets/color_selector.cpp \
    QtColorWidgets/color_utils.cpp \
    QtColorWidgets/color_wheel.cpp \
    QtColorWidgets/gradient_editor.cpp \
    QtColorWidgets/gradient_slider.cpp \
    QtColorWidgets/hue_slider.cpp \
    QtColorWidgets/swatch.cpp \
    clickablelabel.cpp \
    fl_app_com.cpp \
    fwupdatestm.cpp \
    main.cpp \
    fluorescenceapp.cpp \
    src/fl_app_colors.cpp \
    src/fl_app_lights.c \
    src/fl_app_time.c

HEADERS += \
    ../FluorescenceV3/Commons/vfdco_clock_routine_defines.h \
    ../FluorescenceV3/vfdco_config.h \
    QtColorWidgets/AbstractWidgetList \
    QtColorWidgets/BoundColorSelector \
    QtColorWidgets/ColorDelegate \
    QtColorWidgets/ColorDialog \
    QtColorWidgets/ColorListWidget \
    QtColorWidgets/ColorPreview \
    QtColorWidgets/ColorSelector \
    QtColorWidgets/ColorWheel \
    QtColorWidgets/GradientEditor \
    QtColorWidgets/GradientSlider \
    QtColorWidgets/HueSlider \
    QtColorWidgets/abstract_widget_list.hpp \
    QtColorWidgets/bound_color_selector.hpp \
    QtColorWidgets/color_2d_slider.hpp \
    QtColorWidgets/color_delegate.hpp \
    QtColorWidgets/color_dialog.hpp \
    QtColorWidgets/color_line_edit.hpp \
    QtColorWidgets/color_list_widget.hpp \
    QtColorWidgets/color_names.hpp \
    QtColorWidgets/color_palette.hpp \
    QtColorWidgets/color_palette_model.hpp \
    QtColorWidgets/color_palette_widget.hpp \
    QtColorWidgets/color_preview.hpp \
    QtColorWidgets/color_selector.hpp \
    QtColorWidgets/color_utils.hpp \
    QtColorWidgets/color_wheel.hpp \
    QtColorWidgets/colorwidgets_global.hpp \
    QtColorWidgets/gradient_editor.hpp \
    QtColorWidgets/gradient_helper.hpp \
    QtColorWidgets/gradient_slider.hpp \
    QtColorWidgets/hue_slider.hpp \
    QtColorWidgets/swatch.hpp \
    QtColorWidgets/ui_color_dialog.h \
    QtColorWidgets/ui_color_palette_widget.h \
    clickablelabel.h \
    fl_app_com.h \
    fluorescenceapp.h \
    fwupdatestm.h \
    src/fl_app_colors.h \
    src/fl_app_lights.h \
    src/fl_app_time.h

FORMS += \
    QtColorWidgets/color_dialog.ui \
    QtColorWidgets/color_palette_widget.ui \
    fluorescenceapp.ui \
    fwupdatestm.ui

TRANSLATIONS += \
    FluorescenceApp_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#target.path = $$[QT_INSTALL_PLUGINS]/designer
#INSTALLS += target

RESOURCES += \
    QtColorWidgets/color_widgets.qrc \
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
    Resources/HXP_TN.png



#DISTFILES += \
#    QtColorWidgets/CMakeLists.txt \
#    QtColorWidgets/alphaback.png \
#    QtColorWidgets/new_plugin
