#ifndef SYSTEMCOMMANDWIDGET_H
#define SYSTEMCOMMANDWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"
#include "actions/SystemCommandsAction.h"

class SystemCommandWidget : public AbstractActionWidget
{
    Q_OBJECT
    void changedRunningState() override;
    //TODO CreateWaitActionDialog *m_editDurationDialog;
public:
    explicit SystemCommandWidget(QWidget *parent = nullptr);
    ~SystemCommandWidget() = default;
    void buildWidget() override;
    void systemCommandReceived(QString sysCmdType, QString param1, QString param2); // not const & because it's a slot, will make copy evenif

private:
    QString fromSysCmdTypeToDiplayedStr(SystemCommandType t);
};

#endif // SYSTEMCOMMANDWIDGET_H
