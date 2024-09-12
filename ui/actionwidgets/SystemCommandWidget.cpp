#include "SystemCommandWidget.h"

SystemCommandWidget::SystemCommandWidget(QWidget *parent)
    : AbstractActionWidget{parent}
{

}

void SystemCommandWidget::buildWidget()
{
    if(m_centralWidget == nullptr)
        return;

    auto sysCmdaction =  dynamic_cast<SystemCommandAction*>(m_action);

    QString sysCmdTypeStr = tr("ERROR");
    if(sysCmdaction != nullptr)
    {
        sysCmdTypeStr = fromSysCmdTypeToDiplayedStr(sysCmdaction->e_sysCommandType);

        m_mainButton->setToolTip(sysCmdTypeStr + "\n\nArgument 1 : "+sysCmdaction->m_param1+ "\n\nArgument 2 : "+sysCmdaction->m_param2);

        ActionParameters params = sysCmdaction->generateParameters();
        m_mainButton->setProperty("type", params.m_sysCmdTypeStr);
        m_mainButton->setProperty("param1", params.m_sysCmdParam1);
        m_mainButton->setProperty("param2", params.m_sysCmdParam2);
    }

    m_mainButton->setIcon(QIcon(":/img/systemCommand.png"));
    m_mainButton->setText(sysCmdTypeStr);

    //TODO m_editSystemCommandDialog = new CreateSystemCommandActionDialog(this);

    //connect(m_mainButton, &QPushButton::released, m_editDurationDialog, &CreateWaitActionDialog::showDialog);
    //connect(m_editDurationDialog, &CreateWaitActionDialog::sendDuration, this, &WaitWidget::durationReceived);
}

void SystemCommandWidget::systemCommandReceived(QString sysCmdType, QString param1, QString param2)
{
    auto sysCmdaction =  dynamic_cast<SystemCommandAction*>(m_action);
    if(sysCmdaction == nullptr)
    {
        m_mainButton->setToolTip("ERROR on access to action");
        m_mainButton->setText("ERROR");
        return;
    }

    ActionParameters params;
    params.m_sysCmdTypeStr = sysCmdType;
    params.m_sysCmdParam1 = param1;
    params.m_sysCmdParam2 = param2;

    sysCmdaction->setParameters(params);
    QString displayedStr = fromSysCmdTypeToDiplayedStr(sysCmdaction->e_sysCommandType);
    m_mainButton->setText(displayedStr);
    m_mainButton->setToolTip(displayedStr + "\n\nArgument 1 : "+param1+ "\n\nArgument 2 : "+param2);
    m_mainButton->setProperty("type", params.m_sysCmdTypeStr);
    m_mainButton->setProperty("param1", params.m_sysCmdParam1);
    m_mainButton->setProperty("param2", params.m_sysCmdParam2);

    emit anyParamChanged();
}

QString SystemCommandWidget::fromSysCmdTypeToDiplayedStr(SystemCommandType t)
{
    QString sysCmdTypeStr;
    if(t == SystemCommandType::ShutDown)
        sysCmdTypeStr = tr("Shutting down the PC");
    else if(t == SystemCommandType::Restart)
        sysCmdTypeStr = tr("Restarting the PC");
    else if(t == SystemCommandType::LogOff)
        sysCmdTypeStr = tr("Logout of user session");
    else if(t == SystemCommandType::PutIntoSleepMode)
        sysCmdTypeStr = tr("Putting the PC into sleep mode");
    else if(t == SystemCommandType::ChangeAudioVolume)
        sysCmdTypeStr = tr("Changing the audio volume");
    else if(t == SystemCommandType::ChangeDefaultAudioDevice)
        sysCmdTypeStr = tr("Changing the default audio device");
    else if(t == SystemCommandType::KillProcess)
        sysCmdTypeStr = tr("Killing a process");
    else if(t == SystemCommandType::FocusWindow)
        sysCmdTypeStr = tr("Focusing on a window");
    else if(t == SystemCommandType::CreateFolder)
        sysCmdTypeStr = tr("Creating a folder");
    else if(t == SystemCommandType::DeleteFolder)
        sysCmdTypeStr = tr("Deleting a folder");
    else if(t == SystemCommandType::CreateFile)
        sysCmdTypeStr = tr("Creating a file");
    else if(t == SystemCommandType::DeleteFile)
        sysCmdTypeStr = tr("Deleting a file");
    else if(t == SystemCommandType::TakeScreenshot)
        sysCmdTypeStr = tr("Taking a screenshot");
    else if(t == SystemCommandType::PrintActualScreen)
        sysCmdTypeStr = tr("Printing the actual screen display");

    return sysCmdTypeStr;
}


void SystemCommandWidget::changedRunningState()
{

}
