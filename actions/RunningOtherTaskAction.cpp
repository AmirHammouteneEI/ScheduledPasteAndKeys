#include "RunningOtherTaskAction.h"

#include "mainwindow.h"

RunningOtherTaskAction::RunningOtherTaskAction() : AbstractAction()
{
    e_type = ActionType::RunningOtherTask;

    connect(this, &RunningOtherTaskAction::autoRunTaskRequest, MainWindow::getInstance().get(), &MainWindow::autoRun);
}

void RunningOtherTaskAction::runAction()
{
    if(!m_taskName.isEmpty() && m_delay >= 0 && m_timesToRun > 0)
        emit autoRunTaskRequest(m_taskName, m_delay, m_timesToRun);
}

void RunningOtherTaskAction::setParameters(const ActionParameters &param)
{
    m_taskName = param.m_taskName;
    m_delay = param.m_delay;
    m_timesToRun = param.m_timesToRun;
}

std::shared_ptr<AbstractAction> RunningOtherTaskAction::deepCopy() const
{
    auto actToReturn = std::make_shared<RunningOtherTaskAction>();
    actToReturn->m_taskName = m_taskName;
    actToReturn->m_delay = m_delay;
    actToReturn->m_timesToRun = m_timesToRun;
    actToReturn->m_refID = m_ID;
    return actToReturn;
}

ActionParameters RunningOtherTaskAction::generateParameters() const
{
    ActionParameters param;
    param.m_taskName = m_taskName;
    param.m_delay = m_delay;
    param.m_timesToRun = m_timesToRun;
    return param;
}
