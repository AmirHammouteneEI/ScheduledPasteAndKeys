#include "TaskTab.h"
#include "TaskThread.h"

#include <QToolButton>
#include <QFrame>
#include <QGridLayout>
#include <QSpacerItem>
#include <QTimer>
#include <QApplication>

TaskTab::TaskTab(QWidget *parent, const QString &name)
    : QScrollArea{parent},m_name(name)
{
    m_ID = -1;

    buildBasicInterface();

    m_scheduleState = ScheduleState::NotScheduled;
}

TaskTab::~TaskTab()
{
    if(m_task != nullptr)
    {
        delete m_task;
        m_task = nullptr;
    }
}

void TaskTab::refreshActionsList()
{

}

void TaskTab::buildBasicInterface()
{
    setWidgetResizable(true);
    new QVBoxLayout(this);

    auto mainWidget = new QWidget(this);
    new QVBoxLayout(mainWidget);

    //-- Top widget with Schedule button
    auto topWidget = new QFrame(mainWidget);
    auto topGridLayout = new QGridLayout(topWidget);
    topGridLayout->setSizeConstraint(QLayout::SetMinimumSize);
    m_nameLabel = new QLabel(m_name,topWidget);
    m_nameLabel->setWordWrap(true);
    m_nameLabel->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    m_nameLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    m_scheduleButton = new QPushButton(tr("Schedule the Task"), topWidget);
    m_scheduleButton->setObjectName("scheduleButton");
    m_stopButton = new QPushButton("■", topWidget);
    m_stopButton->setObjectName("stopButton");
    m_delayChrono = new QLabel("",topWidget);
    m_loopState = new QLabel(tr("Loop Off"),topWidget);
    auto font = m_nameLabel->font();
    font.setBold(true);
    font.setPointSize(13);
    m_nameLabel->setFont(font);
    font.setPointSize(16);
    m_scheduleButton->setFont(font);
    m_stopButton->setFont(font);topGridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum),0,0);
    topGridLayout->addWidget(m_nameLabel,0,1,1,2, Qt::AlignCenter | Qt::AlignHCenter);
    topGridLayout->addWidget(m_scheduleButton,1,1, Qt::AlignCenter | Qt::AlignRight);
    topGridLayout->addWidget(m_stopButton,1,2, Qt::AlignCenter | Qt::AlignLeft);
    topGridLayout->addWidget(m_delayChrono,2,1,1,2, Qt::AlignCenter | Qt::AlignHCenter);
    topGridLayout->addWidget(m_loopState,3,1,1,2, Qt::AlignCenter | Qt::AlignHCenter);
    m_stopButton->setFont(font);topGridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum),3,3);
    topGridLayout->setContentsMargins(1,1,1,1);
    topGridLayout->setSpacing(2);

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

    m_stopButton->setEnabled(false);

    m_getDelayDialog = new getDelayDialog(this);
    m_getDelayDialog->m_savedDelay = QTime(0,1,0);

    connect(m_scheduleButton,&QPushButton::released, m_getDelayDialog, &getDelayDialog::showDialog);
    connect(m_stopButton,&QPushButton::released, this, &TaskTab::stopPushed);
    connect(m_getDelayDialog,&getDelayDialog::sendDelay, this, &TaskTab::scheduleTaskAfterDelay);
}

void TaskTab::setTask(Task *task)
{
    if(m_task != nullptr)
    {
        delete m_task;
        m_task = nullptr;
    }

    m_task = task;
}

void TaskTab::setName(const QString &newname)
{
    m_name = newname;
    m_nameLabel->setText(m_name);
}

void TaskTab::scheduleTaskAfterDelay(int delayInSeconds)
{
    QTimer::singleShot(delayInSeconds*1000, this, &TaskTab::runTaskThread);
    m_scheduleState = ScheduleState::ScheduledInDelay;

    m_scheduleButton->setEnabled(false);
    m_stopButton->setEnabled(true);
}

void TaskTab::runTaskThread()
{
    if(m_scheduleState == ScheduleState::NotScheduled)
        return;

    TaskThread* thread = new TaskThread(this);

    thread->m_task = m_task;

    connect(m_stopButton, &QPushButton::released, thread, &TaskThread::stop);
    connect(thread, &TaskThread::finished, this, &TaskTab::stopPushed);
    connect(thread, &TaskThread::finished, thread, &TaskThread::quit);
    connect(thread, &TaskThread::finished, thread, &TaskThread::deleteLater);
    thread->start();
    m_scheduleState = ScheduleState::Running;
}

void TaskTab::stopPushed()
{
    // Stop button is also connect to quitting the thread in TaskTab::runTaskThread()
    m_scheduleState = ScheduleState::NotScheduled;
    m_scheduleButton->setEnabled(true);
    m_stopButton->setEnabled(false);
}

