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
    actions/CursorMovementsAction.cpp \
    actions/KeysSequenceAction.cpp \
    actions/PasteAction.cpp \
    actions/RunningOtherTaskAction.cpp \
    actions/SystemCommandsAction.cpp \
    actions/WaitAction.cpp \
    main.cpp \
    mainwindow.cpp \
    ui/actionwidgets/RunningOtherTaskWidget.cpp \
    ui/createactiondialogs/CreateRunningOtherTaskActionDialog.cpp \
    ui/various_dialogs/CreateAutorunDesktopShortcutDialog.cpp \
    ui/various_dialogs/CreateLoadTaskDialog.cpp \
    ui/editselectedactiondialogs/CursorMovementsSelectedEditDialog.cpp \
    ui/tablewidgets/CursorMovementsTableWidget.cpp \
    ui/various_dialogs/DataEditDialog.cpp \
    ui/various_dialogs/KeysSelectorDialog.cpp \
    ui/editselectedactiondialogs/KeysSequenceSelectedEditDialog.cpp \
    ui/tablewidgets/KeysSequencesTableWidget.cpp \
    ui/editselectedactiondialogs/SentenceSelectedEditDialog.cpp \
    ui/tablewidgets/SentencesTableWidget.cpp \
    ui/various_dialogs/StartupTaskEditDialog.cpp \
    ui/various_dialogs/StartupTasksDialog.cpp \
    ui/tablewidgets/StartupTasksTableWidget.cpp \
    ui/TaskTab.cpp \
    ui/actionwidgets/AbstractActionWidget.cpp \
    ui/actionwidgets/CursorMovementsWidget.cpp \
    ui/actionwidgets/KeysSequenceWidget.cpp \
    ui/actionwidgets/PasteWidget.cpp \
    ui/actionwidgets/SystemCommandWidget.cpp \
    ui/actionwidgets/WaitWidget.cpp \
    ui/createactiondialogs/CreateCursorMovementsActionDialog.cpp \
    ui/createactiondialogs/CreateKeysSequenceActionDialog.cpp \
    ui/createactiondialogs/CreatePasteActionDialog.cpp \
    ui/createactiondialogs/CreateSystemCommandActionDialog.cpp \
    ui/createactiondialogs/CreateWaitActionDialog.cpp \
    ui/getdialogs/getAutoRenameOptionDialog.cpp \
    ui/getdialogs/getCursorCoordinatesWidget.cpp \
    ui/getdialogs/getDelayDialog.cpp \
    ui/getdialogs/getFilePathDialog.cpp \
    ui/getdialogs/getFolderPathDialog.cpp \
    ui/getdialogs/getImagePathDialog.cpp \
    ui/getdialogs/getProgramPathDialog.cpp

HEADERS += \
    ActionWidgetsManager.h \
    Task.h \
    TaskTabsManager.h \
    TaskThread.h \
    actions/AbstractAction.h \
    actions/ActionParameters.h \
    actions/ActionsTools.h \
    actions/CursorMovementsAction.h \
    actions/KeysSequenceAction.h \
    actions/PasteAction.h \
    actions/RunningOtherTaskAction.h \
    actions/SystemCommandsAction.h \
    actions/WaitAction.h \
    globals.h \
    mainwindow.h \
    ui/actionwidgets/RunningOtherTaskWidget.h \
    ui/createactiondialogs/CreateRunningOtherTaskActionDialog.h \
    ui/various_dialogs/CreateAutorunDesktopShortcutDialog.h \
    ui/various_dialogs/CreateLoadTaskDialog.h \
    ui/editselectedactiondialogs/CursorMovementsSelectedEditDialog.h \
    ui/tablewidgets/CursorMovementsTableWidget.h \
    ui/CustomPrimaryWidgets.h \
    ui/various_dialogs/DataEditDialog.h \
    ui/various_dialogs/KeysSelectorDialog.h \
    ui/editselectedactiondialogs/KeysSequenceSelectedEditDialog.h \
    ui/tablewidgets/KeysSequencesTableWidget.h \
    ui/editselectedactiondialogs/SentenceSelectedEditDialog.h \
    ui/tablewidgets/SentencesTableWidget.h \
    ui/various_dialogs/StartupTaskEditDialog.h \
    ui/various_dialogs/StartupTasksDialog.h \
    ui/tablewidgets/StartupTasksTableWidget.h \
    ui/TaskTab.h \
    ui/actionwidgets/AbstractActionWidget.h \
    ui/actionwidgets/CursorMovementsWidget.h \
    ui/actionwidgets/KeysSequenceWidget.h \
    ui/actionwidgets/PasteWidget.h \
    ui/actionwidgets/SystemCommandWidget.h \
    ui/actionwidgets/WaitWidget.h \
    ui/createactiondialogs/CreateCursorMovementsActionDialog.h \
    ui/createactiondialogs/CreateKeysSequenceActionDialog.h \
    ui/createactiondialogs/CreatePasteActionDialog.h \
    ui/createactiondialogs/CreateSystemCommandActionDialog.h \
    ui/createactiondialogs/CreateWaitActionDialog.h \
    ui/getdialogs/getAutoRenameOptionDialog.h \
    ui/getdialogs/getCursorCoordinatesWidget.h \
    ui/getdialogs/getDelayDialog.h \
    ui/getdialogs/getFilePathDialog.h \
    ui/getdialogs/getFolderPathDialog.h \
    ui/getdialogs/getImagePathDialog.h \
    ui/getdialogs/getProgramPathDialog.h

FORMS += \
    mainwindow.ui \
    ui/createactiondialogs/CreateRunningOtherTaskActionDialog.ui \
    ui/various_dialogs/CreateAutorunDesktopShortcutDialog.ui \
    ui/various_dialogs/CreateLoadTaskDialog.ui \
    ui/editselectedactiondialogs/CursorMovementsSelectedEditDialog.ui \
    ui/various_dialogs/DataEditDialog.ui \
    ui/various_dialogs/KeysSelectorDialog.ui \
    ui/editselectedactiondialogs/KeysSequenceSelectedEditDialog.ui \
    ui/editselectedactiondialogs/SentenceSelectedEditDialog.ui \
    ui/various_dialogs/StartupTaskEditDialog.ui \
    ui/various_dialogs/StartupTasksDialog.ui \
    ui/createactiondialogs/CreateCursorMovementsActionDialog.ui \
    ui/createactiondialogs/CreateKeysSequenceActionDialog.ui \
    ui/createactiondialogs/CreatePasteActionDialog.ui \
    ui/createactiondialogs/CreateSystemCommandActionDialog.ui \
    ui/createactiondialogs/CreateWaitActionDialog.ui \
    ui/getdialogs/getAutoRenameOptionDialog.ui \
    ui/getdialogs/getDelayDialog.ui \
    ui/getdialogs/getFilePathDialog.ui \
    ui/getdialogs/getFolderPathDialog.ui \
    ui/getdialogs/getImagePathDialog.ui \
    ui/getdialogs/getProgramPathDialog.ui

LIBS += -luser32 -lole32 -luuid

TRANSLATIONS = ScheduledPCTasks_fr.ts

RESOURCES += \
    resources.qrc

RC_ICONS = img/programIcon.ico

VERSION = 1.4.1
QMAKE_TARGET_COMPANY = "Amir Hammoutene"
QMAKE_TARGET_COPYRIGHT = "GNU general public license version 3"

DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\" \
           APP_COMPANY=\"\\\"$${QMAKE_TARGET_COMPANY}\\\"\" \
           APP_COPYRIGHT=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\"
