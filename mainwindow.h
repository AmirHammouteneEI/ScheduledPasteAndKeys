#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

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
public slots:
    void showWindow(QSystemTrayIcon::ActivationReason reason = QSystemTrayIcon::Trigger);
private slots:
    void quitApp();
    void swapStayOnTop(bool state);
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
    void buildToolBar();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
