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
    static unsigned int m_idCounter;
    unsigned int m_ID;
public:
    AbstractAction();

    virtual void runAction() = 0;
    virtual void setParameters(const ActionParameters& param) = 0;

    ActionType m_type;
    unsigned int getID() const {return m_ID;}
};

#endif // ABSTRACTACTION_H
