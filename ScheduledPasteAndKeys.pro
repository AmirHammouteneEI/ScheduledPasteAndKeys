QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    ActionWidgetsManager.cpp \
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
    ui/DataEditDialog.cpp \
    ui/SelectSentenceDialog.cpp \
    ui/SentenceSelectedEditDialog.cpp \
    ui/SentencesTableWidget.cpp \
    ui/TaskTab.cpp \
    ui/actionwidgets/AbstractActionWidget.cpp \
    ui/actionwidgets/PasteWidget.cpp \
    ui/actionwidgets/WaitWidget.cpp \
    ui/createactiondialog/CreatePasteActionDialog.cpp \
    ui/getDelayDialog.cpp

HEADERS += \
    ActionWidgetsManager.h \
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
    ui/DataEditDialog.h \
    ui/SelectSentenceDialog.h \
    ui/SentenceSelectedEditDialog.h \
    ui/SentencesTableWidget.h \
    ui/TaskTab.h \
    ui/actionwidgets/AbstractActionWidget.h \
    ui/actionwidgets/PasteWidget.h \
    ui/actionwidgets/WaitWidget.h \
    ui/createactiondialog/CreatePasteActionDialog.h \
    ui/getDelayDialog.h

FORMS += \
    mainwindow.ui \
    ui/CreateLoadTaskDialog.ui \
    ui/DataEditDialog.ui \
    ui/SelectSentenceDialog.ui \
    ui/SentenceSelectedEditDialog.ui \
    ui/createactiondialog/CreatePasteActionDialog.ui \
    ui/getDelayDialog.ui

LIBS += -luser32

RESOURCES += \
    resources.qrc
