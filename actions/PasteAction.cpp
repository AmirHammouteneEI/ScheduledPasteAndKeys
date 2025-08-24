#include "PasteAction.h"
#include "ActionsTools.h"

#include <QApplication>

PasteAction::PasteAction() : AbstractAction()
{
    e_type = ActionType::Paste;
}

void PasteAction::runAction()
{
    int timesToRun = m_timesToRun;

    begin:

    ActionsTools::setClipboard(m_content);
    ActionsTools::pasteKeystrokeSimulate();

    --timesToRun;
    if(timesToRun > 0)
        goto begin;
}

void PasteAction::setParameters(const ActionParameters &param)
{
    m_content = param.m_pasteContent;
    m_contentId = param.m_dataId;
    m_timesToRun = param.m_timesToRun;
}

std::shared_ptr<AbstractAction> PasteAction::deepCopy() const
{
    auto actToReturn = std::make_shared<PasteAction>();
    actToReturn->m_content = m_content;
    actToReturn->m_contentId = m_contentId;
    actToReturn->m_timesToRun = m_timesToRun;
    actToReturn->m_refID = m_ID;
    return actToReturn;
}

ActionParameters PasteAction::generateParameters() const
{
    ActionParameters param;
    param.m_pasteContent = m_content;
    param.m_dataId = m_contentId;
    param.m_timesToRun = m_timesToRun;
    return param;
}

int PasteAction::computeOneExecutionDuration()
{
    return 200;// 0.2 second as the minimal time wait for each execution
}
