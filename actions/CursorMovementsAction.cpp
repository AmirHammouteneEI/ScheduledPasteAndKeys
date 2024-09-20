#include "CursorMovementsAction.h"
#include "actions/ActionsTools.h"

CursorMovementsAction::CursorMovementsAction() : AbstractAction()
{
    e_type = ActionType::CursorMovements;
}

void CursorMovementsAction::runAction() const
{
    int timesToRun = m_timesToRun;

begin:

    auto it = m_cursorMovementsMap.begin();
    while(it != m_cursorMovementsMap.end())
    {
        //TODO implement to movements action
        ++it;
    }

    Sleep(100);

    --timesToRun;
    if(timesToRun > 0)
        goto begin;
}

void CursorMovementsAction::setParameters(const ActionParameters &param)
{
    m_cursorMovementsMap = param.m_cursorMovementsMap;
    m_movementsId = param.m_dataId;
    m_timesToRun = param.m_timesToRun;
}

CursorMovementsAction *CursorMovementsAction::deepCopy() const
{
    CursorMovementsAction *actToReturn = new CursorMovementsAction();
    actToReturn->m_cursorMovementsMap = m_cursorMovementsMap;
    actToReturn->m_movementsId = m_movementsId;
    actToReturn->m_timesToRun = m_timesToRun;
    actToReturn->m_refID = m_ID;
    return actToReturn;
}

ActionParameters CursorMovementsAction::generateParameters() const
{
    ActionParameters param;
    param.m_cursorMovementsMap = m_cursorMovementsMap;
    param.m_dataId = m_movementsId;
    param.m_timesToRun = m_timesToRun;
    return param;
}

int CursorMovementsAction::computeOneExecutionDuration()
{
    int returnedValue = 0;

    for(auto [key,val] : m_cursorMovementsMap.asKeyValueRange())
        returnedValue += key + val.first;

    return returnedValue+100; // 0.1 second as the minimal time wait for each execution

}
