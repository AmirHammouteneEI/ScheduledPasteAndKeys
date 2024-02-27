#include "AbstractActionWidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QStyle>

AbstractActionWidget::AbstractActionWidget(QWidget *parent)
    : QWidget{parent}
{
    setProperty("runningState","notExecuted");

    auto gridLayout = new QGridLayout(this);
    m_centralWidget = new QWidget(this);
    auto removeButton = new QPushButton(QIcon(":/img/close_red.png"),"",this);

    gridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum),0,0);
    gridLayout->addWidget(m_centralWidget,0,1,Qt::AlignCenter | Qt::AlignHCenter);
    gridLayout->addWidget(removeButton,0,2,Qt::AlignRight | Qt::AlignTop);
    connect(removeButton,&QPushButton::released, this, &AbstractActionWidget::removeSelf);
}

void AbstractActionWidget::setRunningState(const QString &state)
{
    setProperty("runningState",state);
    style()->unpolish(this);
    style()->polish(this);
}
