#ifndef GETFILEPATHDIALOG_H
#define GETFILEPATHDIALOG_H

#include <QDialog>

namespace Ui {
class getFilePathDialog;
}

class getFilePathDialog : public QDialog
{
    Q_OBJECT
    bool m_getSavedFileName = false;
public:
    explicit getFilePathDialog(QWidget *parent = nullptr, bool getSaved = false);
    ~getFilePathDialog();
private slots:
    void showBrowseFilesDialog();
    void accept() override;
public slots:
    void showDialog();
signals:
    void sendFile(QString dir);
private:
    Ui::getFilePathDialog *ui;
};

#endif // GETFILEPATHDIALOG_H
