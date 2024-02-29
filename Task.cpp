#include "Task.h"

Task::Task()
{}

Task::~Task()
{
    qDeleteAll(m_actions);
}

int Task::appendAction(AbstractAction *act)
{
    if(act != nullptr)
    {
        m_actions.insert(act->getID(), act);
    }
    return m_actions.size();
}
