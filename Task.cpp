#include "Task.h"

Task::Task()
{}

Task::~Task()
{
    qDeleteAll(m_actionsMap);
    m_actionsMap.clear();
    m_actionsOrderedList.clear();
}

int Task::appendAction(AbstractAction *act)
{
    if(act != nullptr)
    {
        m_actionsMap.insert(act->getID(), act);
        m_actionsOrderedList.append(act);
    }

    return m_actionsMap.size();
}

