#include "PasteAction.h"
#include "ActionsTools.h"

#include <QApplication>

PasteAction::PasteAction() : AbstractAction()
{
    m_type = ActionType::Paste;
}

void PasteAction::runAction() const
{
    ActionsTools::setClipboard(m_content);
    ActionsTools::pasteKeystrokeSimulate();
}

void PasteAction::setParameters(const ActionParameters &param)
{
    m_content = param.m_pasteContent;
    m_contentId = param.m_dataId;
}

PasteAction *PasteAction::deepCopy() const
{
    PasteAction *actToReturn = new PasteAction();
    actToReturn->m_content = m_content;
    actToReturn->m_contentId = m_contentId;
    actToReturn->m_refID = m_ID;
    return actToReturn;
}

ActionParameters PasteAction::generateParameters() const
{
    ActionParameters param;
    param.m_pasteContent = m_content;
    param.m_dataId = m_contentId;
    return param;
}
