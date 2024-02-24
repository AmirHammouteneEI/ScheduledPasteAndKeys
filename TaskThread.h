#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include "Task.h"

#include <QThread>

class TaskThread : public QThread
{
    Q_OBJECT
public:
    explicit TaskThread(QObject *parent = nullptr);
    Task *m_task;
    bool m_loop = false;
protected:
    void run() override;

signals:
    void taskFinished();
};

#endif // TASKTHREAD_H
