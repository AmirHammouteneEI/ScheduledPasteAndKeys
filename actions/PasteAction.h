#ifndef PASTEACTION_H
#define PASTEACTION_H

#include "AbstractAction.h"

class PasteAction : public AbstractAction
{
private:
    QString m_content;
public:
    PasteAction();
    ~PasteAction() = default;

    void runAction() override;
    void setParameters(const ActionParameters& param) override;
    PasteAction* deepCopy() override;

    friend class PasteWidget;
};

#endif // PASTEACTION_H
