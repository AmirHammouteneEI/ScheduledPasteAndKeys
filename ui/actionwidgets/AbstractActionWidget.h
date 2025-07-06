#ifndef ABSTRACTACTIONWIDGET_H
#define ABSTRACTACTIONWIDGET_H

#include "actions/AbstractAction.h"
#include "qevent.h"

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>

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
    virtual void changedRunningState() {}
    QPushButton *m_mainButton;
    QLabel *m_infoLabel;
    QPushButton *m_removeButton;
    QPushButton *m_moveToTopButton;
    QPushButton *m_moveToBottomButton;
    QPushButton *m_moveUpButton;
    QPushButton *m_moveDownButton;
public:
    explicit AbstractActionWidget(QWidget *parent = nullptr);
    virtual ~AbstractActionWidget();
    void setRunningState(RunningState state);
    void setAction(AbstractAction *action);
    virtual void buildWidget() = 0;
    unsigned int getActionID() const {return m_actionID;}

    inline bool operator==(const AbstractActionWidget& otherActWidg) { return m_actionID == otherActWidg.m_actionID; }

protected slots:
    void removeSelf();
    void moveToTopSelf();
    void moveToBottomSelf();
    void moveUpSelf();
    void moveDownSelf();
signals:
    void removeActionRequest(unsigned int id);
    void moveToTopActionRequest(unsigned int id);
    void moveToBottomActionRequest(unsigned int id);
    void moveUpActionRequest(unsigned int id);
    void moveDownActionRequest(unsigned int id);
    void anyParamChanged();
};

class NoWheelFocusSpinBox : public QSpinBox
{
    Q_OBJECT
public:

    NoWheelFocusSpinBox(QWidget *parent = 0):QSpinBox(parent)
    {
        setFocusPolicy(Qt::StrongFocus);
    }
protected:
    virtual void wheelEvent(QWheelEvent *event) override
    {
        if(!hasFocus())
            event->ignore();
        else
            QSpinBox::wheelEvent(event);
    }
};

#endif // ABSTRACTACTIONWIDGET_H
