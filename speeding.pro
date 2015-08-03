QT       += core gui opengl multimedia sensors testlib network quick #androidextras
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = speeding
TEMPLATE = app

RESOURCES += resource.qrc

SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    game.cpp \
    box.cpp \
    point.cpp \
    maths.cpp \
    random.cpp \
    utility.cpp \
    colorscheme.cpp

HEADERS  += mainwindow.h \
    camera.h \
    game.h \
    box.h \
    maths.h \
    point.h \
    random.h \
    utility.h \
    colorscheme.h

CONFIG += mobility static c++11
MOBILITY = 

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/org/qtproject/example/speeding/Vibrate.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

