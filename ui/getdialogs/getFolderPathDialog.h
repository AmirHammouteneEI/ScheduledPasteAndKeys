#ifndef GETFOLDERPATHDIALOG_H
#define GETFOLDERPATHDIALOG_H

#include <QDialog>

namespace Ui {
class getFolderPathDialog;
}

class getFolderPathDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getFolderPathDialog(QWidget *parent = nullptr);
    ~getFolderPathDialog();
private slots:
    void showBrowseFoldersDialog();
    void accept() override;
public slots:
    void showDialog();
signals:
    void sendDirectory(QString dir);
private:
    Ui::getFolderPathDialog *ui;
};

#endif // GETFOLDERPATHDIALOG_H
