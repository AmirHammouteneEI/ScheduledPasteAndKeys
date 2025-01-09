#ifndef GETAUTORENAMEOPTIONDIALOG_H
#define GETAUTORENAMEOPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class getAutoRenameOptionDialog;
}

class getAutoRenameOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getAutoRenameOptionDialog(QWidget *parent = nullptr);
    ~getAutoRenameOptionDialog();
private slots:
    void accept() override;
public slots:
    void showDialog();
signals:
    void sendAutorename(bool autorename);
private:
    Ui::getAutoRenameOptionDialog *ui;
};

#endif // GETAUTORENAMEOPTIONDIALOG_H
