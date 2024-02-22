#ifndef CREATELOADTASKDIALOG_H
#define CREATELOADTASKDIALOG_H

#include <QDialog>

namespace Ui {
class CreateLoadTaskDialog;
}

class CreateLoadTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateLoadTaskDialog(QWidget *parent = nullptr);
    ~CreateLoadTaskDialog();
    static const QString s_tasksFolder;
    static const QString s_tasksFileExtension;
    void showDialog();

private slots:
    void accept() override;
    void onOpenFilename();
    void onRenameFilename();
    void onDeleteFilename();

signals:
    void requestOpenNewTab(QString);
    void requestRefreshTabs();
    void taskfilePathChanged(QString,QString);

private:
    Ui::CreateLoadTaskDialog *ui;
    void fillExistingTasksTable();
};

#endif // CREATELOADTASKDIALOG_H
