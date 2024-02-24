#include "Task.h"

Task::Task()
{}

Task::~Task()
{
    qDeleteAll(m_actions);
}

int Task::appendAction(AbstractAction *act)
{
    if(act == nullptr)
        return -1;
    int pos = m_actions.size();
    m_actions.insert(pos, act);
    return pos;
}
