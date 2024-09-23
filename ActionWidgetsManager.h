#ifndef ACTIONWIDGETSMANAGER_H
#define ACTIONWIDGETSMANAGER_H

#include "ui/actionwidgets/AbstractActionWidget.h"

#include <QObject>
#include <QVBoxLayout>

class ActionWidgetsManager : public QObject
{
    Q_OBJECT
    QVBoxLayout *m_layout = nullptr;
    QMap<unsigned int, AbstractActionWidget*> m_actionWidgetsMap;
    QList<AbstractActionWidget*> m_actionWidgetsDisplayOrderedList;
    void fullRefreshActionWidgets();
public:
    explicit ActionWidgetsManager(QVBoxLayout *parent = nullptr);
    ~ActionWidgetsManager();
    int appendWidget(AbstractActionWidget* actionWidget);
    void clear();
    void taskStopped();
    void taskScheduled();
public slots:
    void receivedActionRunningState(unsigned int id);
    void receivedActionDoneState(unsigned int id);

    friend class TaskTab;
};

#endif // ACTIONWIDGETSMANAGER_H
