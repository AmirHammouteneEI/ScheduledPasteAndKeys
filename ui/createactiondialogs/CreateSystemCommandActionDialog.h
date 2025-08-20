#ifndef CREATESYSTEMCOMMANDACTIONDIALOG_H
#define CREATESYSTEMCOMMANDACTIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include "ui/getdialogs/getProgramPathDialog.h"
#include "ui/getdialogs/getFilePathDialog.h"
#include "ui/getdialogs/getFolderPathDialog.h"
#include "ui/getdialogs/getImagePathDialog.h"
#include "ui/getdialogs/getAutoRenameOptionDialog.h"

namespace Ui {
class CreateSystemCommandActionDialog;
}

class CreateSystemCommandActionDialog : public QDialog
{
    Q_OBJECT

    QString m_type;
    QPushButton *m_option1Button = nullptr;
    QPushButton *m_option2Button = nullptr;
    getProgramPathDialog *m_programPathDialog = nullptr;
    getFilePathDialog *m_filePathDialog = nullptr;
    getFolderPathDialog *m_folderPathDialog = nullptr;
    getFilePathDialog *m_savedFilePathDialog = nullptr;
    getImagePathDialog *m_imagePathDialog = nullptr;
    getAutoRenameOptionDialog *m_autorenameDialog = nullptr;
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
