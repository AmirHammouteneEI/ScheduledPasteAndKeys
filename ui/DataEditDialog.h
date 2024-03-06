#ifndef DATAEDITDIALOG_H
#define DATAEDITDIALOG_H

#include <QDialog>

namespace Ui {
class DataEditDialog;
}

class DataEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataEditDialog(QWidget *parent = nullptr);
    ~DataEditDialog();
public slots:
    void showDialog();

private:
    Ui::DataEditDialog *ui;
};

#endif // DATAEDITDIALOG_H
