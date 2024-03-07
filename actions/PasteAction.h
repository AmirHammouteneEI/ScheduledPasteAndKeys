#ifndef PASTEACTION_H
#define PASTEACTION_H

#include "AbstractAction.h"

class PasteAction : public AbstractAction
{
private:
    QString m_content;
    QString m_contentId;
public:
    PasteAction();
    ~PasteAction() = default;
    PasteAction(const PasteAction & other) = delete;
    PasteAction& operator=(const PasteAction & other) = delete;

    void runAction() const override;
    void setParameters(const ActionParameters& param) override;
    PasteAction* deepCopy() const override;

    friend class PasteWidget;
};

#endif // PASTEACTION_H
