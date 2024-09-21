#ifndef CURSORMOVEMENTSSELECTEDEDITDIALOG_H
#define CURSORMOVEMENTSSELECTEDEDITDIALOG_H

#include "actions/ActionParameters.h"
#include <QDialog>

namespace Ui {
class CursorMovementsSelectedEditDialog;
}

class CursorMovementsSelectedEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CursorMovementsSelectedEditDialog(QWidget *parent = nullptr);
    ~CursorMovementsSelectedEditDialog();
    void setEditable(bool id);
    void setIdentity(const QString & id);
    QString identity();
    void setTableCursorMovements(const CursorMovementsList &tableContent);
    CursorMovementsList tableCursorMovements();
public slots:
    QList<QWidget*> addMovsRow();
    void removeLastMovsRow();
private:
    Ui::CursorMovementsSelectedEditDialog *ui;
};

#endif // CURSORMOVEMENTSSELECTEDEDITDIALOG_H
