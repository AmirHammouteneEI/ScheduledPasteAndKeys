#ifndef STARTUPTASKSDIALOG_H
#define STARTUPTASKSDIALOG_H

#include <QDialog>

namespace Ui {
class StartupTasksDialog;
}

class StartupTasksDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartupTasksDialog(QWidget *parent = nullptr);
    ~StartupTasksDialog();
public slots:
    void showDialog();
private:
    Ui::StartupTasksDialog *ui;
};

#endif // STARTUPTASKSDIALOG_H
