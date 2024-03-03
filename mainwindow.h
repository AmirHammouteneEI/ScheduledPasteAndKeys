#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "ui/CreateLoadTaskDialog.h"
#include "TaskTabsManager.h"

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
    QTabWidget * getTabWidget();
public slots:
    void showWindow(QSystemTrayIcon::ActivationReason reason = QSystemTrayIcon::Trigger);
private slots:
    void quitApp();
    void swapStayOnTop(bool state);
    void switchTheme();
    void taskTabPageClicked(int newIndex);
    void taskTabContextMenuRequest(QPoint point);
protected :
    QSystemTrayIcon* m_sticon;
    void buildSystemTrayMenu();
    void closeEvent(QCloseEvent *event) override;
    int m_windowWidth = 300;
    int m_windowHeight = 900;
    void geometrySet();
    void loadSettings();
    void saveSettings();
    QAction *m_stayOnTopAction;
    QAction * m_lightThemeAction;
    QAction * m_penombraThemeAction;
    QAction * m_darkThemeAction;
    void buildToolBar();
    QString m_currentThemeName;
    void setTheme();
    CreateLoadTaskDialog *m_createLoadTaskDialog;
    TaskTabsManager* m_tasktabsManager;
    QMenu *m_stmenu;
    QShortcut *m_stopAllTasksShortcut;
    QToolBar *m_toolBar;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
