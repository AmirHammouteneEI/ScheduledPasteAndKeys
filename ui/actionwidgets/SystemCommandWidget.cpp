#include "SystemCommandWidget.h"

SystemCommandWidget::SystemCommandWidget(QWidget *parent)
    : AbstractActionWidget{parent}
{

}

void SystemCommandWidget::buildWidget()
{
    if(m_centralWidget == nullptr)
        return;

    auto sysCmdaction =  dynamic_cast<SystemCommandAction*>(m_action.get());

    QString sysCmdTypeStr = tr("ERROR");
    if(sysCmdaction != nullptr)
    {
        sysCmdTypeStr = fromSysCmdTypeToDiplayedStr(sysCmdaction->e_sysCommandType);

        m_mainButton->setToolTip(sysCmdTypeStr + "\nOption 1 : "+sysCmdaction->m_param1+ "\nOption 2 : "+sysCmdaction->m_param2);

        ActionParameters params = sysCmdaction->generateParameters();
        m_mainButton->setProperty("type", params.m_sysCmdTypeStr);
        m_mainButton->setProperty("param1", params.m_sysCmdParam1);
        m_mainButton->setProperty("param2", params.m_sysCmdParam2);
        m_mainButton->setProperty("displayedStr", sysCmdTypeStr);
    }

    m_mainButton->setObjectName("syscommandActionButton");
    m_mainButton->setText(sysCmdTypeStr);

    m_editSysCmdDialog = new CreateSystemCommandActionDialog(this);

    connect(m_mainButton, &QPushButton::released, m_editSysCmdDialog, &CreateSystemCommandActionDialog::showDialog);
    connect(m_editSysCmdDialog, &CreateSystemCommandActionDialog::sendSystemCommand, this, &SystemCommandWidget::systemCommandReceived);
}

void SystemCommandWidget::systemCommandReceived(QString sysCmdType, QString param1, QString param2)
{
    auto sysCmdaction =  dynamic_cast<SystemCommandAction*>(m_action.get());
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
    m_mainButton->setToolTip(displayedStr + "\n\nOption 1 : "+param1+ "\nOption 2 : "+param2);
    m_mainButton->setProperty("type", params.m_sysCmdTypeStr);
    m_mainButton->setProperty("param1", params.m_sysCmdParam1);
    m_mainButton->setProperty("param2", params.m_sysCmdParam2);

    emit anyParamChanged();
}

QString SystemCommandWidget::fromSysCmdTypeToDiplayedStr(SystemCommandType t)
{
    QString sysCmdTypeStr;
    if(t == SystemCommandType::ShutDown)
        sysCmdTypeStr = tr("Shut down the PC");
    else if(t == SystemCommandType::Restart)
        sysCmdTypeStr = tr("Restart the PC");
    else if(t == SystemCommandType::LogOff)
        sysCmdTypeStr = tr("Log off user session");
    else if(t == SystemCommandType::ChangeAudioVolume)
        sysCmdTypeStr = tr("Change PC audio volume");
    else if(t == SystemCommandType::ChangeDefaultAudioDevice)
        sysCmdTypeStr = tr("Change the default audio device");
    else if(t == SystemCommandType::KillProcess)
        sysCmdTypeStr = tr("Kill a process");
    else if(t == SystemCommandType::QuitSelfProgram)
        sysCmdTypeStr = tr("Quit Tasket++");
    else if(t == SystemCommandType::CreateFolder)
        sysCmdTypeStr = tr("Create a folder");
    else if(t == SystemCommandType::DeleteFolder)
        sysCmdTypeStr = tr("Delete a folder");
    else if(t == SystemCommandType::CreateOneFile)
        sysCmdTypeStr = tr("Create a file");
    else if(t == SystemCommandType::DeleteOneFile)
        sysCmdTypeStr = tr("Delete a file");
    else if(t == SystemCommandType::TakeScreenshot)
        sysCmdTypeStr = tr("Take a screenshot");
    else if(t == SystemCommandType::PrintActualScreen)
        sysCmdTypeStr = tr("Print the actual screen display");
    else if(t == SystemCommandType::OpenFile)
        sysCmdTypeStr = tr("Open a file");
    else if(t == SystemCommandType::ExecuteProgram)
        sysCmdTypeStr = tr("Execute a program");
    else if(t == SystemCommandType::OpenUrl)
        sysCmdTypeStr = tr("Open an URL");
    else if(t == SystemCommandType::OpenFolder)
        sysCmdTypeStr = tr("Open a folder");
    else if(t == SystemCommandType::CopyOneFile)
        sysCmdTypeStr = tr("Copy a file");

    return sysCmdTypeStr;
}
