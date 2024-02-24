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
