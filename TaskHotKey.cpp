#include "TaskHotKey.h"
#include "globals.h"

unsigned int TaskHotKey::s_idCounter = G_Files::HotKeyRangeStart;

TaskHotKey::TaskHotKey()
{
    m_ID = s_idCounter++;
    e_type = TaskHotkeyType::NormalTask;
}

void TaskHotKey::setAllAttributes(const QKeySequence &keySeq, const QString &taskname, qint64 delay, int looptimes)
{
    m_keySeq = keySeq;
    m_taskname = taskname;
    m_delay = delay;
    m_loopTimes = looptimes;
}