#ifndef RUNNINGOTHERTASKACTION_H
#define RUNNINGOTHERTASKACTION_H

#include "AbstractAction.h"

class RunningOtherTaskAction : public AbstractAction
{
    Q_OBJECT
private:
    QString m_taskName;
    int m_delay; // in seconds
    int m_timesToRun = 1;
public:
    RunningOtherTaskAction();
    ~RunningOtherTaskAction() = default;
    RunningOtherTaskAction(const RunningOtherTaskAction & other) = delete;
    RunningOtherTaskAction& operator=(const RunningOtherTaskAction & other) = delete;

    void runAction() override;
    void setParameters(const ActionParameters& param) override;
    std::shared_ptr<AbstractAction> deepCopy() const override;
    ActionParameters generateParameters() const override;
    void optionalProcesses() override {}

    friend class RunningOtherTaskWidget;
signals:
    void autoRunTaskRequest(QString filename, int delay, int loopTimes);
};

#endif // RUNNINGOTHERTASKACTION_H
