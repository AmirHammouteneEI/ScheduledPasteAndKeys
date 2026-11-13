#ifndef GLOBALHOTKEYSDIALOG_H
#define GLOBALHOTKEYSDIALOG_H

#include <QDialog>

namespace Ui {
class GlobalHotKeysDialog;
}

class GlobalHotKeysDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GlobalHotKeysDialog(QWidget *parent = nullptr);
    ~GlobalHotKeysDialog();
    void refreshTable();
public slots:
    void showDialog();
private:
    Ui::GlobalHotKeysDialog *ui;
};

#endif // GLOBALHOTKEYSDIALOG_H
