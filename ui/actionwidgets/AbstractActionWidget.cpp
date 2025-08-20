#include "AbstractActionWidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QStyle>
#include <QMessageBox>

AbstractActionWidget::AbstractActionWidget(QWidget *parent)
    : QFrame{parent}
{
    m_runningState = RunningState::NotExecuted;
    setProperty("runningState","notExecuted");

    auto gridLayout = new QGridLayout(this);
    m_centralWidget = new QFrame(this);

    //m_centralWidget->setMinimumHeight(90);

    auto centralGridLayout = new QGridLayout(m_centralWidget);
    m_mainButton = new QPushButton(m_centralWidget);
    m_mainButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_mainButton->setMinimumWidth(200);

    m_infoLabel = new QLabel("",m_centralWidget);
    m_infoLabel->setObjectName("actionSubLabel");
    m_infoLabel->setWordWrap(true);
    m_infoLabel->setAlignment(Qt::AlignCenter);
    m_infoLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_infoLabel->setMinimumWidth(200);

    centralGridLayout->addWidget(m_mainButton,0,0,Qt::AlignCenter);
    centralGridLayout->addWidget(m_infoLabel,1,0,Qt::AlignCenter);

    centralGridLayout->setContentsMargins(1,1,1,1);
    centralGridLayout->setSpacing(2);
    centralGridLayout->setSizeConstraint(QLayout::SetMinimumSize);
    m_centralWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    m_removeButton = new QPushButton(QIcon(":/img/close_red.png"),"",this);
    m_removeButton->setMaximumSize(QSize(20,20));
    m_removeButton->setFlat(true);
    m_removeButton->setToolTip(tr("Remove this action from the task..."));
    m_moveToTopButton = new QPushButton(QIcon(":/img/top_arrow_rounded.png"),"",this);
    m_moveToTopButton->setMaximumSize(QSize(20,20));
    m_moveToTopButton->setFlat(true);
    m_moveToTopButton->setToolTip(tr("Move this action to the top"));
    m_moveToBottomButton = new QPushButton(QIcon(":/img/bottom_arrow_rounded.png"),"",this);
    m_moveToBottomButton->setMaximumSize(QSize(20,20));
    m_moveToBottomButton->setFlat(true);
    m_moveToBottomButton->setToolTip(tr("Move this action to the bottom"));
    m_moveUpButton = new QPushButton(QIcon(":/img/up_arrow_rounded.png"),"",this);
    m_moveUpButton->setMaximumSize(QSize(20,20));
    m_moveUpButton->setFlat(true);
    m_moveUpButton->setToolTip(tr("Move this action up"));
    m_moveDownButton = new QPushButton(QIcon(":/img/down_arrow_rounded.png"),"",this);
    m_moveDownButton->setMaximumSize(QSize(20,20));
    m_moveDownButton->setFlat(true);
    m_moveDownButton->setToolTip(tr("Move this action down"));

    QGridLayout *rightLayout = new QGridLayout();
    rightLayout->addWidget(m_moveUpButton,0,0);
    rightLayout->addWidget(m_moveDownButton,1,0);
    rightLayout->addWidget(m_moveToTopButton,0,1);
    rightLayout->addWidget(m_moveToBottomButton,1,1);
    rightLayout->addWidget(m_removeButton,0,2);
    rightLayout->setContentsMargins(1,1,1,1);
    rightLayout->setSpacing(2);
    rightLayout->setSizeConstraint(QLayout::SetMinimumSize);

    gridLayout->addItem(new QSpacerItem(3,3,QSizePolicy::Fixed,QSizePolicy::Fixed),0,0);
    gridLayout->addWidget(m_centralWidget,1,1,Qt::AlignCenter);
    gridLayout->addItem(new QSpacerItem(5,5,QSizePolicy::Fixed,QSizePolicy::Fixed),1,2);
    gridLayout->addLayout(rightLayout,1,3,Qt::AlignRight | Qt::AlignTop);
    gridLayout->addItem(new QSpacerItem(3,3,QSizePolicy::Fixed,QSizePolicy::Fixed),2,2);
    gridLayout->setContentsMargins(1,1,1,1);
    gridLayout->setSpacing(2);
    gridLayout->setColumnStretch(1,1);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    gridLayout->setSizeConstraint(QLayout::SetMinimumSize);

    connect(m_removeButton,&QPushButton::released, this, &AbstractActionWidget::removeSelf);
    connect(m_moveToTopButton,&QPushButton::released, this, &AbstractActionWidget::moveToTopSelf);
    connect(m_moveToBottomButton,&QPushButton::released, this, &AbstractActionWidget::moveToBottomSelf);
    connect(m_moveUpButton,&QPushButton::released, this, &AbstractActionWidget::moveUpSelf);
    connect(m_moveDownButton,&QPushButton::released, this, &AbstractActionWidget::moveDownSelf);
}

AbstractActionWidget::~AbstractActionWidget()
{
    layout()->deleteLater();
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

    changedRunningState();
}

void AbstractActionWidget::setAction(const std::shared_ptr<AbstractAction> &action)
{
    m_action = action;
    if(action != nullptr)
        m_actionID = action->getID();
}

void AbstractActionWidget::removeSelf()
{
    if(QMessageBox::question(this, tr("Confirm removing action"), tr("You are about to remove this action from the task, are you sure ?"),
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
