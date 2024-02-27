#ifndef WAITACTION_H
#define WAITACTION_H

#include "AbstractAction.h"

class WaitAction : public AbstractAction
{
private:
    float m_duration;
public:
    WaitAction();

    void runAction();
    void setParameters(const ActionParameters& param);
};

#endif // WAITACTION_H
