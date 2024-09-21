#ifndef CREATECURSORMOVEMENTSACTIONDIALOG_H
#define CREATECURSORMOVEMENTSACTIONDIALOG_H

#include <QDialog>

namespace Ui {
class CreateCursorMovementsActionDialog;
}

class CreateCursorMovementsActionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateCursorMovementsActionDialog(QWidget *parent = nullptr);
    ~CreateCursorMovementsActionDialog();
public slots:
    void showDialog();
private slots:
    void accept() override;
signals:
    void sendCursorMovements(QString cursorMovsIdentity);
private:
    Ui::CreateCursorMovementsActionDialog *ui;
};

#endif // CREATECURSORMOVEMENTSACTIONDIALOG_H
