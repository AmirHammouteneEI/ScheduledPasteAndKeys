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

public:
    void renameFilename(const QString &oldFileName, const QString &oldFilePath);
private slots:
    void accept() override;
    void onOpenFilename();
    void onDeleteFilename();
    void onRenameFilename();
    //TODO onDuplicateFilename()
signals:
    void requestOpenNewTab(QString);
    void requestRefreshTabsName();
    void taskfilePathChanged(QString,QString);

private:
    Ui::CreateLoadTaskDialog *ui;
    void fillExistingTasksTable();
    void createTasksFolderIfNotExist();
};

#endif // CREATELOADTASKDIALOG_H
