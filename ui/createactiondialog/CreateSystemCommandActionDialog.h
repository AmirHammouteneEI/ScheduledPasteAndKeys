#ifndef CREATESYSTEMCOMMANDACTIONDIALOG_H
#define CREATESYSTEMCOMMANDACTIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include "ui/getFolderPathDialog.h"
#include "ui/getFilePathDialog.h"

namespace Ui {
class CreateSystemCommandActionDialog;
}

class CreateSystemCommandActionDialog : public QDialog
{
    Q_OBJECT

    QString m_type;
    QPushButton *m_option1Button;
    QPushButton *m_option2Button;
    getFolderPathDialog *m_folderPathDialog;
    getFilePathDialog *m_filePathDialog;
    getFilePathDialog *m_filePathDialog2;
    void activateButtons();
public:
    explicit CreateSystemCommandActionDialog(QWidget *parent = nullptr);
    ~CreateSystemCommandActionDialog();
private slots:
    void accept() override;
    void commandButtonPushed();
public slots:
    void showDialog();
signals:
    void sendSystemCommand(QString sysCmdType, QString param1, QString param2);
private:
    Ui::CreateSystemCommandActionDialog *ui;
};

#endif // CREATESYSTEMCOMMANDACTIONDIALOG_H
