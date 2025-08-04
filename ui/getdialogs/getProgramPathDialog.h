#ifndef GETPROGRAMPATHDIALOG_H
#define GETPROGRAMPATHDIALOG_H

#include <QDialog>

namespace Ui {
class getProgramPathDialog;
}

class getProgramPathDialog : public QDialog
{
    Q_OBJECT
public:
    explicit getProgramPathDialog(QWidget *parent = nullptr);
    ~getProgramPathDialog();
private slots:
    void showBrowseFilesDialog();
    void accept() override;
public slots:
    void showDialog();
signals:
    void sendProgram(QString dir);
private:
    Ui::getProgramPathDialog *ui;
};

#endif // GETPROGRAMPATHDIALOG_H
