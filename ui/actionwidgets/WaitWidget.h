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
    CreateWaitActionDialog *m_editDurationDialog = nullptr;
public:
    explicit WaitWidget(QWidget *parent = nullptr);
    ~WaitWidget() = default;
    void buildWidget() override;
public slots:
    void durationReceived(long double dur);
private slots:
    void refreshTimeRemainingText(const QDateTime& departureDate);
signals:
};

#endif // WAITWIDGET_H
