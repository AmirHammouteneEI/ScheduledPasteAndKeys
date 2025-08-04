#ifndef KEYSSELECTORDIALOG_H
#define KEYSSELECTORDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QListWidgetItem>

namespace Ui {
class KeysSelectorDialog;
}

class KeysSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeysSelectorDialog(QWidget *parent = nullptr);
    ~KeysSelectorDialog();
private slots:
    void appendToList(QTreeWidgetItem *item, int);
    void removeFromList(QListWidgetItem* item);
    void accept() override;
public slots:
    void showDialog();
signals:
    void sendKeysList(QStringList keysList);
private:
    Ui::KeysSelectorDialog *ui;
};

#endif // KEYSSELECTORDIALOG_H
