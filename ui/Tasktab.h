#ifndef TASKTAB_H
#define TASKTAB_H

#include <QScrollArea>
#include <QVBoxLayout>

class TaskTab : public QScrollArea
{
    Q_OBJECT
protected:
    QString m_name;
    int m_ID;
    QVBoxLayout *m_actionsLayout;
public:
    explicit TaskTab(QWidget *parent = nullptr, const QString & name = "NONAME");

    friend class TaskTabsManager;
};

#endif // TASKTAB_H
