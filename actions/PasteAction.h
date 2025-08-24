#ifndef PASTEACTION_H
#define PASTEACTION_H

#include "AbstractAction.h"

class PasteAction : public AbstractAction
{
    Q_OBJECT
private:
    QString m_content;
    QString m_contentId;
    int m_timesToRun = 1;
public:
    PasteAction();
    ~PasteAction() = default;
    PasteAction(const PasteAction & other) = delete;
    PasteAction& operator=(const PasteAction & other) = delete;

    void runAction() override;
    void setParameters(const ActionParameters& param) override;
    std::shared_ptr<AbstractAction> deepCopy() const override;
    ActionParameters generateParameters() const override;
    void optionalProcesses() override {}
    int computeOneExecutionDuration(); // in miliseconds

    friend class PasteWidget;
};

#endif // PASTEACTION_H
