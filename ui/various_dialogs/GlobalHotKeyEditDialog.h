#ifndef GLOBALHOTKEYEDITDIALOG_H
#define GLOBALHOTKEYEDITDIALOG_H

#include <QDialog>

namespace Ui {
class GlobalHotKeyEditDialog;
}

class GlobalHotKeyEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GlobalHotKeyEditDialog(QWidget *parent = nullptr);
    ~GlobalHotKeyEditDialog();
    QString m_filename;
    qint64 m_delay;
    int m_loopTimes;
    QKeySequence m_keySeq;
    void showDialog();
private slots:
    void accept() override;
    void loopToggled(bool state);
private:
    Ui::GlobalHotKeyEditDialog *ui;
    void fillExistingTasksTable();
};

#endif // GLOBALHOTKEYEDITDIALOG_H
