#ifndef KEYSSEQUENCESELECTEDEDITDIALOG_H
#define KEYSSEQUENCESELECTEDEDITDIALOG_H

#include "actions/ActionParameters.h"
#include <QDialog>

namespace Ui {
class KeysSequenceSelectedEditDialog;
}

class KeysSequenceSelectedEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeysSequenceSelectedEditDialog(QWidget *parent = nullptr);
    ~KeysSequenceSelectedEditDialog();
    void setEditable(bool id);
    void setIdentity(const QString & id);
    QString identity();
    void setTableKeysSequence(const PressedReleaseDelaysKeysMap &tableContent);
    PressedReleaseDelaysKeysMap tableKeysSequence();
public slots:
    QList<QWidget*> addKeysRow();
    void removeLastKeysRow();

private:
    Ui::KeysSequenceSelectedEditDialog *ui;
};

#endif // KEYSSEQUENCESELECTEDEDITDIALOG_H
