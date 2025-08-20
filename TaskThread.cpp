#include "TaskThread.h"

TaskThread::TaskThread(QObject *parent)
    : QThread{parent}
{

}

TaskThread::~TaskThread()
{
    qDeleteAll(m_actionsList);
    quit();
    requestInterruption();
    wait();
}

void TaskThread::copyActionsList(const std::shared_ptr<Task> &task)
{
    qDeleteAll(m_actionsList);
    m_actionsList.clear();

    if(task == nullptr)
        return;

    for(auto it = task->m_actionsOrderedList.begin(); it != task->m_actionsOrderedList.end(); ++it)
    {
        if((*it) == nullptr)
            continue;
        m_actionsList.append((*it)->deepCopy());
    }
}

void TaskThread::stop()
{
    m_haveToStop = true;
    //quit();
    terminate();
}

void TaskThread::run()
{
    begin:

    for(auto it = m_actionsList.begin(); it != m_actionsList.end(); ++it)
    {
        if(m_haveToStop)
            return;
        if((*it) == nullptr)
            continue;
        emit sendRunningStateAct((*it)->getRefID());

        (*it)->runAction();

        if(m_haveToStop)
            return;
        if((*it) == nullptr)
            continue;
        emit sendDoneStateAct((*it)->getRefID());
    }

    --m_timesToRun;
    if(m_loop || m_timesToRun > 0)
    {
        emit sendFinishedOneLoop();
        goto begin;
    }

    emit sendFinishedOneLoop();
    emit sendFinishedAllLoops();
}
