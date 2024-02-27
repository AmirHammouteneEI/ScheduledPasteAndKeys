#include "TaskTab.h"
#include "TaskThread.h"

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

    m_mainWidget->deleteLater();
}

void TaskTab::refreshActionsList()
{

}

void TaskTab::buildBasicInterface()
{
    setWidgetResizable(true);
    new QVBoxLayout(this);

    m_mainWidget = new QWidget(this);
    new QVBoxLayout(m_mainWidget);

    //-- Top widget with Schedule button
    auto topWidget = new QFrame(m_mainWidget);
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
    m_delayChrono = new QLabel(tr("not scheduled"),topWidget);
    m_delayChrono->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    m_loopState = new QLabel(tr("Loop OFF"),topWidget);
    auto font = m_nameLabel->font();
    font.setBold(true);
    font.setPointSize(13);
    m_nameLabel->setFont(font);
    font.setPointSize(16);
    m_scheduleButton->setFont(font);
    m_stopButton->setFont(font);
    font.setPointSize(11);
    font.setBold(false);
    m_delayChrono->setFont(font);
    m_loopState->setFont(font);
    topGridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum),0,0);
    topGridLayout->addWidget(m_nameLabel,0,1,1,2, Qt::AlignCenter | Qt::AlignHCenter);
    topGridLayout->addWidget(m_scheduleButton,1,1, Qt::AlignCenter | Qt::AlignRight);
    topGridLayout->addWidget(m_stopButton,1,2, Qt::AlignCenter | Qt::AlignLeft);
    topGridLayout->addWidget(m_delayChrono,2,1,1,2, Qt::AlignCenter | Qt::AlignHCenter);
    topGridLayout->addWidget(m_loopState,3,1,1,2, Qt::AlignCenter | Qt::AlignHCenter);
    m_stopButton->setFont(font);topGridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum),3,3);
    topGridLayout->setContentsMargins(1,1,1,1);
    topGridLayout->setSpacing(2);

    //-- Middle widget with list of actions
    auto actionsFrame = new QFrame(m_mainWidget);
    actionsFrame->setObjectName("actionFrame");
    m_actionsLayout =  new QVBoxLayout(actionsFrame);

    //-- Botton widget with + button and loop button
    auto bottomWidget = new QFrame(m_mainWidget);
    auto bottomGridLayout = new QGridLayout(bottomWidget);
    m_addActionButton = new QPushButton(tr("+"), bottomWidget);
    m_addActionButton->setObjectName("addActionButton");
    font.setPointSize(14);
    font.setBold(true);
    m_addActionButton->setFont(font);
    bottomGridLayout->addWidget(m_addActionButton,0,0,Qt::AlignCenter | Qt::AlignHCenter);
    m_loopButton = new QToolButton(bottomWidget);
    m_loopButton->setObjectName("loopButton");
    m_loopButton->setText(tr("Loop"));
    m_loopButton->setCheckable(true);
    font.setPointSize(12);
    m_loopButton->setFont(font);
    bottomGridLayout->addItem(new QSpacerItem(20,20,QSizePolicy::Minimum,QSizePolicy::Minimum),1,0);
    bottomGridLayout->addWidget(m_loopButton,2,0, Qt::AlignCenter | Qt::AlignHCenter);

    m_mainWidget->layout()->addWidget(topWidget);
    m_mainWidget->layout()->addWidget(actionsFrame);
    m_mainWidget->layout()->addWidget(bottomWidget);
    m_mainWidget->layout()->addItem(new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Expanding));

    setWidget(m_mainWidget);

    setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    m_stopButton->setEnabled(false);

    m_getDelayDialog = new getDelayDialog(this);
    m_getDelayDialog->m_savedDelay = QTime(0,1,0);

    connect(m_scheduleButton,&QPushButton::released, m_getDelayDialog, &getDelayDialog::showDialog);
    connect(m_stopButton,&QPushButton::released, this, &TaskTab::stopPushed);
    connect(m_getDelayDialog,&getDelayDialog::sendDelay, this, &TaskTab::scheduleTaskAfterDelay);
    connect(m_loopButton,&QToolButton::toggled, this, &TaskTab::loopToggled);
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

void TaskTab::scheduleTaskAfterDelay(qint64 delayInSeconds)
{
    m_datetimeOfRun = QDateTime::currentDateTime().addSecs(delayInSeconds);
    QTimer::singleShot((std::chrono::milliseconds)(delayInSeconds*1000), this, &TaskTab::runTaskThread);
    m_scheduleState = ScheduleState::ScheduledInDelay;

    m_scheduleButton->setEnabled(false);
    m_stopButton->setEnabled(true);
    m_addActionButton->setEnabled(false);
    m_loopButton->setEnabled(false);

    refreshScheduleText();
}

void TaskTab::runTaskThread()
{
    if(m_scheduleState == ScheduleState::NotScheduled)
        return;

    TaskThread* thread = new TaskThread();

    thread->m_task = m_task;
    thread->m_loop = m_loopButton->isChecked();

    connect(m_stopButton, &QPushButton::released, thread, &TaskThread::stop);
    connect(thread, &TaskThread::finished, this, &TaskTab::stopPushed);
    connect(thread, &TaskThread::finished, thread, &TaskThread::quit);
    connect(thread, &TaskThread::finished, thread, &TaskThread::deleteLater);
    connect(qApp, &QApplication::aboutToQuit, thread, &TaskThread::stop);

    thread->start();
    m_scheduleState = ScheduleState::Running;
}

void TaskTab::stopPushed()
{
    // Stop button is also connect to quitting the thread in TaskTab::runTaskThread()
    m_scheduleState = ScheduleState::NotScheduled;
    m_scheduleButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_addActionButton->setEnabled(true);
    m_loopButton->setEnabled(true);
    refreshScheduleText();
}

void TaskTab::loopToggled(bool state)
{
    if(state)
    {
        m_loopState->setText(tr("Loop ON"));
        m_loopState->setStyleSheet("color: darkgreen;");
    }
    else
    {
        m_loopState->setText(tr("Loop OFF"));
        m_loopState->setStyleSheet("");
    }
}

void TaskTab::refreshScheduleText()
{
    if(m_scheduleState == ScheduleState::NotScheduled)
    {
        m_delayChrono->setText(tr("not scheduled"));
        m_delayChrono->setStyleSheet("");
    }
    else if(m_scheduleState == ScheduleState::Running)
    {
        m_delayChrono->setText(tr("Running"));
        m_delayChrono->setStyleSheet("color: darkgreen;");
    }
    else
    {
        qint64 dateDelay = QDateTime::currentDateTime().secsTo(m_datetimeOfRun);
        qint64 secs = dateDelay % 60;
        qint64 minNum = (dateDelay-secs)/60;
        qint64 mins = minNum % 60;
        qint64 hourNum = (minNum-mins)/ 60 ;
        qint64 hours = hourNum %24;
        qint64 days = (hourNum - hours)/ 24;
        m_delayChrono->setText(tr("Schedule to run in :\n")
            +QString::number(days)+tr(" days ")+QString::number(hours)+tr(" hours ")
            +QString::number(mins)+tr(" mins ")+QString::number(secs)+tr(" secs"));
        m_delayChrono->setStyleSheet("color: darkblue;");
        QTimer::singleShot(200, this, &TaskTab::refreshScheduleText);
    }
}

