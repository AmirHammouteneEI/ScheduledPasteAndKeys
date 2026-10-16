#ifndef GETFORCEOPTIONDIALOG_H
#define GETFORCEOPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class getForceOptionDialog;
}

class getForceOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getForceOptionDialog(QWidget *parent = nullptr);
    ~getForceOptionDialog();
private slots:
    void accept() override;
public slots:
    void showDialog();
signals:
    void sendForce(bool force);
private:
    Ui::getForceOptionDialog *ui;
};

#endif // GETFORCEOPTIONDIALOG_H
