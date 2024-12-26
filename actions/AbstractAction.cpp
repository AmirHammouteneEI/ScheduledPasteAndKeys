#include "AbstractAction.h"

unsigned int AbstractAction::s_idCounter = 0;

AbstractAction::AbstractAction()
{
    e_type = ActionType::Undefined;
    m_ID = ++s_idCounter; // No action with id = 0
}
