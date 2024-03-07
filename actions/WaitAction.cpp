#include "WaitAction.h"
#include <windows.h>

#include <QThread>

WaitAction::WaitAction()
{
    m_type = ActionType::Wait;
}

void WaitAction::runAction() const
{
    Sleep(m_duration*1000);
    //QThread::msleep((int)(m_duration*1000));
}

void WaitAction::setParameters(const ActionParameters &param)
{
    m_duration = param.m_waitDuration;
}

WaitAction *WaitAction::deepCopy() const
{
    WaitAction *actToReturn = new WaitAction();
    actToReturn->m_duration = m_duration;
    actToReturn->m_refID = m_ID;
    return actToReturn;
}
