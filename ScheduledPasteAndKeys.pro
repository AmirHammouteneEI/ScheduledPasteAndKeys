QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    TaskTabsManager.cpp \
    main.cpp \
    mainwindow.cpp \
    ui/CreateLoadTaskDialog.cpp \
    ui/TaskTab.cpp

HEADERS += \
    TaskTabsManager.h \
    globals.h \
    mainwindow.h \
    ui/CreateLoadTaskDialog.h \
    ui/TaskTab.h

FORMS += \
    mainwindow.ui \
    ui/CreateLoadTaskDialog.ui

RESOURCES += \
    resources.qrc
