#ifndef TASKTABSMANAGER_H
#define TASKTABSMANAGER_H

#include <QObject>

class TaskTabsManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskTabsManager(QObject *parent = nullptr);

signals:
};

#endif // TASKTABSMANAGER_H
