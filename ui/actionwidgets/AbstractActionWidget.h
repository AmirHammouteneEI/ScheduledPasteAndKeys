#ifndef ABSTRACTACTIONWIDGET_H
#define ABSTRACTACTIONWIDGET_H

#include "actions/AbstractAction.h"

#include <QFrame>

enum class RunningState{
    NotExecuted,
    Running,
    Done
};

class AbstractActionWidget : public QFrame
{
    Q_OBJECT
protected:
    QFrame *m_centralWidget = nullptr;
    AbstractAction *m_action = nullptr;
    unsigned int m_actionID;
    RunningState m_runningState;
public:
    explicit AbstractActionWidget(QWidget *parent = nullptr);
    void setRunningState(RunningState state);
    void setAction(AbstractAction *action);
    virtual void buildWidget() = 0;
    unsigned int getActionID() {return m_actionID;}

signals:
    void removeSelf();
};

#endif // ABSTRACTACTIONWIDGET_H
