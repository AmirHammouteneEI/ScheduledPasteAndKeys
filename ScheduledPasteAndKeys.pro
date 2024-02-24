QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    Task.cpp \
    TaskTabsManager.cpp \
    TaskThread.cpp \
    actions/AbstractAction.cpp \
    actions/ActionsTools.cpp \
    actions/PasteAction.cpp \
    actions/WaitAction.cpp \
    main.cpp \
    mainwindow.cpp \
    ui/CreateLoadTaskDialog.cpp \
    ui/TaskTab.cpp

HEADERS += \
    Task.h \
    TaskTabsManager.h \
    TaskThread.h \
    actions/AbstractAction.h \
    actions/ActionParameters.h \
    actions/ActionsTools.h \
    actions/PasteAction.h \
    actions/WaitAction.h \
    globals.h \
    mainwindow.h \
    ui/CreateLoadTaskDialog.h \
    ui/TaskTab.h

FORMS += \
    mainwindow.ui \
    ui/CreateLoadTaskDialog.ui

LIBS += -luser32

RESOURCES += \
    resources.qrc
