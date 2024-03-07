#ifndef TASKTAB_H
#define TASKTAB_H

#include "Task.h"
#include "ui/getDelayDialog.h"
#include "ActionWidgetsManager.h"
#include "ui/createactiondialog/CreatePasteActionDialog.h"

#include <QFrame>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QTimer>

enum class ScheduleState{
    NotScheduled,
    ScheduledInDelay,
    Running
};

class TaskTab : public QScrollArea
{
    Q_OBJECT
protected:
    QString m_name;
    int m_ID;
    QWidget *m_mainWidget;
    QFrame *m_actionsFrame;
    QVBoxLayout *m_actionsLayout;
    Task *m_task = nullptr;
    void buildBasicInterface();
    void setTask(Task *task);
    AbstractActionWidget *createActionWidget(AbstractAction *act);
    QLabel *m_nameLabel;
    QPushButton *m_scheduleButton;
    QPushButton *m_stopButton;
    QToolButton *m_addActionButton;
    QToolButton *m_loopButton;
    QLabel *m_delayChrono;
    void setName(const QString & newname);
    getDelayDialog *m_getDelayDialog;
    QDateTime m_datetimeOfRun;
    ActionWidgetsManager *m_actionWidgetsManager;
    QTimer *m_scheduleTimer;
    CreatePasteActionDialog *m_createPasteActionDialog;
    void buildAddButtonMenu();
    void appendAction(AbstractAction *act);
public:
    explicit TaskTab(QWidget *parent = nullptr, const QString & name = "NONAME");
    ~TaskTab();
    void runTaskThread();
    ScheduleState m_scheduleState;

private slots:
    void stopPushed();
    void loopToggled(bool state);
    void refreshScheduleText();
public slots:
    void scheduleTaskAfterDelay(qint64 delayInSeconds);
    void finishedOneLoop();
    void receivedActionRunningState(unsigned int actId);
    void removeActionReceived(unsigned int actId);
    void moveToTopActionReceived(unsigned int actId);
    void moveToBottomActionReceived(unsigned int actId);
    void moveUpActionReceived(unsigned int actId);
    void moveDownActionReceived(unsigned int actId);
    void createPasteActionRequest(QString sentenceIdentity, float addWaitActionSeconds);

    friend class TaskTabsManager;
};

#endif // TASKTAB_H
