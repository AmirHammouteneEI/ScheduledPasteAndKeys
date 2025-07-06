#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QShortcut>

#include "ui/CreateLoadTaskDialog.h"
#include "TaskTabsManager.h"
#include "ui/DataEditDialog.h"
#include "ui/StartupTasksDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTabWidget * getTabWidget(); // ui container of TaskTabs
    void autoRun(const QString & filename, int delay, int loopTimes = 1); // automatically run a task in silent mode (no window appears)
    QShortcut *m_stopAllTasksShortcut;
    void forceQuit();
public slots:
    void showWindow(QSystemTrayIcon::ActivationReason reason = QSystemTrayIcon::Trigger); // show mainwindow or systemtray icon menu (if right click on icon)
    void quitApp(); // stops all running tasks (forced), asks to saved modified tasks and quit the program
private slots:
    void swapStayOnTop(bool state);
    void swapAutoscrollMode(bool state); // autoscroll force the action widget visible when running the action
    void switchTheme(); // set stylesheet
    void taskTabPageClicked(int newIndex);
    void taskTabContextMenuRequest(QPoint point);
protected :
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
    QAction *m_stayOnTopAction;
    QAction * m_lightThemeAction;
    QAction * m_penombraThemeAction;
    QAction * m_darkThemeAction;
    QAction * m_scrollAction;
    QString m_currentThemeName;
    CreateLoadTaskDialog *m_createLoadTaskDialog;
    DataEditDialog *m_dataEditDialog;
    StartupTasksDialog *m_startupTasksDialog;
    TaskTabsManager* m_tasktabsManager;
    QMenu *m_stmenu;
    QToolBar *m_toolBar;
    bool m_stMessageShown = false;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
