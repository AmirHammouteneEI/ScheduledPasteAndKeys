#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include "Task.h"

#include <QThread>

class TaskThread : public QThread
{
    Q_OBJECT
    QList<AbstractAction*> m_actionsList;
public:
    explicit TaskThread(QObject *parent = nullptr);
    ~TaskThread();
private:
    void copyActionsList(Task *task);
    bool m_loop = false;
    unsigned int m_timesToRun = 1;
    bool m_haveToStop = false;
public slots:
    void stop();
protected:
    void run() override;

signals:
    void sendRunningStateAct(unsigned int actId);
    void sendDoneStateAct(unsigned int actId);
    void sendFinishedOneLoop();

    friend class TaskTab;
};

#endif // TASKTHREAD_H
