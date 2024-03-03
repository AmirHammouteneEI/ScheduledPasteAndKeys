#include "AbstractActionWidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QStyle>
#include <QMessageBox>

AbstractActionWidget::AbstractActionWidget(QWidget *parent)
    : QFrame{parent}
{
    auto gridLayout = new QGridLayout(this);
    m_centralWidget = new QFrame(this);

    auto removeButton = new QPushButton(QIcon(":/img/close_red.png"),"",this);
    removeButton->setMaximumSize(QSize(20,20));
    removeButton->setFlat(true);
    auto moveToTopButton = new QPushButton(QIcon(":/img/top_arrow_rounded.png"),"",this);
    moveToTopButton->setMaximumSize(QSize(20,20));
    moveToTopButton->setFlat(true);
    auto moveToBottomButton = new QPushButton(QIcon(":/img/bottom_arrow_rounded.png"),"",this);
    moveToBottomButton->setMaximumSize(QSize(20,20));
    moveToBottomButton->setFlat(true);
    auto moveUpButton = new QPushButton(QIcon(":/img/up_arrow_rounded.png"),"",this);
    moveUpButton->setMaximumSize(QSize(20,20));
    moveUpButton->setFlat(true);
    auto moveDownButton = new QPushButton(QIcon(":/img/down_arrow_rounded.png"),"",this);
    moveDownButton->setMaximumSize(QSize(20,20));
    moveDownButton->setFlat(true);

    QGridLayout *rightLayout = new QGridLayout();
    rightLayout->addWidget(removeButton,0,1);
    rightLayout->addWidget(moveToTopButton,1,1);
    rightLayout->addWidget(moveToBottomButton,2,1);
    rightLayout->addWidget(moveUpButton,1,0);
    rightLayout->addWidget(moveDownButton,2,0);

    gridLayout->addItem(new QSpacerItem(3,3,QSizePolicy::Minimum,QSizePolicy::Minimum),0,0);
    gridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum),1,0);
    gridLayout->addWidget(m_centralWidget,1,1,Qt::AlignCenter | Qt::AlignHCenter);
    gridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum),1,2);
    gridLayout->addWidget(removeButton,1,3,Qt::AlignRight | Qt::AlignTop);
    gridLayout->addLayout(rightLayout,1,3,Qt::AlignRight | Qt::AlignTop);
    gridLayout->addItem(new QSpacerItem(3,3,QSizePolicy::Minimum,QSizePolicy::Minimum),2,0);


    gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
    gridLayout->setContentsMargins(1,1,1,1);
    gridLayout->setSpacing(2);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    setRunningState(RunningState::NotExecuted);
    connect(removeButton,&QPushButton::released, this, &AbstractActionWidget::removeSelf);
    connect(moveToTopButton,&QPushButton::released, this, &AbstractActionWidget::moveToTopSelf);
    connect(moveToBottomButton,&QPushButton::released, this, &AbstractActionWidget::moveToBottomSelf);
    connect(moveUpButton,&QPushButton::released, this, &AbstractActionWidget::moveUpSelf);
    connect(moveDownButton,&QPushButton::released, this, &AbstractActionWidget::moveDownSelf);
}

AbstractActionWidget::~AbstractActionWidget() = default;

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

void AbstractActionWidget::removeSelf()
{
    if(QMessageBox::question(this, tr("Confirm removing action"), tr("You are about to remove this action from Task, are you sure ?"),
        QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
        return;

    emit removeActionRequest(m_actionID);
}

void AbstractActionWidget::moveToTopSelf()
{
    emit moveToTopActionRequest(m_actionID);
}

void AbstractActionWidget::moveToBottomSelf()
{
    emit moveToBottomActionRequest(m_actionID);
}

void AbstractActionWidget::moveUpSelf()
{
    emit moveUpActionRequest(m_actionID);
}

void AbstractActionWidget::moveDownSelf()
{
    emit moveDownActionRequest(m_actionID);
}
