#ifndef ABSTRACTACTION_H
#define ABSTRACTACTION_H

#include "actions/ActionParameters.h"

enum class ActionType {
    Undefined,
    Paste,
    Wait,
    KeysSequence,
    SystemCommand,
    CursorMovements
};

class AbstractAction
{
    static unsigned int m_idCounter;
protected:
    unsigned int m_ID = 0;
    unsigned int m_refID = 0;
public:
    AbstractAction();
    virtual ~AbstractAction() = default;
    AbstractAction(const AbstractAction & other) = delete;
    AbstractAction& operator=(const AbstractAction & other) = delete;

    virtual void runAction() const = 0;
    virtual void setParameters(const ActionParameters& param) = 0;
    virtual AbstractAction* deepCopy() const = 0;
    virtual ActionParameters generateParameters() const = 0;
    virtual void optionalProcesses() = 0;

    ActionType e_type;
    unsigned int getID() const {return m_ID;}
    unsigned int getRefID() const {return m_refID;}
};

#endif // ABSTRACTACTION_H
