#include "CursorMovementsAction.h"
#include "actions/ActionsTools.h"

CursorMovementsAction::CursorMovementsAction() : AbstractAction()
{
    e_type = ActionType::CursorMovements;
}

void CursorMovementsAction::runAction()
{
    int timesToRun = m_timesToRun;

    begin:

    auto it = m_cursorMovementsList.begin();
    while(it != m_cursorMovementsList.end())
    {
        if(it->size() < 4)
            break;
        Sleep(it->at(0));
        ActionsTools::moveCursorSimulate(it->at(2),it->at(3),it->at(1));

        if(it == m_cursorMovementsList.begin())
        {
            for(auto& pressStr : m_cursorMovementsOptionalKeysStroke)
                ActionsTools::keyStokeSimulate(pressStr, 0);
        }

        ++it;
    }

    for(auto& releaseStr : m_cursorMovementsOptionalKeysStroke)
        ActionsTools::keyStokeSimulate(releaseStr, KEYEVENTF_KEYUP);

    --timesToRun;
    if(timesToRun > 0)
        goto begin;
}

void CursorMovementsAction::setParameters(const ActionParameters &param)
{
    m_cursorMovementsList = param.m_cursorMovementsList;
    m_movementsId = param.m_dataId;
    m_timesToRun = param.m_timesToRun;
    m_cursorMovementsOptionalKeysStroke = param.m_cursorMovementsOptionalKeysStroke;
}

std::shared_ptr<AbstractAction> CursorMovementsAction::deepCopy() const
{
    auto actToReturn = std::make_shared<CursorMovementsAction>();
    actToReturn->m_cursorMovementsList = m_cursorMovementsList;
    actToReturn->m_movementsId = m_movementsId;
    actToReturn->m_timesToRun = m_timesToRun;
    actToReturn->m_cursorMovementsOptionalKeysStroke = m_cursorMovementsOptionalKeysStroke;
    actToReturn->m_refID = m_ID;
    return actToReturn;
}

ActionParameters CursorMovementsAction::generateParameters() const
{
    ActionParameters param;
    param.m_cursorMovementsList = m_cursorMovementsList;
    param.m_dataId = m_movementsId;
    param.m_timesToRun = m_timesToRun;
    param.m_cursorMovementsOptionalKeysStroke = m_cursorMovementsOptionalKeysStroke;
    return param;
}

int CursorMovementsAction::computeOneExecutionDuration()
{
    int returnedValue = 0;

    for(auto mov : m_cursorMovementsList)
    {
        if(mov.size() >= 2)
            returnedValue += mov[0] + mov[1];
    }

    return returnedValue;

}
