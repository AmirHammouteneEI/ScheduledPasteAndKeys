#ifndef TASKTAB_H
#define TASKTAB_H

#include "Task.h"
#include "ui/getDelayDialog.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>

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
    QVBoxLayout *m_actionsLayout;
    Task *m_task = nullptr;
    void buildBasicInterface();
    void setTask(Task *task);
    QLabel *m_nameLabel;
    QPushButton *m_scheduleButton;
    QPushButton *m_stopButton;
    QPushButton *m_addActionButton;
    QToolButton *m_loopButton;
    QLabel *m_delayChrono;
    QLabel *m_loopState;
    void setName(const QString & newname);
    getDelayDialog *m_getDelayDialog;
    QDateTime m_datetimeOfRun;
public:
    explicit TaskTab(QWidget *parent = nullptr, const QString & name = "NONAME");
    ~TaskTab();
    void refreshActionsList(); //TODO refresh ui for actions widgets
    void runTaskThread();
    ScheduleState m_scheduleState;

private slots:
    void stopPushed();
    void loopToggled(bool state);
    void refreshScheduleText();
public slots:
    void scheduleTaskAfterDelay(qint64 delayInSeconds);

    friend class TaskTabsManager;
};

#endif // TASKTAB_H
