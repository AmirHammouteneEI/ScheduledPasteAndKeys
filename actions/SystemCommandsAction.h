#ifndef SYSTEMCOMMANDSACTION_H
#define SYSTEMCOMMANDSACTION_H

#include "AbstractAction.h"

enum class SystemCommandType {
    Undefined,
    ShutDown,
    Restart,
    LogOff,
    ChangeAudioVolume,
    ChangeDefaultAudioDevice,
    KillProcess,
    QuitSelfProgram,
    CreateFolder,
    DeleteFolder,
    CreateOneFile,
    DeleteOneFile,
    TakeScreenshot,
    PrintActualScreen,
    OpenFile,
    CopyOneFile,
    ExecuteProgram,
    OpenUrl,
    OpenFolder
};

class SystemCommandAction : public AbstractAction
{
    Q_OBJECT
private:
    SystemCommandType e_sysCommandType;
    QString m_param1;
    QString m_param2;
public:
    SystemCommandAction();
    ~SystemCommandAction() = default;
    SystemCommandAction(const SystemCommandAction & other) = delete;
    SystemCommandAction& operator=(const SystemCommandAction & other) = delete;

    void runAction() override;
    void setParameters(const ActionParameters& param) override;
    std::shared_ptr<AbstractAction> deepCopy() const override;
    ActionParameters generateParameters() const override;
    void optionalProcesses() override {}

    QString incrementFilenameIfExists(const QString & path) const;
    QString incrementFilenameIfExists(const QString & path, unsigned char ndigits) const;

    friend class SystemCommandWidget;
signals:
    void forceQuitProgramRequest();
};

#endif // SYSTEMCOMMANDSACTION_H
