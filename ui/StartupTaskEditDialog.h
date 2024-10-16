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
    void showDialog();
private slots:
    void accept() override;
private:
    Ui::StartupTaskEditDialog *ui;
    void fillExistingTasksTable();
};

#endif // STARTUPTASKEDITDIALOG_H
