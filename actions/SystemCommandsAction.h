#ifndef SYSTEMCOMMANDSACTION_H
#define SYSTEMCOMMANDSACTION_H

#include "AbstractAction.h"

enum class SystemCommandType {
    Undefined,
    ShutDown,
    Restart,
    LogOff,
    PutIntoSleepMode,
    ChangeAudioVolume,
    ChangeDefaultAudioDevice,
    KillProcess,
    FocusWindow,
    CreateFolder,
    DeleteFolder,
    CreateFile,
    DeleteFile,
    TakeScreenshot,
    PrintActualScreen
};

class SystemCommandAction : public AbstractAction
{
public:
    SystemCommandAction();
    ~SystemCommandAction() = default;
    SystemCommandAction(const SystemCommandAction & other) = delete;
    SystemCommandAction& operator=(const SystemCommandAction & other) = delete;

    void runAction() const override;
    void setParameters(const ActionParameters& param) override;
    SystemCommandAction *deepCopy() const override;
    ActionParameters generateParameters() const override;
    void optionalProcesses() override {}

    void executeCommand(const QString & cmd,const QString & option) const;

    SystemCommandType e_sysCommandType;
    QString m_param1;
    QString m_param2;
};

#endif // SYSTEMCOMMANDSACTION_H
