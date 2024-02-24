#ifndef TASKTAB_H
#define TASKTAB_H

#include "Task.h"

#include <QScrollArea>
#include <QVBoxLayout>

class TaskTab : public QScrollArea
{
    Q_OBJECT
protected:
    QString m_name;
    int m_ID;
    QVBoxLayout *m_actionsLayout;
    Task *m_task;
    void buildBasicInterface();
public:
    explicit TaskTab(QWidget *parent = nullptr, const QString & name = "NONAME");
    void TODELETE_TaskRunTest(); //TODELETE testing prebuilt task

public slots:
    void TODELETE_PushScheduleToDelayRun(); //TODELETE testing prebuilt task

    friend class TaskTabsManager;
};

#endif // TASKTAB_H
