#ifndef CREATEPASTEACTIONDIALOG_H
#define CREATEPASTEACTIONDIALOG_H

#include <QDialog>

namespace Ui {
class CreatePasteActionDialog;
}

class CreatePasteActionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePasteActionDialog(QWidget *parent = nullptr);
    ~CreatePasteActionDialog();
public slots:
    void showDialog();
    void accept() override;
signals:
    void createPasteActionRequest(QString sentenceIdentity, float addWaitActionSeconds);

private:
    Ui::CreatePasteActionDialog *ui;
};

#endif // CREATEPASTEACTIONDIALOG_H
