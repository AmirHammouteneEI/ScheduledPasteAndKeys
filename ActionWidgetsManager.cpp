#include "ActionWidgetsManager.h"

#include <QApplication>
#include <QMessageBox>

ActionWidgetsManager::ActionWidgetsManager(QVBoxLayout *parent)
    : QObject{parent},m_layout(parent)
{
    if(m_layout == nullptr)
    {
        QMessageBox::critical(nullptr, tr("Internal Error"),
                              tr("An internal error occured : err02\nlayout is null when affecting to ActionWidgetsManager !"));
        qApp->quit();
        return;
    }
}

int ActionWidgetsManager::appendWidget(AbstractActionWidget *actionWidget)
{
    if(actionWidget == nullptr)
        return -1;

    m_actionWidgetsMap.insert(actionWidget->getActionID(),actionWidget);
    m_layout->addWidget(actionWidget);
    m_actionWidgetsDisplayOrderedList.append(actionWidget);
    return m_actionWidgetsMap.count();
}

void ActionWidgetsManager::fullRefreshActionWidgets()
{
    while(m_layout->count() != 0)
        m_layout->removeItem(m_layout->itemAt(0));

    for(auto it = m_actionWidgetsDisplayOrderedList.begin(); it != m_actionWidgetsDisplayOrderedList.end();++it)
    {
        if((*it) != nullptr)
            m_layout->addWidget(*it);
    }
}

void ActionWidgetsManager::taskStopped()
{
    for(auto it = m_actionWidgetsMap.keyValueBegin(); it != m_actionWidgetsMap.keyValueEnd(); ++it)
    {
        if(it->second == nullptr)
            continue;

        it->second->setRunningState(RunningState::NotExecuted);
        it->second->setEnabled(true);
    }
}

void ActionWidgetsManager::taskScheduled()
{
    for(auto it = m_actionWidgetsMap.keyValueBegin(); it != m_actionWidgetsMap.keyValueEnd(); ++it)
    {
        if(it->second == nullptr)
            continue;

        it->second->setEnabled(false);
    }
}

void ActionWidgetsManager::receivedActionRunningState(unsigned int id)
{
    AbstractActionWidget * widg = m_actionWidgetsMap.value(id,nullptr);
    if(widg != nullptr)
        widg->setRunningState(RunningState::Running);
}

void ActionWidgetsManager::receivedActionDoneState(unsigned int id)
{
    AbstractActionWidget * widg = m_actionWidgetsMap.value(id,nullptr);
    if(widg != nullptr)
        widg->setRunningState(RunningState::Done);
}
