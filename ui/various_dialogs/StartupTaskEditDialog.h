#ifndef STARTUPTASKEDITDIALOG_H
#define STARTUPTASKEDITDIALOG_H

#include <QDialog>

namespace Ui {
class StartupTaskEditDialog;
}

class StartupTaskEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartupTaskEditDialog(QWidget *parent = nullptr);
    ~StartupTaskEditDialog();
    QString m_filename;
    qint64 m_delay;
    int m_loopTimes;
    void showDialog();
private slots:
    void accept() override;
    void loopToggled(bool state);
private:
    Ui::StartupTaskEditDialog *ui;
    void fillExistingTasksTable();
};

#endif // STARTUPTASKEDITDIALOG_H
