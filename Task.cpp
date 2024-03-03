#include "Task.h"

Task::Task()
{}

Task::~Task()
{
    qDeleteAll(m_actionsMap);
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

