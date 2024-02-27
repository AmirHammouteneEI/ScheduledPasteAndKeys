#ifndef GETDELAYDIALOG_H
#define GETDELAYDIALOG_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
class getDelayDialog;
}

class getDelayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getDelayDialog(QWidget *parent = nullptr);
    ~getDelayDialog();
    QTime m_savedDelay;
private slots:
    void uncheckDelayGroup();
    void uncheckDateGroup();
    void accept() override;
    void refreshDateDelayDetails(const QDateTime &datet);
public slots:
    void showDialog();
signals:
    void sendDelay(qint64 delayInSeconds);
private:
    Ui::getDelayDialog *ui;
};

#endif // GETDELAYDIALOG_H
