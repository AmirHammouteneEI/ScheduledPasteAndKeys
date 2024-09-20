#ifndef CURSORMOVEMENTSWIDGET_H
#define CURSORMOVEMENTSWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"
#include <QSpinBox>
#include <QDateTime>

class CursorMovementsWidget : public AbstractActionWidget
{
    Q_OBJECT
    //TODO CreateCursorMovementsActionDialog
    QSpinBox *m_loopSpin;
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
