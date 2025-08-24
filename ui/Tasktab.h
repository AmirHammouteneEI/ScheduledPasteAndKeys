#ifndef TASKTAB_H
#define TASKTAB_H

#include "Task.h"
#include "ui/getdialogs/getDelayDialog.h"
#include "ActionWidgetsManager.h"
#include "ui/createactiondialogs/CreatePasteActionDialog.h"
#include "ui/createactiondialogs/CreateWaitActionDialog.h"
#include "ui/createactiondialogs/CreateKeysSequenceActionDialog.h"
#include "ui/createactiondialogs/CreateSystemCommandActionDialog.h"
#include "ui/createactiondialogs/CreateCursorMovementsActionDialog.h"
#include "ui/createactiondialogs/CreateRunningOtherTaskActionDialog.h"
#include "ui/CustomPrimaryWidgets.h"

#include <QFrame>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QTimer>
#include <QPlainTextEdit>

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
    QString m_description = nullptr;
    QWidget *m_mainWidget = nullptr;
    QFrame *m_actionsFrame = nullptr;
    QVBoxLayout *m_actionsLayout = nullptr;
    std::shared_ptr<Task> m_task = nullptr;
    void buildBasicInterface();
    void setTask(const std::shared_ptr<Task> & task);
    AbstractActionWidget *createActionWidget(const std::shared_ptr<AbstractAction> &act);
    QLabel *m_nameLabel = nullptr;
    QPlainTextEdit *m_descriptionEdit = nullptr;
    QPushButton *m_scheduleButton = nullptr;
    QPushButton *m_stopButton = nullptr;
    QToolButton *m_addActionButton = nullptr;
    QToolButton *m_loopButton = nullptr;
    QLabel *m_delayChrono = nullptr;
    void setName(const QString & newname);
    void setDescription(const QString & newdescription);
    getDelayDialog *m_getDelayDialog = nullptr;
    QDateTime m_datetimeOfRun;
    ActionWidgetsManager *m_actionWidgetsManager = nullptr;
    QTimer *m_scheduleTimer = nullptr;
    CreatePasteActionDialog *m_createPasteActionDialog = nullptr;
    CreateWaitActionDialog *m_createWaitActionDialog = nullptr;
    CreateSystemCommandActionDialog *m_createSystemCommandActionDialog = nullptr;
    CreateKeysSequenceActionDialog *m_createKeysSequenceActionDialog = nullptr;
    CreateCursorMovementsActionDialog *m_createCursorMovementsActionDialog = nullptr;
    CreateRunningOtherTaskActionDialog *m_createRunningOtherTaskActionDialog = nullptr;
    void buildAddButtonMenu();
    void appendAction(const std::shared_ptr<AbstractAction> &act);
    unsigned int m_loopedTimes = 0;
    QLabel *m_loopedTimesLabel = nullptr;
    QPushButton *m_saveButton = nullptr;
    bool m_taskModifiedFromLastSave = false;
    QWidget *m_runOptionsWidget = nullptr;
    QWidget *m_timesToRunWidget = nullptr;
    NoWheelFocusSpinBox *m_timesToRunSpinBox = nullptr;
    void refreshTabRunIcon();
public:
    explicit TaskTab(QWidget *parent = nullptr, const QString & name = "NONAME");
    ~TaskTab();
    void runTaskThread();
    ScheduleState m_scheduleState;
    void setTaskModified(bool val);
    bool taskIsModified() const {return m_taskModifiedFromLastSave;}
    void setTimesToRunValue(int timesToRun);
private slots:
    void stopTask();
    void forcedStop();
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
    void createWaitActionRequest(long double duration); // not const & because it's a slot, will make copy evenif
    void createKeysSequenceActionRequest(QString keysSequenceIdentity); // not const & because it's a slot, will make copy evenif
    void createSystemCommandActionRequest(QString sysCmdType, QString param1, QString param2); // not const & because it's a slot, will make copy evenif
    void createCursorMovementsActionRequest(QString cursorMovementsIdentity); // not const & because it's a slot, will make copy evenif
    void createRunningOtherTaskActionRequest(QString filename, int delay, int loops); // not const & because it's a slot, will make copy evenif
signals:
    void saveTaskRequest(int taskId, bool verb);
    void forceStopThread();

    friend class TaskTabsManager;
};

#endif // TASKTAB_H
