#include "SystemCommandsAction.h"
#include "globals.h"
#include <QDir>

//#include "actions/ActionsTools.h"

SystemCommandAction::SystemCommandAction() : AbstractAction()
{
    e_type = ActionType::SystemCommand;
    e_sysCommandType = SystemCommandType::Undefined;
}

void SystemCommandAction::runAction() const
{
    switch(e_sysCommandType)
    {
        case SystemCommandType::CreateFolder:
        {
            QDir d;
            d.mkpath(m_param1+"/"+m_param2);
        }
        break;
        default:
        break;
    }
}

void SystemCommandAction::setParameters(const ActionParameters &param)
{
    if(param.m_sysCmdTypeStr == G_SystemCommands::ShutDownType)
        e_sysCommandType = SystemCommandType::ShutDown;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::RestartType)
        e_sysCommandType = SystemCommandType::Restart;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::LogOffType)
        e_sysCommandType = SystemCommandType::LogOff;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::PutIntoSleepModeType)
        e_sysCommandType = SystemCommandType::PutIntoSleepMode;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::ChangeAudioVolumeType)
        e_sysCommandType = SystemCommandType::ChangeAudioVolume;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::ChangeDefaultAudioDeviceType)
        e_sysCommandType = SystemCommandType::ChangeDefaultAudioDevice;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::KillProcessType)
        e_sysCommandType = SystemCommandType::KillProcess;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::FocusWindowType)
        e_sysCommandType = SystemCommandType::FocusWindow;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::CreateFolderType)
        e_sysCommandType = SystemCommandType::CreateFolder;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::DeleteFolderType)
        e_sysCommandType = SystemCommandType::DeleteFolder;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::CreateFileType)
        e_sysCommandType = SystemCommandType::CreateFile;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::DeleteFileType)
        e_sysCommandType = SystemCommandType::DeleteFile;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::TakeScreenshotType)
        e_sysCommandType = SystemCommandType::TakeScreenshot;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::PrintActualScreenType)
        e_sysCommandType = SystemCommandType::PrintActualScreen;
    else
        e_sysCommandType = SystemCommandType::Undefined;

    m_param1 = param.m_sysCmdParam1;
    m_param2 = param.m_sysCmdParam2;
}

SystemCommandAction *SystemCommandAction::deepCopy() const
{
    SystemCommandAction *actToReturn = new SystemCommandAction();
    actToReturn->e_sysCommandType = e_sysCommandType;
    actToReturn->m_param1 = m_param1;
    actToReturn->m_param2 = m_param2;
    actToReturn->m_refID = m_ID;
    return actToReturn;
}

ActionParameters SystemCommandAction::generateParameters() const
{
    ActionParameters param;

    if(e_sysCommandType == SystemCommandType::ShutDown)
        param.m_sysCmdTypeStr = G_SystemCommands::ShutDownType;
    else if(e_sysCommandType == SystemCommandType::Restart)
        param.m_sysCmdTypeStr = G_SystemCommands::RestartType;
    else if(e_sysCommandType == SystemCommandType::LogOff)
        param.m_sysCmdTypeStr = G_SystemCommands::LogOffType;
    else if(e_sysCommandType == SystemCommandType::PutIntoSleepMode)
        param.m_sysCmdTypeStr = G_SystemCommands::PutIntoSleepModeType;
    else if(e_sysCommandType == SystemCommandType::ChangeAudioVolume)
        param.m_sysCmdTypeStr = G_SystemCommands::ChangeAudioVolumeType;
    else if(e_sysCommandType == SystemCommandType::ChangeDefaultAudioDevice)
        param.m_sysCmdTypeStr = G_SystemCommands::ChangeDefaultAudioDeviceType;
    else if(e_sysCommandType == SystemCommandType::KillProcess)
        param.m_sysCmdTypeStr = G_SystemCommands::KillProcessType;
    else if(e_sysCommandType == SystemCommandType::FocusWindow)
        param.m_sysCmdTypeStr = G_SystemCommands::FocusWindowType;
    else if(e_sysCommandType == SystemCommandType::CreateFolder)
        param.m_sysCmdTypeStr = G_SystemCommands::CreateFolderType;
    else if(e_sysCommandType == SystemCommandType::DeleteFolder)
        param.m_sysCmdTypeStr = G_SystemCommands::DeleteFolderType;
    else if(e_sysCommandType == SystemCommandType::CreateFile)
        param.m_sysCmdTypeStr = G_SystemCommands::CreateFileType;
    else if(e_sysCommandType == SystemCommandType::DeleteFile)
        param.m_sysCmdTypeStr = G_SystemCommands::DeleteFileType;
    else if(e_sysCommandType == SystemCommandType::TakeScreenshot)
        param.m_sysCmdTypeStr = G_SystemCommands::TakeScreenshotType;
    else if(e_sysCommandType == SystemCommandType::PrintActualScreen)
        param.m_sysCmdTypeStr = G_SystemCommands::PrintActualScreenType;
    else
        param.m_sysCmdTypeStr = G_SystemCommands::UndefinedType;

    param.m_sysCmdParam1 = m_param1;
    param.m_sysCmdParam2 = m_param2;

    return param;
}
