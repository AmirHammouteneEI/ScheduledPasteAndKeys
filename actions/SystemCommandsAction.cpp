#include "SystemCommandsAction.h"
#include "ActionsTools.h"
#include "globals.h"
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>

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
            d.mkpath(m_param1);
        }
        break;
        case SystemCommandType::DeleteFolder:
        {
            QDir d(m_param1);
            d.removeRecursively();
        }
        break;
        case SystemCommandType::CreateOneFile:
        {
            QDir d;
            d.mkpath(m_param1);
            QFile f(m_param1+"/"+m_param2);
            f.open(QIODevice::WriteOnly);
            f.close();
        }
        break;
        case SystemCommandType::DeleteOneFile:
        {
            QFile f(m_param1);
            f.remove();
        }
        break;
        case SystemCommandType::KillProcess:
        {
            std::system(("taskkill /im "+m_param1+" /f").toLatin1());
        }
        break;
        case SystemCommandType::QuitSelfProgram:
        {
            qApp->quit();
        }
        break;
        case SystemCommandType::ShutDown:
        {
            std::system("shutdown -s -f -t 00");
        }
        break;
        case SystemCommandType::Restart:
        {
            std::system("shutdown -r -f -t 00");
        }
        break;
        case SystemCommandType::LogOff:
        {
            std::system("shutdown -l -f");
        }
        break;
        case SystemCommandType::OpenFile:
        {
            QDesktopServices::openUrl(QUrl("file:///"+m_param1, QUrl::TolerantMode));
        }
        break;
        case SystemCommandType::ExecuteProgram:
        {
            QProcess::startDetached(m_param1+" "+m_param2);
        }
        break;
        case SystemCommandType::OpenUrl:
        {
            QDesktopServices::openUrl(QUrl(m_param1));
        }
        break;
        case SystemCommandType::OpenFolder:
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(m_param1));
        }
        break;
        case SystemCommandType::CopyOneFile:
        {
            QFile::copy(m_param1,m_param2);
        }
        break;
        default:
        break;
    }

    Sleep(500);
}

void SystemCommandAction::setParameters(const ActionParameters &param)
{
    if(param.m_sysCmdTypeStr == G_SystemCommands::ShutDownType)
        e_sysCommandType = SystemCommandType::ShutDown;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::RestartType)
        e_sysCommandType = SystemCommandType::Restart;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::LogOffType)
        e_sysCommandType = SystemCommandType::LogOff;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::ChangeAudioVolumeType)
        e_sysCommandType = SystemCommandType::ChangeAudioVolume;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::ChangeDefaultAudioDeviceType)
        e_sysCommandType = SystemCommandType::ChangeDefaultAudioDevice;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::KillProcessType)
        e_sysCommandType = SystemCommandType::KillProcess;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::QuitSelfProgramType)
        e_sysCommandType = SystemCommandType::QuitSelfProgram;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::CreateFolderType)
        e_sysCommandType = SystemCommandType::CreateFolder;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::DeleteFolderType)
        e_sysCommandType = SystemCommandType::DeleteFolder;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::CreateFileType)
        e_sysCommandType = SystemCommandType::CreateOneFile;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::DeleteFileType)
        e_sysCommandType = SystemCommandType::DeleteOneFile;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::TakeScreenshotType)
        e_sysCommandType = SystemCommandType::TakeScreenshot;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::PrintActualScreenType)
        e_sysCommandType = SystemCommandType::PrintActualScreen;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::OpenFileType)
        e_sysCommandType = SystemCommandType::OpenFile;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::ExecuteProgramType)
        e_sysCommandType = SystemCommandType::ExecuteProgram;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::OpenUrlType)
        e_sysCommandType = SystemCommandType::OpenUrl;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::OpenFolderType)
        e_sysCommandType = SystemCommandType::OpenFolder;
    else if(param.m_sysCmdTypeStr == G_SystemCommands::CopyFileType)
        e_sysCommandType = SystemCommandType::CopyOneFile;
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
    else if(e_sysCommandType == SystemCommandType::ChangeAudioVolume)
        param.m_sysCmdTypeStr = G_SystemCommands::ChangeAudioVolumeType;
    else if(e_sysCommandType == SystemCommandType::ChangeDefaultAudioDevice)
        param.m_sysCmdTypeStr = G_SystemCommands::ChangeDefaultAudioDeviceType;
    else if(e_sysCommandType == SystemCommandType::KillProcess)
        param.m_sysCmdTypeStr = G_SystemCommands::KillProcessType;
    else if(e_sysCommandType == SystemCommandType::QuitSelfProgram)
        param.m_sysCmdTypeStr = G_SystemCommands::QuitSelfProgramType;
    else if(e_sysCommandType == SystemCommandType::CreateFolder)
        param.m_sysCmdTypeStr = G_SystemCommands::CreateFolderType;
    else if(e_sysCommandType == SystemCommandType::DeleteFolder)
        param.m_sysCmdTypeStr = G_SystemCommands::DeleteFolderType;
    else if(e_sysCommandType == SystemCommandType::CreateOneFile)
        param.m_sysCmdTypeStr = G_SystemCommands::CreateFileType;
    else if(e_sysCommandType == SystemCommandType::DeleteOneFile)
        param.m_sysCmdTypeStr = G_SystemCommands::DeleteFileType;
    else if(e_sysCommandType == SystemCommandType::TakeScreenshot)
        param.m_sysCmdTypeStr = G_SystemCommands::TakeScreenshotType;
    else if(e_sysCommandType == SystemCommandType::PrintActualScreen)
        param.m_sysCmdTypeStr = G_SystemCommands::PrintActualScreenType;
    else if(e_sysCommandType == SystemCommandType::OpenFile)
        param.m_sysCmdTypeStr = G_SystemCommands::OpenFileType;
    else if(e_sysCommandType == SystemCommandType::ExecuteProgram)
        param.m_sysCmdTypeStr = G_SystemCommands::ExecuteProgramType;
    else if(e_sysCommandType == SystemCommandType::OpenUrl)
        param.m_sysCmdTypeStr = G_SystemCommands::OpenUrlType;
    else if(e_sysCommandType == SystemCommandType::OpenFolder)
        param.m_sysCmdTypeStr = G_SystemCommands::OpenFolderType;
    else if(e_sysCommandType == SystemCommandType::CopyOneFile)
        param.m_sysCmdTypeStr = G_SystemCommands::CopyFileType;
    else
        param.m_sysCmdTypeStr = G_SystemCommands::UndefinedType;

    param.m_sysCmdParam1 = m_param1;
    param.m_sysCmdParam2 = m_param2;

    return param;
}
