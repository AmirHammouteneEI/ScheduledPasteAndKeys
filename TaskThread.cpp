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

    for(auto it = m_task->m_actionsOrderedList.begin(); it != m_task->m_actionsOrderedList.end(); ++it)
    {
        if((*it) == nullptr)
            continue;
        emit sendRunningStateAct((*it)->getID());
        (*it)->runAction();
        emit sendDoneStateAct((*it)->getID());

        if(m_haveToStop == true)
            return;
    }

    if(m_loop)
    {
        emit sendFinishedOneLoop();
        goto begin;
    }
}
