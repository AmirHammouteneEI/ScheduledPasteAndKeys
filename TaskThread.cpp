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
        emit taskFinished();

    for(auto it = m_task->m_actions.keyValueBegin(); it != m_task->m_actions.keyValueEnd(); ++it)
    {
        it->second->runAction();
        if(m_haveToStop == true)
            return;
    }

    emit taskFinished();
}
