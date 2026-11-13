#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QShortcut>

#include "ui/various_dialogs/CreateLoadTaskDialog.h"
#include "TaskTabsManager.h"
#include "ui/various_dialogs/DataEditDialog.h"
#include "ui/various_dialogs/StartupTasksDialog.h"
#include "ui/various_dialogs/CreateAutorunDesktopShortcutDialog.h"
#include "ui/various_dialogs/GlobalHotKeysDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// SINGLETON
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* getInstance(QWidget *parent = nullptr);
    MainWindow(MainWindow &other) = delete;
    void operator=(const MainWindow &) = delete;
    ~MainWindow();
    static void destroyInstance();
    void buildTaskTabsManager();
    QTabWidget * getTabWidget(); // ui container of TaskTabs
public slots:
    void showWindow(QSystemTrayIcon::ActivationReason reason = QSystemTrayIcon::Trigger); // show mainwindow or systemtray icon menu (if right click on icon)
    void quitApp(); // stops all running tasks (forced), asks to saved modified tasks and quit the program
    void forceQuit();
    void autoRun(const QString & filename, int delay, int loopTimes = 1); // automatically run a task in silent mode (no window appears)
    void stopAllTasks();
private slots:
    void swapStayOnTop(bool state);
    void swapAutoscrollMode(bool state); // autoscroll force the action widget visible when running the action
    void swapListenHotkeysMode(bool state);
    void switchTheme(); // set stylesheet
    void taskTabPageClicked(int newIndex);
    void taskTabContextMenuRequest(QPoint point);// not const & because it's a slot, will make copy anyway
    void createAutorunShortcutFromDialogReceived();
    void openReadmeFile(QString link);// not const & because it's a slot, will make copy anyway
protected :
    MainWindow(QWidget *parent = nullptr);
    static MainWindow* s_singleton;
    QSystemTrayIcon* m_sticon;
    void setTheme(); // simulate trigger on the appropriate theme button
    void buildToolBar();
    void buildSystemTrayMenu();
    void closeEvent(QCloseEvent *event) override;
    int m_windowWidth = 540;
    int m_windowHeight = 800;
    void geometrySet();
    void loadSettings();
    void saveSettings();
    QAction *m_stayOnTopAction = nullptr;
    QAction * m_lightThemeAction = nullptr;
    QAction * m_darkThemeAction = nullptr;
    QAction * m_scrollAction = nullptr;
    QAction * m_listenHotkeysAction = nullptr;
    QString m_currentThemeName = nullptr;
    CreateLoadTaskDialog *m_createLoadTaskDialog = nullptr;
    DataEditDialog *m_dataEditDialog = nullptr;
    StartupTasksDialog *m_startupTasksDialog = nullptr;
    CreateAutorunDesktopShortcutDialog *m_createDesktopShortcutDialog = nullptr;
    GlobalHotKeysDialog *m_globalHorkeysDialog = nullptr;
    std::shared_ptr<TaskTabsManager> m_tasktabsManager = nullptr;
    QMenu *m_stmenu = nullptr;
    QToolBar *m_toolBar = nullptr;
    bool m_stMessageShown = false;
    bool m_dialogSaveChangesShown = false;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
