#include "WaitAction.h"
#include <windows.h>

#include <QThread>

WaitAction::WaitAction() : AbstractAction()
{
    e_type = ActionType::Wait;
}

void WaitAction::runAction()
{
    Sleep(m_duration*1000);
}

void WaitAction::setParameters(const ActionParameters &param)
{
    m_duration = param.m_waitDuration;
}

std::shared_ptr<AbstractAction> WaitAction::deepCopy() const
{
    auto actToReturn = std::make_shared<WaitAction>();
    actToReturn->m_duration = m_duration;
    actToReturn->m_refID = m_ID;
    return actToReturn;
}

ActionParameters WaitAction::generateParameters() const
{
    ActionParameters param;
    param.m_waitDuration = m_duration;
    return param;
}
