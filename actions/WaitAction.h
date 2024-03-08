#ifndef WAITACTION_H
#define WAITACTION_H

#include "AbstractAction.h"

class WaitAction : public AbstractAction
{
private:
    long double m_duration; // in seconds
public:
    WaitAction();
    ~WaitAction() = default;
    WaitAction(const WaitAction & other) = delete;
    WaitAction& operator=(const WaitAction & other) = delete;

    void runAction() const override;
    void setParameters(const ActionParameters& param) override;
    WaitAction* deepCopy() const override;
    ActionParameters generateParameters() const override;

    friend class WaitWidget;
};

#endif // WAITACTION_H
