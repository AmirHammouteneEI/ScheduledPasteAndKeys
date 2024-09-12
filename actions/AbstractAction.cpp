#include "AbstractAction.h"

unsigned int AbstractAction::m_idCounter = 0;

AbstractAction::AbstractAction()
{
    e_type = ActionType::Undefined;
    m_ID = ++m_idCounter; // No action with id = 0
}
