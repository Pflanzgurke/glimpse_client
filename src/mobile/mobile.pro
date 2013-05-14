TEMPLATE = app

QT += gui quick qml concurrent

include(../libclient/libclient.pri)

android {
    HEADERS += androidhelper.h \
               statusbarhelper.h
    SOURCES += androidhelper.cpp \
               statusbarhelper.cpp
}

SOURCES += \
    main.cpp

RESOURCES += \
    resources/resource.qrc

OTHER_FILES += \
    resources/qml/main.qml \
    resources/qml/packettrain.qml \
    resources/qml/android/TextFieldStyle.qml \
    resources/qml/android/TabViewStyle.qml \
    resources/qml/android/SliderStyle.qml \
    resources/qml/android/ProgressBarStyle.qml \
    resources/qml/android/ButtonStyle.qml \
    resources/qml/android/BackButton.qml \
    resources/qml/android/AndroidDelegate.qml \
    resources/qml/android/images/toolbar.png \
    resources/qml/android/images/textinput.png \
    resources/qml/android/images/tabs_standard.png \
    resources/qml/android/images/tab_selected.png \
    resources/qml/android/images/navigation_previous_item.png \
    resources/qml/android/images/navigation_next_item.png \
    resources/qml/android/images/button_pressed.png \
    resources/qml/android/images/button_default.png \
    resources/qml/Settings.qml \
    resources/qml/Tests.qml
