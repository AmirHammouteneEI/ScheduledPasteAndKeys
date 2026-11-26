#include "Task.h"

Task::Task()
{}

Task::~Task()
{
    m_actionsMap.clear();
    m_actionsOrderedList.clear();
}