#ifndef TASKTABSMANAGER_H
#define TASKTABSMANAGER_H

#include "ui/TaskTab.h"

#include <QObject>

class MainWindow;

class TaskTabsManager : public QObject
{
    Q_OBJECT

    MainWindow *m_mainwindow;
    QMap<unsigned int, TaskTab*> m_taskTabsMap;
    unsigned int m_idCounter = 0;
    QMap<unsigned int, QString> m_taskFilePathsMap; // may contains old closed tab paths of file, but should be updated with opened tasks
public:
    explicit TaskTabsManager(MainWindow *parent = nullptr);
    void forceCloseTask(int id);
public slots:
    void onOpenNewTabRequest(QString path);
    void onTabCloseRequest(int index);
    void onRefreshTabsRequest();
    void onTaskfilePathChanged(QString oldpath, QString newpath);
protected:
    TaskTab* createEmptyTaskAndOpenTab(const QString& name);
    int appendTaskInMap(TaskTab* task);
    int getIdforTaskName(const QString &name);
    int getTabIndexforId(int id);

signals:
};

#endif // TASKTABSMANAGER_H
