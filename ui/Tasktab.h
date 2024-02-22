#ifndef TASKTAB_H
#define TASKTAB_H

#include <QObject>
#include <QWidget>

class TaskTab : public QWidget
{
    Q_OBJECT
protected:
    QString m_name;
    int m_ID;
public:
    explicit TaskTab(QWidget *parent = nullptr, const QString & name = "NONAME");

    friend class TaskTabsManager;
};

#endif // TASKTAB_H
