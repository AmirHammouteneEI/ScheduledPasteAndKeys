#ifndef CREATELOADTASKDIALOG_H
#define CREATELOADTASKDIALOG_H

#include <QDialog>

namespace Ui {
class CreateLoadTaskDialog;
}

class CreateLoadTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateLoadTaskDialog(QWidget *parent = nullptr);
    ~CreateLoadTaskDialog();

private:
    Ui::CreateLoadTaskDialog *ui;
};

#endif // CREATELOADTASKDIALOG_H
