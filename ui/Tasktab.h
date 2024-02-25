#ifndef TASKTAB_H
#define TASKTAB_H

#include "Task.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>

class TaskTab : public QScrollArea
{
    Q_OBJECT
protected:
    QString m_name;
    int m_ID;
    QVBoxLayout *m_actionsLayout;
    Task *m_task = nullptr;
    void buildBasicInterface();
    void setTask(Task *task);
    QLabel *m_nameLabel;
    void setName(const QString & newname);
public:
    explicit TaskTab(QWidget *parent = nullptr, const QString & name = "NONAME");
    ~TaskTab();
    void refreshActionsList(); //TODO refresh ui for actions widgets
    void runTaskThread();

public slots:
    void TODELETE_PushScheduleToDelayRun(); //TODELETE testing prebuilt task

    friend class TaskTabsManager;
};

#endif // TASKTAB_H
