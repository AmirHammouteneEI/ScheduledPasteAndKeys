#ifndef CREATEAUTORUNDESKTOPSHORTCUTDIALOG_H
#define CREATEAUTORUNDESKTOPSHORTCUTDIALOG_H

#include <QDialog>

namespace Ui {
class CreateAutorunDesktopShortcutDialog;
}

class CreateAutorunDesktopShortcutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateAutorunDesktopShortcutDialog(QWidget *parent = nullptr);
    ~CreateAutorunDesktopShortcutDialog();
    QString m_filename;
    qint64 m_delay;
    int m_loopTimes;
    QString m_shortcutname;
    void showDialog();
private slots:
    void accept() override;
    void loopToggled(bool state);
    void onFileSelected();
private:
    Ui::CreateAutorunDesktopShortcutDialog *ui;
    void fillExistingTasksTable();
    bool doesFilenameContainForbiddenChar(const QString & filename);
};

#endif // CREATEAUTORUNDESKTOPSHORTCUTDIALOG_H
