#include "SystemCommandsAction.h"
#include "ActionsTools.h"
#include "globals.h"
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QScreen>
#include <QPainter>

#include "mainwindow.h"

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
            MainWindow* myMainWindow = nullptr;
            auto wList = qApp->topLevelWidgets();
            for(auto * w : wList)
            {
                if(w->inherits("MainWindow"))
                {
                    auto *mainW = qobject_cast<MainWindow*>(w);
                    if(mainW != nullptr)
                    {
                        myMainWindow = mainW;
                        break;
                    }
                }
            }
            if(myMainWindow != nullptr)
            {
                emit myMainWindow->m_stopAllTasksShortcut->activated();
            }
            QApplication::quit();
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
        case SystemCommandType::TakeScreenshot:
        {
            QFileInfo finfo(m_param1);
            QDir d;
            d.mkpath(finfo.absolutePath());
            QList<QPixmap> pixmaps;
            int width = 0;
            int height = 0;
            for(auto &screen : QApplication::screens())
            {
                width+=screen->geometry().width();
                if(height < screen->geometry().height())
                    height = screen->geometry().height();
                pixmaps << screen->grabWindow(0);
            }

            QPixmap globalPix(width, height);
            QPainter painter(&globalPix);
            globalPix.fill(Qt::white);
            int drawOrigin = 0;
            for(auto &pix : pixmaps)
            {
                painter.drawPixmap(QPoint(drawOrigin, 0), pix);
                drawOrigin += pix.width();
            }
            auto pathToSave = m_param2 == "Auto-rename if exists" ? incrementFilenameIfExists(m_param1) : m_param1;
            globalPix.save(pathToSave,"PNG");
        }
        break;
        case SystemCommandType::ChangeAudioVolume:
        {
            float volumeVal = m_param1.toInt()/100.0;
            if(volumeVal <0.0 || volumeVal >1.0)
                volumeVal = 0.5;

            CoInitialize(NULL);
            IMMDeviceEnumerator *deviceEnumerator = NULL;
            CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,__uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
            IMMDevice *defaultDevice = NULL;
            deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
            deviceEnumerator->Release();
            deviceEnumerator = NULL;
            IAudioEndpointVolume *endpointVolume = NULL;
            defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
                                         CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
            defaultDevice->Release();
            defaultDevice = NULL;

            endpointVolume->SetMasterVolumeLevelScalar(volumeVal, NULL);

            endpointVolume->Release();
            endpointVolume = NULL;
            CoUninitialize();
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

QString SystemCommandAction::incrementFilenameIfExists(const QString &path) const
{
    QFileInfo finfo(path);
    if(!finfo.exists())
        return path;

    auto filename = finfo.fileName();
    auto ext = finfo.suffix();
    auto name = filename.chopped(ext.size()+1);

    auto lastDigits = name.last(4);

    if(lastDigits.size() == 4 && lastDigits[0].isDigit() && lastDigits[1].isDigit() && lastDigits[2].isDigit() && lastDigits[3].isDigit() && lastDigits != "9999")
        name = name.chopped(4)+(QString::number(lastDigits.toInt()+1).rightJustified(4,'0'));
    else
        name.append("-0000");

    auto newPath = (path.chopped(filename.size()))+name+"."+ext;
    return incrementFilenameIfExists(newPath);
}

QString SystemCommandAction::incrementFilenameIfExists(const QString &path, unsigned char ndigits) const
{
    QFileInfo finfo(path);
    if(!finfo.exists() || ndigits == 0)
        return path;

    auto filename = finfo.fileName();
    auto ext = finfo.suffix();
    auto name = filename.chopped(ext.size()+1);

    auto lastDigits = name.last(ndigits);

    bool aredigits = true;
    for(QChar &c : lastDigits)
    {
        if(!c.isDigit())
        {
            aredigits = false;
            break;
        }
    }

    QString zeros = "0";
    zeros = ("-")+(zeros.rightJustified(ndigits,'0'));
    QString nines = "9";
    nines = nines.rightJustified(ndigits,'9');

    if(lastDigits.size() == ndigits && aredigits && lastDigits != nines)
        name = name.chopped(ndigits)+(QString::number(lastDigits.toInt()+1).rightJustified(ndigits,'0'));

    else
        name.append(zeros);

    auto newPath = (path.chopped(filename.size()))+name+"."+ext;
    return incrementFilenameIfExists(newPath,ndigits);
}
