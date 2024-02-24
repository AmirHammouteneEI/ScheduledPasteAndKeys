#ifndef TASK_H
#define TASK_H

#include <QMap>
#include "actions/AbstractAction.h"

class Task
{
public:
    explicit Task();
    ~Task();
    QMap<unsigned int, AbstractAction*> m_actions;
    int appendAction(AbstractAction *act);
};

#endif // TASK_H
