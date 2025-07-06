#ifndef CURSORMOVEMENTSWIDGET_H
#define CURSORMOVEMENTSWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"
#include "ui/createactiondialog/CreateCursorMovementsActionDialog.h"
#include <QDateTime>

class CursorMovementsWidget : public AbstractActionWidget
{
    Q_OBJECT
    CreateCursorMovementsActionDialog *m_createCursorMovsActionDialog;
    NoWheelFocusSpinBox *m_loopSpin;
    void changedRunningState() override;
public:
    explicit CursorMovementsWidget(QWidget *parent = nullptr);
    ~CursorMovementsWidget() = default;
    void buildWidget() override;
private slots:
    void cursorMovsIdentityReceived(QString id); // not const & because it's a slot, will make copy evenif
    void timesToRunChanged(int times);
private:
    void refreshLoopsRemainingText(const QDateTime& departureDate);
};

#endif // CURSORMOVEMENTSWIDGET_H
