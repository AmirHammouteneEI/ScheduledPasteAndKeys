#include "SystemCommandsAction.h"
#include "actions/ActionsTools.h"

SystemCommandsAction::SystemCommandsAction() : AbstractAction()
{
    m_type = ActionType::SystemCommands;
}

void SystemCommandsAction::runAction() const
{
    auto it = m_commands.begin();
    while(it != m_commands.end())
    {
        executeCommand(it->first,it->second);
        Sleep(100);
    }

}

void SystemCommandsAction::setParameters(const ActionParameters &param)
{
    m_commands = param.m_commands;
    m_sysCmdsId = param.m_dataId;
}

SystemCommandsAction *SystemCommandsAction::deepCopy() const
{
    SystemCommandsAction *actToReturn = new SystemCommandsAction();
    actToReturn->m_commands = m_commands;
    actToReturn->m_sysCmdsId = m_sysCmdsId;
    actToReturn->m_refID = m_ID;
    return actToReturn;
}

ActionParameters SystemCommandsAction::generateParameters() const
{
    ActionParameters param;
    param.m_commands = m_commands;
    param.m_dataId = m_sysCmdsId;
    return param;
}

void SystemCommandsAction::executeCommand(const QString &cmd, const QString &option) const
{

}
