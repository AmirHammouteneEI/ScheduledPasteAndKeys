#ifndef GETIMAGEPATHDIALOG_H
#define GETIMAGEPATHDIALOG_H

#include <QDialog>

namespace Ui {
class getImagePathDialog;
}

class getImagePathDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getImagePathDialog(QWidget *parent = nullptr);
    ~getImagePathDialog();
private slots:
    void showBrowseFilesDialog();
    void accept() override;
public slots:
    void showDialog();
signals:
    void sendImage(QString dir);
private:
    Ui::getImagePathDialog *ui;
};

#endif // GETIMAGEPATHDIALOG_H
