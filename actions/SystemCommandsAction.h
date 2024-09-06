#ifndef SYSTEMCOMMANDSACTION_H
#define SYSTEMCOMMANDSACTION_H

#include "AbstractAction.h"
#include <QList>
#include <QPair>
#include <QString>

class SystemCommandsAction : public AbstractAction
{
private:
    QList<QPair<QString, QString>> m_commands; // list of ordered commands and their options
    QString m_sysCmdsId;
public:
    SystemCommandsAction();
    ~SystemCommandsAction() = default;
    SystemCommandsAction(const SystemCommandsAction & other) = delete;
    SystemCommandsAction& operator=(const SystemCommandsAction & other) = delete;

    void runAction() const override;
    void setParameters(const ActionParameters& param) override;
    SystemCommandsAction *deepCopy() const override;
    ActionParameters generateParameters() const override;
    void optionalProcesses() override {}

    void executeCommand(const QString & cmd,const QString & option) const;
};

#endif // SYSTEMCOMMANDSACTION_H
