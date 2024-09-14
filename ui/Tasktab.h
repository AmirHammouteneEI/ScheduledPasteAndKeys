#ifndef TASKTAB_H
#define TASKTAB_H

#include "Task.h"
#include "ui/getDelayDialog.h"
#include "ActionWidgetsManager.h"
#include "ui/createactiondialog/CreatePasteActionDialog.h"
#include "ui/createactiondialog/CreateWaitActionDialog.h"
#include "ui/createactiondialog/CreateKeysSequenceActionDialog.h"
#include "ui/createactiondialog/CreateSystemCommandActionDialog.h"

#include <QFrame>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QTimer>
#include <QSpinBox>

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
    CreateWaitActionDialog *m_createWaitActionDialog;
    CreateSystemCommandActionDialog *m_createSystemCommandActionDialog;
    CreateKeysSequenceActionDialog *m_m_createKeysSequenceActionDialog;
    void buildAddButtonMenu();
    void appendAction(AbstractAction *act);
    unsigned int m_loopedTimes = 0;
    QLabel *m_loopedTimesLabel;
    QPushButton *m_saveButton;
    bool m_taskModifiedFromLastSave = false;
    QWidget *m_runOptionsWidget;
    QWidget *m_timesToRunWidget;
    QSpinBox *m_timesToRunSpinBox;
public:
    explicit TaskTab(QWidget *parent = nullptr, const QString & name = "NONAME");
    ~TaskTab();
    void runTaskThread();
    ScheduleState m_scheduleState;
    void setTaskModified(bool val);
    bool taskIsModified() const {return m_taskModifiedFromLastSave;}
private slots:
    void stopPushed();
    void loopToggled(bool state);
    void refreshScheduleText();
    void anyActionChangedParam();
public slots:
    void scheduleTaskAfterDelay(qint64 delayInSeconds);
    void finishedOneLoop();
    void receivedActionRunningState(unsigned int actId);
    void removeActionReceived(unsigned int actId);
    void moveToTopActionReceived(unsigned int actId);
    void moveToBottomActionReceived(unsigned int actId);
    void moveUpActionReceived(unsigned int actId);
    void moveDownActionReceived(unsigned int actId);
    void createPasteActionRequest(QString sentenceIdentity); // not const & because it's a slot, will make copy evenif
    void createWaitActionRequest(long double duration);
    void createKeysSequenceActionRequest(QString keysSequenceIdentity); // not const & because it's a slot, will make copy evenif
    void createSystemCommandActionRequest(QString sysCmdType, QString param1, QString param2); // not const & because it's a slot, will make copy evenif
signals:
    void saveTaskRequest(int taskId, bool verb);

    friend class TaskTabsManager;
};

#endif // TASKTAB_H
