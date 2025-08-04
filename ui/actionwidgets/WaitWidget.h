#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"
#include "ui/createactiondialogs/CreateWaitActionDialog.h"

#include <QLabel>
#include <QDateTime>

class WaitWidget : public AbstractActionWidget
{
    Q_OBJECT
    void changedRunningState() override;
    CreateWaitActionDialog *m_editDurationDialog;
public:
    explicit WaitWidget(QWidget *parent = nullptr);
    ~WaitWidget() = default;
    void buildWidget() override;
private slots:
    void refreshTimeRemainingText(const QDateTime& departureDate);
    void durationReceived(long double dur);
signals:
};

#endif // WAITWIDGET_H
