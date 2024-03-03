#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"

#include <QLabel>
#include <QDateTime>

class WaitWidget : public AbstractActionWidget
{
    Q_OBJECT
    void changedRunningState() override;
    QLabel *m_timeRemainingLabel;
public:
    explicit WaitWidget(QWidget *parent = nullptr);
    ~WaitWidget() = default;
    void buildWidget() override;
private slots:
    void refreshTimeRemainingText(const QDateTime& departureDate);
signals:
};

#endif // WAITWIDGET_H
