#ifndef CURSORMOVEMENTSTABLEWIDGET_H
#define CURSORMOVEMENTSTABLEWIDGET_H

#include "ui/editselectedactiondialogs/CursorMovementsSelectedEditDialog.h"
#include "ui/CustomPrimaryWidgets.h"

class CursorMovementsTableWidget : public NoFocusCellTableWidget
{
    Q_OBJECT
    CursorMovementsSelectedEditDialog *m_cursorMovementsEditDialog;
public:
    explicit CursorMovementsTableWidget(QWidget *parent = nullptr);
    ~CursorMovementsTableWidget() = default;
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
