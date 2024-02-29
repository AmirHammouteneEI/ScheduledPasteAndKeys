#include "AbstractActionWidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QStyle>

AbstractActionWidget::AbstractActionWidget(QWidget *parent)
    : QFrame{parent}
{
    auto gridLayout = new QGridLayout(this);
    m_centralWidget = new QFrame(this);

    auto removeButton = new QPushButton(QIcon(":/img/close_red.png"),"",this);
    removeButton->setMaximumSize(QSize(20,20));
    removeButton->setFlat(true);

    gridLayout->addItem(new QSpacerItem(3,3,QSizePolicy::Minimum,QSizePolicy::Minimum),0,0);
    gridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum),1,0);
    gridLayout->addWidget(m_centralWidget,1,1,Qt::AlignCenter | Qt::AlignHCenter);
    gridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum),1,2);
    gridLayout->addWidget(removeButton,1,3,Qt::AlignRight | Qt::AlignTop);
    gridLayout->addItem(new QSpacerItem(3,3,QSizePolicy::Minimum,QSizePolicy::Minimum),2,0);

    gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
    gridLayout->setContentsMargins(1,1,1,1);
    gridLayout->setSpacing(2);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    setRunningState(RunningState::NotExecuted);
    connect(removeButton,&QPushButton::released, this, &AbstractActionWidget::removeSelf);
}

void AbstractActionWidget::setRunningState(RunningState state)
{
    m_runningState = state;

    if(m_runningState == RunningState::NotExecuted)
        setProperty("runningState","notExecuted");
    else if(m_runningState == RunningState::Running)
        setProperty("runningState","running");
    else
        setProperty("runningState","done");

    style()->unpolish(this);
    style()->polish(this);
    m_centralWidget->style()->unpolish(m_centralWidget);
    m_centralWidget->style()->polish(m_centralWidget);
}

void AbstractActionWidget::setAction(AbstractAction *action)
{
    m_action = action;
    if(action != nullptr)
        m_actionID = action->getID();
}
