#ifndef CURSORMOVEMENTSSELECTEDEDITDIALOG_H
#define CURSORMOVEMENTSSELECTEDEDITDIALOG_H

#include "actions/ActionParameters.h"
#include <QDialog>
#include "ui/getdialogs/getCursorCoordinatesWidget.h"
#include "ui/various_dialogs/KeysSelectorDialog.h"

namespace Ui {
class CursorMovementsSelectedEditDialog;
}

class CursorMovementsSelectedEditDialog : public QDialog
{
    Q_OBJECT
    getCursorCoordinatesWidget *m_getCursorCoordinatesWidget;
    KeysSelectorDialog *m_keySelectorDialog;
public:
    explicit CursorMovementsSelectedEditDialog(QWidget *parent = nullptr);
    ~CursorMovementsSelectedEditDialog();
    void setEditable(bool id);
    void setIdentity(const QString & id);
    QString identity();
    void setTableCursorMovements(const CursorMovementsList &tableContent);
    CursorMovementsList tableCursorMovements();
    void setOptionalKeysStroke(const QStringList &keysStroke);
    QStringList optionalKeysStroke();
public slots:
    QList<QWidget*> addMovsRow();
    void removeLastMovsRow();
    void coordinatesReceived(int index, int x, int y);
private slots:
    void accept() override;
private:
    Ui::CursorMovementsSelectedEditDialog *ui;
};

#endif // CURSORMOVEMENTSSELECTEDEDITDIALOG_H
