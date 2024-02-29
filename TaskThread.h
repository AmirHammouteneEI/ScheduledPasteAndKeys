#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include "Task.h"

#include <QThread>

class TaskThread : public QThread
{
    Q_OBJECT
public:
    explicit TaskThread(QObject *parent = nullptr);
    Task *m_task = nullptr;
    bool m_loop = false;
    bool m_haveToStop = false;
public slots:
    void stop();
protected:
    void run() override;

signals:
    void sendRunningStateAct(unsigned int actId);
    void sendDoneStateAct(unsigned int actId);
    void sendFinishedOneLoop();
};

#endif // TASKTHREAD_H
