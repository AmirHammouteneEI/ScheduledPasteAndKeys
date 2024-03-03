#ifndef TASK_H
#define TASK_H

#include <QMap>
#include <QList>
#include "actions/AbstractAction.h"

class Task
{
    QMap<unsigned int, AbstractAction*> m_actionsMap;
    QList<AbstractAction*> m_actionsOrderedList;
public:
    explicit Task();
    ~Task();
    int appendAction(AbstractAction *act);

    friend class TaskThread;
    friend class TaskTab;
};

#endif // TASK_H
