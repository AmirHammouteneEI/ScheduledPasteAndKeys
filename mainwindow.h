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
    void quitApp();
    void showWindow(QSystemTrayIcon::ActivationReason reason = QSystemTrayIcon::Trigger);
protected :
    QSystemTrayIcon* m_sticon;
    void buildSystemTrayMenu();
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
