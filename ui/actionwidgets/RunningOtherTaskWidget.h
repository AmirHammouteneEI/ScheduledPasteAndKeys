#ifndef RUNNINGOTHERTASKWIDGET_H
#define RUNNINGOTHERTASKWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"
#include "actions/RunningOtherTaskAction.h"
#include "ui/createactiondialogs/CreateRunningOtherTaskActionDialog.h"

class RunningOtherTaskWidget : public AbstractActionWidget
{
    Q_OBJECT
    void changedRunningState() override {};
    CreateRunningOtherTaskActionDialog *m_editRunOtherTaskDialog = nullptr;
public:
    explicit RunningOtherTaskWidget(QWidget *parent = nullptr);
    ~RunningOtherTaskWidget() = default;
    void buildWidget() override;
public slots:
    void runningOtherTaskReceived(QString filename, int delay, int timesOfLoop); // not const & because it's a slot, will make copy evenif
};

#endif // RUNNINGOTHERTASKWIDGET_H
