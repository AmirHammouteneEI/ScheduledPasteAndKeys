#ifndef WAITACTION_H
#define WAITACTION_H

#include "AbstractAction.h"

class WaitAction : public AbstractAction
{
private:
    float m_duration; // in seconds
public:
    WaitAction();
    ~WaitAction() = default;

    void runAction() override;
    void setParameters(const ActionParameters& param) override;
    WaitAction* deepCopy() override;

    friend class WaitWidget;
};

#endif // WAITACTION_H
