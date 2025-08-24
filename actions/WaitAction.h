#ifndef WAITACTION_H
#define WAITACTION_H

#include "AbstractAction.h"

class WaitAction : public AbstractAction
{
    Q_OBJECT
private:
    long double m_duration; // in seconds
public:
    WaitAction();
    ~WaitAction() = default;
    WaitAction(const WaitAction & other) = delete;
    WaitAction& operator=(const WaitAction & other) = delete;

    void runAction() override;
    void setParameters(const ActionParameters& param) override;
    std::shared_ptr<AbstractAction> deepCopy() const override;
    ActionParameters generateParameters() const override;
    void optionalProcesses() override {}

    friend class WaitWidget;
};

#endif // WAITACTION_H
