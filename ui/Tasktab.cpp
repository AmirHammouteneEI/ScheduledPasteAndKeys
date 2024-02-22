#include "TaskTab.h"

#include <QPushButton>
#include <QToolButton>
#include <QFrame>
#include <QGridLayout>
#include <QSpacerItem>

TaskTab::TaskTab(QWidget *parent, const QString &name)
    : QScrollArea{parent},m_name(name)
{
    m_ID = -1;

    setWidgetResizable(true);
    new QVBoxLayout(this);

    auto mainWidget = new QWidget(this);
    new QVBoxLayout(mainWidget);

    //-- Top widget with Schedule button
    auto topWidget = new QFrame(mainWidget);
    auto topGridLayout = new QGridLayout(topWidget);
    auto scheduleButton = new QPushButton(tr("Schedule the Task"), topWidget);
    scheduleButton->setObjectName("scheduleButton");
    auto font = scheduleButton->font();
    font.setBold(true);
    font.setPointSize(16);
    scheduleButton->setFont(font);
    topGridLayout->addWidget(scheduleButton,0,0, Qt::AlignCenter | Qt::AlignHCenter);

    //-- Middle widget with list of actions
    auto actionsFrame = new QFrame(mainWidget);
    actionsFrame->setObjectName("actionFrame");
    m_actionsLayout =  new QVBoxLayout(actionsFrame);

    //-- Botton widget with + button and loop button
    auto bottomWidget = new QFrame(mainWidget);
    auto bottomGridLayout = new QGridLayout(bottomWidget);
    auto addActionButton = new QPushButton(tr("+"), bottomWidget);
    addActionButton->setObjectName("addActionButton");
    font.setPointSize(14);
    addActionButton->setFont(font);
    bottomGridLayout->addWidget(addActionButton,0,0,Qt::AlignCenter | Qt::AlignHCenter);
    auto loopButton = new QToolButton(bottomWidget);
    loopButton->setObjectName("loopButton");
    loopButton->setText(tr("Loop"));
    loopButton->setCheckable(true);
    font.setPointSize(12);
    loopButton->setFont(font);
    bottomGridLayout->addItem(new QSpacerItem(20,20,QSizePolicy::Minimum,QSizePolicy::Minimum),1,0);
    bottomGridLayout->addWidget(loopButton,2,0, Qt::AlignCenter | Qt::AlignHCenter);

    mainWidget->layout()->addWidget(topWidget);
    mainWidget->layout()->addWidget(actionsFrame);
    mainWidget->layout()->addWidget(bottomWidget);
    mainWidget->layout()->addItem(new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Expanding));

    setWidget(mainWidget);

    setAlignment(Qt::AlignTop | Qt::AlignHCenter);
}
