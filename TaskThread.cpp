#include "TaskThread.h"

TaskThread::TaskThread(QObject *parent)
    : QThread{parent}
{

}

void TaskThread::run()
{
    if(m_task == nullptr)
        emit taskFinished();

    for(auto it = m_task->m_actions.keyValueBegin(); it != m_task->m_actions.keyValueEnd(); ++it)
    {
        it->second->runAction();
    }

    emit taskFinished();
}
