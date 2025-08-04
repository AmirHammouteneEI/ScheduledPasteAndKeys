#ifndef CREATEKEYSSEQUENCEACTIONDIALOG_H
#define CREATEKEYSSEQUENCEACTIONDIALOG_H

#include <QDialog>

namespace Ui {
class CreateKeysSequenceActionDialog;
}

class CreateKeysSequenceActionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateKeysSequenceActionDialog(QWidget *parent = nullptr);
    ~CreateKeysSequenceActionDialog();
public slots:
    void showDialog();
private slots:
    void accept() override;
signals:
    void sendKeysSequence(QString keysSeqIdentity);
private:
    Ui::CreateKeysSequenceActionDialog *ui;
};

#endif // CREATEKEYSSEQUENCEACTIONDIALOG_H
