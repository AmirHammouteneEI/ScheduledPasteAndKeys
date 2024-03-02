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
    unsigned int m_actionID = 0;
    RunningState m_runningState;
public:
    explicit AbstractActionWidget(QWidget *parent = nullptr);
    virtual ~AbstractActionWidget();
    void setRunningState(RunningState state);
    void setAction(AbstractAction *action);
    virtual void buildWidget() = 0;
    unsigned int getActionID() {return m_actionID;}

    inline bool operator==(const AbstractActionWidget& otherActWidg) { return m_actionID == otherActWidg.m_actionID; }

protected slots:
    void removeSelf();
signals:
    void removeActionRequest(unsigned int id);
};

#endif // ABSTRACTACTIONWIDGET_H
