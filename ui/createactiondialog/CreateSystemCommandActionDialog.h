#ifndef CREATESYSTEMCOMMANDACTIONDIALOG_H
#define CREATESYSTEMCOMMANDACTIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include "ui/getProgramPathDialog.h"
#include "ui/getFilePathDialog.h"
#include "ui/getFolderPathDialog.h"
#include "ui/getImagePathDialog.h"
#include "ui/getAutoRenameOptionDialog.h"

namespace Ui {
class CreateSystemCommandActionDialog;
}

class CreateSystemCommandActionDialog : public QDialog
{
    Q_OBJECT

    QString m_type;
    QPushButton *m_option1Button;
    QPushButton *m_option2Button;
    getProgramPathDialog *m_programPathDialog;
    getFilePathDialog *m_filePathDialog;
    getFolderPathDialog *m_folderPathDialog;
    getFilePathDialog *m_savedFilePathDialog;
    getImagePathDialog *m_imagePathDialog;
    getAutoRenameOptionDialog *m_autorenameDialog;
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
