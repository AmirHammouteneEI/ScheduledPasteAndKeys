#include "TaskTab.h"

TaskTab::TaskTab(QWidget *parent, const QString &name)
    : QWidget{parent},m_name(name)
{
    m_ID = -1;
}
