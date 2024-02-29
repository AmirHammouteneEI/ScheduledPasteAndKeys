#include "TaskThread.h"

TaskThread::TaskThread(QObject *parent)
    : QThread{parent}
{

}

void TaskThread::stop()
{
    m_haveToStop = true;
    quit();
}

void TaskThread::run()
{
    if(m_task == nullptr)
        return;

    begin:

    for(auto it = m_task->m_actions.keyValueBegin(); it != m_task->m_actions.keyValueEnd(); ++it)
    {
        emit sendRunningStateAct(it->second->getID());
        it->second->runAction();
        emit sendDoneStateAct(it->second->getID());

        if(m_haveToStop == true)
            return;
    }

    if(m_loop)
    {
        emit sendFinishedOneLoop();
        goto begin;
    }
}
