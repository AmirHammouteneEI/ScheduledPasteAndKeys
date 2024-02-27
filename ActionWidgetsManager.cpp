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

    actionWidget->buildWidget();
    m_actionWidgetsList.append(actionWidget);
    m_layout->addWidget(actionWidget);
    return m_actionWidgetsList.count();
}
