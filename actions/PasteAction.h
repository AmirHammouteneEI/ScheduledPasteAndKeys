#ifndef PASTEACTION_H
#define PASTEACTION_H

#include "AbstractAction.h"

class PasteAction : public AbstractAction
{
private:
    QString m_content;
public:
    PasteAction();

    void runAction();
    void setParameters(const ActionParameters& param);

};

#endif // PASTEACTION_H