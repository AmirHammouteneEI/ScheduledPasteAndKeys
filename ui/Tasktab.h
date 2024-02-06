#ifndef TASKTAB_H
#define TASKTAB_H

#include <QObject>
#include <QWidget>

class TaskTab : public QWidget
{
    Q_OBJECT
public:
    explicit TaskTab(QWidget *parent = nullptr);

signals:
};

#endif // TASKTAB_H
