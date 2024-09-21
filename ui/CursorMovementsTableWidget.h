#ifndef CURSORMOVEMENTSTABLEWIDGET_H
#define CURSORMOVEMENTSTABLEWIDGET_H

#include <QTableWidget>
#include "ui/CursorMovementsSelectedEditDialog.h"

class CursorMovementsTableWidget : public QTableWidget
{
    Q_OBJECT
    CursorMovementsSelectedEditDialog *m_cursorMovementsEditDialog;
public:
    explicit CursorMovementsTableWidget(QWidget *parent = nullptr);
    void refresh();
    void selectCursorMovementsFromIdentity(const QString & id);
    bool m_belongsToDataEditDialog = false;
public slots:
    void createCursorMovementsReceived();
    void removeCursorMovementsReceived();
    void editFromDialogReceived();
private slots:
    void editCursorMovementsSelected(int row, int);
};

#endif // CURSORMOVEMENTSTABLEWIDGET_H
