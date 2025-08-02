#ifndef TASKTABSMANAGER_H
#define TASKTABSMANAGER_H

#include "ui/TaskTab.h"

#include <QObject>
#include <QJsonObject>

class MainWindow;

class TaskTabsManager : public QObject
{
    Q_OBJECT

    QMap<unsigned int, TaskTab*> m_taskTabsMap;
    unsigned int m_idCounter = 0;
    QMap<unsigned int, QString> m_taskFilePathsMap; // may contains old closed tab paths of file, but should be updated with opened tasks
public:
    explicit TaskTabsManager();
    ~TaskTabsManager();
    void forceCloseTask(int id);
    void scheduleTaskFromId(int id, qint64 delay, int loopTimes = 1);
public slots:
    int onOpenNewTabRequest(const QString & path);
    void onTabCloseRequest(int index);
    void onRefreshTabsRequest();
    void onTaskfilePathChanged(QString oldpath, QString newpath);
    void forceStopAllRunningTasksReceived();
    void saveTaskReceived(int taskTabId, bool verbose = false);
    QJsonObject actionToJson(AbstractAction *act);
    AbstractAction* jsonToAction(const QJsonObject &jobj);
    void saveAllTasks();
    bool isAnyTaskModified();
protected:
    TaskTab* createEmptyTaskAndOpenTab(const QString& name);
    int appendTaskInMap(TaskTab* task);
    int getIdfromTaskName(const QString &name) const;
    int getTabIndexfomId(int id) const;
    void createAndLoadTaskObject(int id);

    friend class MainWindow;
};

#endif // TASKTABSMANAGER_H
