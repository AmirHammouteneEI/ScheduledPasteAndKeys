#ifndef TASK_H
#define TASK_H

#include "actions/AbstractAction.h"

#include <QMap>
#include <QList>

class Task
{
    QMap<unsigned int, std::shared_ptr<AbstractAction>> m_actionsMap;
    QList<std::shared_ptr<AbstractAction>> m_actionsOrderedList;
public:
    explicit Task();
    ~Task();
    int appendAction(const std::shared_ptr<AbstractAction> &act);

    friend class TaskThread;
    friend class TaskTab;
    friend class TaskTabsManager;
};

#endif // TASK_H
