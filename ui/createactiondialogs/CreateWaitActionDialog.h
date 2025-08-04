#ifndef CREATEWAITACTIONDIALOG_H
#define CREATEWAITACTIONDIALOG_H

#include <QDialog>

namespace Ui {
class CreateWaitActionDialog;
}

class CreateWaitActionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateWaitActionDialog(QWidget *parent = nullptr);
    ~CreateWaitActionDialog();
private slots:
    void uncheckBySecondsGroup();
    void uncheckByDayBasedGroup();
    void accept() override;
public slots:
    void showDialog();
signals:
    void sendDuration(long double durationInSeconds);
private:
    Ui::CreateWaitActionDialog *ui;
};

#endif // CREATEWAITACTIONDIALOG_H
