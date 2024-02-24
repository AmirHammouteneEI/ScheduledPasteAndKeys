#ifndef ABSTRACTACTION_H
#define ABSTRACTACTION_H

#include "actions/ActionParameters.h"

enum class ActionType {
    Undefined,
    Paste,
    Wait
};

class AbstractAction
{
public:
    AbstractAction();

    virtual void runAction() = 0;
    virtual void setParameters(const ActionParameters& param) = 0;

    ActionType m_type;
};

#endif // ABSTRACTACTION_H
