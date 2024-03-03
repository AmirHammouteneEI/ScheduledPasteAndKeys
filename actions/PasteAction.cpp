#include "PasteAction.h"
#include "ActionsTools.h"

#include <QApplication>

PasteAction::PasteAction() : AbstractAction()
{
    m_type = ActionType::Paste;
}

void PasteAction::runAction()
{
    ActionsTools::setClipboard(m_content);
    ActionsTools::pasteKeystrokeSimulate();
}

void PasteAction::setParameters(const ActionParameters &param)
{
    m_content = param.m_pasteContent;
}

PasteAction *PasteAction::deepCopy()
{
    PasteAction *actToReturn = new PasteAction();
    actToReturn->m_content = m_content;
    actToReturn->m_refID = m_ID;
    return actToReturn;
}
