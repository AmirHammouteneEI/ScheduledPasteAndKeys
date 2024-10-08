#ifndef PASTEACTION_H
#define PASTEACTION_H

#include "AbstractAction.h"

class PasteAction : public AbstractAction
{
private:
    QString m_content;
    QString m_contentId;
    int m_timesToRun = 1;
public:
    PasteAction();
    ~PasteAction() = default;
    PasteAction(const PasteAction & other) = delete;
    PasteAction& operator=(const PasteAction & other) = delete;

    void runAction() const override;
    void setParameters(const ActionParameters& param) override;
    PasteAction* deepCopy() const override;
    ActionParameters generateParameters() const override;
    void optionalProcesses() override {}
    int computeOneExecutionDuration(); // in miliseconds

    friend class PasteWidget;
};

#endif // PASTEACTION_H
