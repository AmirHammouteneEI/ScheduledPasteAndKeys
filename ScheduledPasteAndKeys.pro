QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = ScheduledPCTasks

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-function -Wno-unused-parameter -Wno-unused-result

SOURCES += \
    ActionWidgetsManager.cpp \
    Task.cpp \
    TaskTabsManager.cpp \
    TaskThread.cpp \
    actions/AbstractAction.cpp \
    actions/ActionsTools.cpp \
    actions/KeysSequenceAction.cpp \
    actions/PasteAction.cpp \
    actions/WaitAction.cpp \
    main.cpp \
    mainwindow.cpp \
    ui/CreateLoadTaskDialog.cpp \
    ui/DataEditDialog.cpp \
    ui/KeysSelectorDialog.cpp \
    ui/KeysSequenceSelectedEditDialog.cpp \
    ui/KeysSequencesTableWidget.cpp \
    ui/SentenceSelectedEditDialog.cpp \
    ui/SentencesTableWidget.cpp \
    ui/TaskTab.cpp \
    ui/actionwidgets/AbstractActionWidget.cpp \
    ui/actionwidgets/KeysSequenceWidget.cpp \
    ui/actionwidgets/PasteWidget.cpp \
    ui/actionwidgets/WaitWidget.cpp \
    ui/createactiondialog/CreateKeysSequenceActionDialog.cpp \
    ui/createactiondialog/CreatePasteActionDialog.cpp \
    ui/createactiondialog/CreateWaitActionDialog.cpp \
    ui/getDelayDialog.cpp

HEADERS += \
    ActionWidgetsManager.h \
    Task.h \
    TaskTabsManager.h \
    TaskThread.h \
    actions/AbstractAction.h \
    actions/ActionParameters.h \
    actions/ActionsTools.h \
    actions/KeysSequenceAction.h \
    actions/PasteAction.h \
    actions/WaitAction.h \
    globals.h \
    mainwindow.h \
    ui/CreateLoadTaskDialog.h \
    ui/DataEditDialog.h \
    ui/KeysSelectorDialog.h \
    ui/KeysSequenceSelectedEditDialog.h \
    ui/KeysSequencesTableWidget.h \
    ui/SentenceSelectedEditDialog.h \
    ui/SentencesTableWidget.h \
    ui/TaskTab.h \
    ui/actionwidgets/AbstractActionWidget.h \
    ui/actionwidgets/KeysSequenceWidget.h \
    ui/actionwidgets/PasteWidget.h \
    ui/actionwidgets/WaitWidget.h \
    ui/createactiondialog/CreateKeysSequenceActionDialog.h \
    ui/createactiondialog/CreatePasteActionDialog.h \
    ui/createactiondialog/CreateWaitActionDialog.h \
    ui/getDelayDialog.h

FORMS += \
    mainwindow.ui \
    ui/CreateLoadTaskDialog.ui \
    ui/DataEditDialog.ui \
    ui/KeysSelectorDialog.ui \
    ui/KeysSequenceSelectedEditDialog.ui \
    ui/SentenceSelectedEditDialog.ui \
    ui/createactiondialog/CreateKeysSequenceActionDialog.ui \
    ui/createactiondialog/CreatePasteActionDialog.ui \
    ui/createactiondialog/CreateWaitActionDialog.ui \
    ui/getDelayDialog.ui

LIBS += -luser32

TRANSLATIONS = ScheduledPCTasks_fr.ts

RESOURCES += \
    resources.qrc

RC_ICONS = img/programIcon.ico

VERSION = 0.1.0
QMAKE_TARGET_COMPANY = "Amir Hammoutene"
QMAKE_TARGET_COPYRIGHT = "GNU general public license version 3"

DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\" \
           APP_COMPANY=\"\\\"$${QMAKE_TARGET_COMPANY}\\\"\" \
           APP_COPYRIGHT=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\"
