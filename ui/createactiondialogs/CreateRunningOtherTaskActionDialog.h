#ifndef CREATERUNNINGOTHERTASKACTIONDIALOG_H
#define CREATERUNNINGOTHERTASKACTIONDIALOG_H

#include <QDialog>

namespace Ui {
class CreateRunningOtherTaskActionDialog;
}

class CreateRunningOtherTaskActionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateRunningOtherTaskActionDialog(QWidget *parent = nullptr);
    ~CreateRunningOtherTaskActionDialog();
    QString m_filename;
    qint64 m_delay;
    int m_loopTimes;
public slots:
    void showDialog();
private slots:
    void accept() override;
    void loopToggled(bool state);
signals:
    void sendRunningOtherTask(QString filename, int delay, int loops);
private:
    Ui::CreateRunningOtherTaskActionDialog *ui;
    void fillExistingTasksTable();
};

#endif // CREATERUNNINGOTHERTASKACTIONDIALOG_H
