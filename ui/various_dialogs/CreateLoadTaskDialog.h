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
    void showDialog();

public:
    void renameFilename(const QString &oldFileName, const QString &oldFilePath, bool duplicate = false);
private slots:
    void accept() override;
    void onOpenFilename();
    void onDeleteFilename();
    void onRenameFilename();
    void onDuplicateFilename();
signals:
    void requestOpenNewTab(QString);
    void requestRefreshTabs();
    void taskfilePathChanged(QString,QString);

private:
    Ui::CreateLoadTaskDialog *ui;
    void fillExistingTasksTable();
    void createTasksFolderIfNotExist();
    bool doesFilenameContainForbiddenChar(const QString & filename);
};

#endif // CREATELOADTASKDIALOG_H
