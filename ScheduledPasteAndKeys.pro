QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ui/CreateLoadTaskDialog.cpp \
    ui/TaskTab.cpp

HEADERS += \
    mainwindow.h \
    ui/CreateLoadTaskDialog.h \
    ui/TaskTab.h

FORMS += \
    mainwindow.ui \
    ui/CreateLoadTaskDialog.ui

RESOURCES += \
    resources.qrc
