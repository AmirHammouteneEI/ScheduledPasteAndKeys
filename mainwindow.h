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
    static std::shared_ptr<MainWindow> getInstance(QWidget *parent = nullptr);
    MainWindow(MainWindow &other) = delete;
    void operator=(const MainWindow &) = delete;
    ~MainWindow();
    void buildTaskTabsManager();
    QTabWidget * getTabWidget(); // ui container of TaskTabs
    QShortcut *m_stopAllTasksShortcut = nullptr;
public slots:
    void showWindow(QSystemTrayIcon::ActivationReason reason = QSystemTrayIcon::Trigger); // show mainwindow or systemtray icon menu (if right click on icon)
    void quitApp(); // stops all running tasks (forced), asks to saved modified tasks and quit the program
    void forceQuit();
    void autoRun(const QString & filename, int delay, int loopTimes = 1); // automatically run a task in silent mode (no window appears)
private slots:
    void swapStayOnTop(bool state);
    void swapAutoscrollMode(bool state); // autoscroll force the action widget visible when running the action
    void switchTheme(); // set stylesheet
    void taskTabPageClicked(int newIndex);
    void taskTabContextMenuRequest(QPoint point);
    void createAutorunShortcutFromDialogReceived();
protected :
    MainWindow(QWidget *parent = nullptr);
    struct SharedPtrMainWindow;
    static std::shared_ptr<MainWindow> s_singleton;
    QSystemTrayIcon* m_sticon;
    void setTheme(); // simulate trigger on the appropriate theme button
    void buildToolBar();
    void buildSystemTrayMenu();
    void closeEvent(QCloseEvent *event) override;
    int m_windowWidth = 510;
    int m_windowHeight = 800;
    void geometrySet();
    void loadSettings();
    void saveSettings();
    QAction *m_stayOnTopAction = nullptr;
    QAction * m_lightThemeAction = nullptr;
    QAction * m_penombraThemeAction = nullptr;
    QAction * m_darkThemeAction = nullptr;
    QAction * m_scrollAction = nullptr;
    QString m_currentThemeName = nullptr;
    CreateLoadTaskDialog *m_createLoadTaskDialog = nullptr;
    DataEditDialog *m_dataEditDialog = nullptr;
    StartupTasksDialog *m_startupTasksDialog = nullptr;
    CreateAutorunDesktopShortcutDialog *m_createDesktopShortcutDialog = nullptr;
    std::shared_ptr<TaskTabsManager> m_tasktabsManager = nullptr;
    QMenu *m_stmenu = nullptr;
    QToolBar *m_toolBar = nullptr;
    bool m_stMessageShown = false;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
