#include "WaitAction.h"

#include <QThread>

WaitAction::WaitAction()
{
    m_type = ActionType::Wait;
}

void WaitAction::runAction()
{
    QThread::sleep(m_duration);
}

void WaitAction::setParameters(const ActionParameters &param)
{
    m_duration = param.m_waitDuration;
}
