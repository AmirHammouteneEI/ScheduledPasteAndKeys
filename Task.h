#ifndef TASK_H
#define TASK_H

#include "actions/AbstractAction.h"

#include <QMap>
#include <QList>

struct Task
{
    QMap<unsigned int, std::shared_ptr<AbstractAction>> m_actionsMap;
    QList<std::shared_ptr<AbstractAction>> m_actionsOrderedList;
    explicit Task();
    ~Task();
};

#endif // TASK_H
