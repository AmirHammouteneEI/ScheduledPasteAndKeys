#ifndef CURSORMOVEMENTSSELECTEDEDITDIALOG_H
#define CURSORMOVEMENTSSELECTEDEDITDIALOG_H

#include "actions/ActionParameters.h"
#include <QDialog>
#include "ui/getCursorCoordinatesWidget.h"

namespace Ui {
class CursorMovementsSelectedEditDialog;
}

class CursorMovementsSelectedEditDialog : public QDialog
{
    Q_OBJECT
    getCursorCoordinatesWidget *m_getCursorCoordinatesWidget;
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
    void coordinatesReceived(int index, int x, int y);
private:
    Ui::CursorMovementsSelectedEditDialog *ui;
};

#endif // CURSORMOVEMENTSSELECTEDEDITDIALOG_H
