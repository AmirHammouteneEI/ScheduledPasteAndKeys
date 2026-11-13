#ifndef TASKHOTKEY_H
#define TASKHOTKEY_H

#include <QString>
#include <QKeySequence>
#include <qt_windows.h>

enum class TaskHotkeyType{
    NormalTask,
    StopAllTasks
};

class TaskHotKey
{
    static unsigned int s_idCounter;
    unsigned int m_ID = 0;

    QString m_taskname;
    qint64 m_delay;
    int m_loopTimes;
    QKeySequence m_keySeq;
    TaskHotkeyType e_type;
public:
    TaskHotKey();
    virtual ~TaskHotKey() = default;
    TaskHotKey(const TaskHotKey & other) = delete;
    TaskHotKey& operator=(const TaskHotKey & other) = delete;
    unsigned int getID() const {return m_ID;}
    TaskHotkeyType getType() const {return e_type;}

    QString getTaskname() const {return m_taskname;}
    qint64 getDelay() const {return m_delay;}
    int getLooptimes() const {return m_loopTimes;}
    QKeySequence getKeySeq() const {return m_keySeq;}

    void setAllAttributes(const QKeySequence& keySeq, const QString &taskname, qint64 delay, int looptimes);

    void setAsStopAllTasksType() {e_type = TaskHotkeyType::StopAllTasks;}
};

#endif // TASKHOTKEY_H
