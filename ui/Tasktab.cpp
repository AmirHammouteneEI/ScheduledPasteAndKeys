#include "TaskTab.h"
#include "TaskThread.h"
#include "ui/actionwidgets/PasteWidget.h"
#include "ui/actionwidgets/WaitWidget.h"
#include "globals.h"
#include "actions/PasteAction.h"
#include "actions/WaitAction.h"

#include <QGridLayout>
#include <QSpacerItem>
#include <QApplication>
#include <QMenu>
#include <QSettings>

TaskTab::TaskTab(QWidget *parent, const QString &name)
    : QScrollArea{parent},m_name(name)
{
    m_ID = -1;

    buildBasicInterface();

    m_scheduleState = ScheduleState::NotScheduled;

    m_actionWidgetsManager = new ActionWidgetsManager(m_actionsLayout);

    m_scheduleTimer = new QTimer(this);
    m_scheduleTimer->setSingleShot(true);
    connect(m_scheduleTimer, &QTimer::timeout, this, &TaskTab::runTaskThread);
}

TaskTab::~TaskTab()
{
    if(m_task != nullptr)
    {
        delete m_task;
        m_task = nullptr;
    }

    m_getDelayDialog->deleteLater();
    m_mainWidget->deleteLater();
    layout()->deleteLater();
    m_actionWidgetsManager->deleteLater();
}

void TaskTab::buildBasicInterface()
{
    m_createPasteActionDialog = new CreatePasteActionDialog(this);

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
    m_stopButton->setToolTip(tr("Stop the task. shortcut: Ctrl+Alt+S"));
    m_delayChrono = new QLabel(tr("not scheduled"),topWidget);
    m_delayChrono->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    m_loopButton = new QToolButton(topWidget);
    m_loopButton->setObjectName("loopButton");
    m_loopButton->setText(tr("Loop OFF"));
    m_loopButton->setCheckable(true);
    auto font = m_nameLabel->font();
    font.setBold(true);
    font.setPointSize(13);
    m_nameLabel->setFont(font);
    font.setPointSize(16);
    m_scheduleButton->setFont(font);
    m_stopButton->setFont(font);
    font.setPointSize(12);
    m_loopButton->setFont(font);
    font.setPointSize(11);
    font.setBold(false);
    m_delayChrono->setFont(font);
    topGridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum),0,0);
    topGridLayout->addWidget(m_nameLabel,0,1,1,2, Qt::AlignCenter | Qt::AlignHCenter);
    topGridLayout->addWidget(m_scheduleButton,1,1, Qt::AlignCenter | Qt::AlignRight);
    topGridLayout->addWidget(m_stopButton,1,2, Qt::AlignCenter | Qt::AlignLeft);
    topGridLayout->addWidget(m_delayChrono,2,1,1,2, Qt::AlignCenter | Qt::AlignHCenter);
    topGridLayout->addWidget(m_loopButton,3,1,1,2, Qt::AlignCenter | Qt::AlignHCenter);
    m_stopButton->setFont(font);topGridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::MinimumExpanding,QSizePolicy::Minimum),3,3);
    topGridLayout->setContentsMargins(1,1,1,1);
    topGridLayout->setSpacing(2);

    //-- Middle widget with list of actions
    m_actionsFrame = new QFrame(m_mainWidget);
    m_actionsFrame->setObjectName("actionFrame");
    m_actionsLayout = new QVBoxLayout(m_actionsFrame);
    m_actionsLayout->setSizeConstraint(QLayout::SetMinimumSize);

    //-- Botton widget with + button and loop button
    auto bottomWidget = new QFrame(m_mainWidget);
    auto bottomGridLayout = new QGridLayout(bottomWidget);
    m_addActionButton = new QToolButton(bottomWidget);
    m_addActionButton->setObjectName("addActionButton");
    m_addActionButton->setText("+");
    buildAddButtonMenu();
    //m_addActionButton->setContextMenuPolicy(Qt::CustomContextMenu);
    font.setPointSize(14);
    font.setBold(true);
    m_addActionButton->setFont(font);
    bottomGridLayout->addWidget(m_addActionButton,0,0,Qt::AlignCenter | Qt::AlignHCenter);

    m_mainWidget->layout()->addWidget(topWidget);
    m_mainWidget->layout()->addWidget(m_actionsFrame);
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

    connect(m_createPasteActionDialog, &CreatePasteActionDialog::createPasteActionRequest, this, &TaskTab::createPasteActionRequest);
}

void TaskTab::buildAddButtonMenu()
{
    auto menu = new QMenu(m_addActionButton);
    auto creaPasteAct = new QAction(tr("Add Paste Action..."),menu);
    auto creaWaitAct = new QAction(tr("Add Wait Action..."), menu);
    menu->addAction(creaPasteAct);
    menu->addAction(creaWaitAct);
    connect(creaPasteAct, &QAction::triggered, m_createPasteActionDialog, &CreatePasteActionDialog::showDialog);

    m_addActionButton->setMenu(menu);
    m_addActionButton->setPopupMode(QToolButton::InstantPopup);
}

void TaskTab::setTask(Task *task)
{
    if(m_task != nullptr)
    {
        delete m_task;
        m_task = nullptr;
    }

    m_task = task;

    for(auto it = m_task->m_actionsOrderedList.begin(); it != m_task->m_actionsOrderedList.end(); ++it)
    {
        m_actionWidgetsManager->appendWidget(createActionWidget((*it)));
    }

    m_actionWidgetsManager->fullRefreshActionWidgets();
}

void TaskTab::appendAction(AbstractAction *act)
{
    if(act == nullptr || m_task == nullptr)
        return;

    m_task->appendAction(act);
    m_actionWidgetsManager->appendWidget(createActionWidget(act));

    m_actionWidgetsManager->fullRefreshActionWidgets();
}

AbstractActionWidget *TaskTab::createActionWidget(AbstractAction *act)
{
    if(act == nullptr)
        return nullptr;

    AbstractActionWidget *actWidgToCreate = nullptr;
    switch (act->m_type)
    {
        case ActionType::Paste:
            actWidgToCreate = new PasteWidget(m_actionsFrame);
        break;
        case ActionType::Wait:
            actWidgToCreate = new WaitWidget(m_actionsFrame);
        break;
        default:
            return nullptr;
        break;
    }

    actWidgToCreate->setAction(act);
    actWidgToCreate->buildWidget();
    connect(actWidgToCreate, &AbstractActionWidget::removeActionRequest, this, &TaskTab::removeActionReceived);
    connect(actWidgToCreate, &AbstractActionWidget::moveToTopActionRequest, this, &TaskTab::moveToTopActionReceived);
    connect(actWidgToCreate, &AbstractActionWidget::moveToBottomActionRequest, this, &TaskTab::moveToBottomActionReceived);
    connect(actWidgToCreate, &AbstractActionWidget::moveUpActionRequest, this, &TaskTab::moveUpActionReceived);
    connect(actWidgToCreate, &AbstractActionWidget::moveDownActionRequest, this, &TaskTab::moveDownActionReceived);

    return actWidgToCreate;
}

void TaskTab::setName(const QString &newname)
{
    m_name = newname;
    m_nameLabel->setText(m_name);
}

void TaskTab::scheduleTaskAfterDelay(qint64 delayInSeconds)
{
    m_datetimeOfRun = QDateTime::currentDateTime().addSecs(delayInSeconds);
    m_scheduleTimer->setInterval((std::chrono::milliseconds)(delayInSeconds*1000));
    m_scheduleTimer->start();
    m_scheduleState = ScheduleState::ScheduledInDelay;

    m_scheduleButton->setEnabled(false);
    m_stopButton->setEnabled(true);
    m_addActionButton->setEnabled(false);
    m_loopButton->setEnabled(false);

    refreshScheduleText();

    m_actionWidgetsManager->taskScheduled();
}

void TaskTab::runTaskThread()
{
    if(m_scheduleState == ScheduleState::NotScheduled)
        return;

    TaskThread* thread = new TaskThread();

    thread->copyActionsList(m_task);
    thread->m_loop = m_loopButton->isChecked();

    connect(m_stopButton, &QPushButton::released, thread, &TaskThread::stop);
    connect(thread, &TaskThread::sendRunningStateAct, this, &TaskTab::receivedActionRunningState);
    connect(thread, &TaskThread::sendDoneStateAct, m_actionWidgetsManager, &ActionWidgetsManager::receivedActionDoneState);
    connect(thread, &TaskThread::sendFinishedOneLoop, this, &TaskTab::finishedOneLoop);
    connect(thread, &TaskThread::finished, this, &TaskTab::stopPushed);
    connect(thread, &TaskThread::finished, thread, &TaskThread::deleteLater);
    connect(qApp, &QApplication::aboutToQuit, thread, &TaskThread::stop);

    thread->start();
    m_scheduleState = ScheduleState::Running;
}

void TaskTab::stopPushed()
{
    // Stop button is also connect to quitting the thread in TaskTab::runTaskThread()
    m_scheduleTimer->stop();
    m_scheduleState = ScheduleState::NotScheduled;
    m_scheduleButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_addActionButton->setEnabled(true);
    m_loopButton->setEnabled(true);
    refreshScheduleText();
    m_actionWidgetsManager->taskStopped();
}

void TaskTab::finishedOneLoop()
{
    m_actionWidgetsManager->taskStopped();
    m_actionWidgetsManager->taskScheduled();
}

void TaskTab::receivedActionRunningState(unsigned int actId)
{
    auto widg = m_actionWidgetsManager->m_actionWidgetsMap.value(actId);
    if(widg != nullptr)
        ensureWidgetVisible(widg,0,15);

    m_actionWidgetsManager->receivedActionRunningState(actId);
}

void TaskTab::loopToggled(bool state)
{
    if(state)
    {
        m_loopButton->setText(tr("Loop ON"));
    }
    else
    {
        m_loopButton->setText(tr("Loop OFF"));
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
        m_delayChrono->setStyleSheet("color: green;");
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
        m_delayChrono->setText(tr("Scheduled to run in :\n")
            +QString::number(days)+tr(" days ")+QString::number(hours)+tr(" hours ")
            +QString::number(mins)+tr(" mins ")+QString::number(secs)+tr(" secs"));
        m_delayChrono->setStyleSheet("color: #4169E1;");
        QTimer::singleShot(200, this, &TaskTab::refreshScheduleText);
    }
}

void TaskTab::removeActionReceived(unsigned int actId)
{
    if(m_task == nullptr)
        return;

    auto act = m_task->m_actionsMap.take(actId);
    m_task->m_actionsOrderedList.removeOne(act);
    if(act != nullptr)
    {
        delete act;
        act = nullptr;
    }

    auto actWidg = m_actionWidgetsManager->m_actionWidgetsMap.take(actId);
    m_actionWidgetsManager->m_actionWidgetsDisplayOrderedList.removeOne(actWidg);
    if(actWidg != nullptr)
        actWidg->deleteLater();

    m_actionWidgetsManager->fullRefreshActionWidgets();
}

void TaskTab::moveToTopActionReceived(unsigned int actId)
{
    if(m_task == nullptr)
        return;

    auto act = m_task->m_actionsMap.value(actId, nullptr);
    if(act == nullptr)
        return;
    int indexAct = m_task->m_actionsOrderedList.indexOf(act);
    if(indexAct == -1)
        return;

    m_task->m_actionsOrderedList.move(indexAct,0);

    auto actWidg = m_actionWidgetsManager->m_actionWidgetsMap.value(actId, nullptr);
    if(actWidg == nullptr)
        return;
    int indexWidg = m_actionWidgetsManager->m_actionWidgetsDisplayOrderedList.indexOf(actWidg);
    if(indexWidg == -1)
        return;

    m_actionWidgetsManager->m_actionWidgetsDisplayOrderedList.move(indexWidg,0);

    m_actionWidgetsManager->fullRefreshActionWidgets();

    qApp->processEvents();

    ensureWidgetVisible(actWidg,0,15);
}

void TaskTab::moveToBottomActionReceived(unsigned int actId)
{
    if(m_task == nullptr)
        return;

    auto act = m_task->m_actionsMap.value(actId, nullptr);
    if(act == nullptr)
        return;
    int indexAct = m_task->m_actionsOrderedList.indexOf(act);
    if(indexAct == -1)
        return;

    m_task->m_actionsOrderedList.move(indexAct,m_task->m_actionsOrderedList.size()-1);

    auto actWidg = m_actionWidgetsManager->m_actionWidgetsMap.value(actId, nullptr);
    if(actWidg == nullptr)
        return;
    int indexWidg = m_actionWidgetsManager->m_actionWidgetsDisplayOrderedList.indexOf(actWidg);
    if(indexWidg == -1)
        return;

    m_actionWidgetsManager->m_actionWidgetsDisplayOrderedList.move(indexWidg,m_actionWidgetsManager->m_actionWidgetsDisplayOrderedList.size()-1);

    m_actionWidgetsManager->fullRefreshActionWidgets();

    qApp->processEvents();

    ensureWidgetVisible(actWidg,0,15);
}

void TaskTab::moveUpActionReceived(unsigned int actId)
{
    if(m_task == nullptr)
        return;

    auto act = m_task->m_actionsMap.value(actId, nullptr);
    if(act == nullptr)
        return;
    int indexAct = m_task->m_actionsOrderedList.indexOf(act);
    if(indexAct <= 0)
        return;

    m_task->m_actionsOrderedList.move(indexAct,indexAct-1);

    auto actWidg = m_actionWidgetsManager->m_actionWidgetsMap.value(actId, nullptr);
    if(actWidg == nullptr)
        return;
    int indexWidg = m_actionWidgetsManager->m_actionWidgetsDisplayOrderedList.indexOf(actWidg);
    if(indexWidg <= 0)
        return;

    m_actionWidgetsManager->m_actionWidgetsDisplayOrderedList.move(indexWidg,indexWidg-1);

    m_actionWidgetsManager->fullRefreshActionWidgets();

    qApp->processEvents();

    ensureWidgetVisible(actWidg,0,15);
}

void TaskTab::moveDownActionReceived(unsigned int actId)
{
    if(m_task == nullptr)
        return;

    auto act = m_task->m_actionsMap.value(actId, nullptr);
    if(act == nullptr)
        return;
    int indexAct = m_task->m_actionsOrderedList.indexOf(act);
    if(indexAct == -1 || indexAct >= (m_task->m_actionsOrderedList.size()-1))
        return;

    m_task->m_actionsOrderedList.move(indexAct,indexAct+1);

    auto actWidg = m_actionWidgetsManager->m_actionWidgetsMap.value(actId, nullptr);
    if(actWidg == nullptr)
        return;
    int indexWidg = m_actionWidgetsManager->m_actionWidgetsDisplayOrderedList.indexOf(actWidg);
    if(indexWidg == -1 || indexWidg >= (m_actionWidgetsManager->m_actionWidgetsDisplayOrderedList.size()-1))
        return;

    m_actionWidgetsManager->m_actionWidgetsDisplayOrderedList.move(indexWidg,indexWidg+1);

    m_actionWidgetsManager->fullRefreshActionWidgets();

    qApp->processEvents();

    ensureWidgetVisible(actWidg,0,15);
}

void TaskTab::createPasteActionRequest(QString sentenceIdentity, float addWaitActionSeconds)
{
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    QString content = settings.value(G_Files::SentencesDataCategory+sentenceIdentity).toString();

    ActionParameters paramPaste;
    paramPaste.m_pasteContent = content;
    paramPaste.m_dataId = sentenceIdentity;

    PasteAction *pasteAct = new PasteAction();
    pasteAct->setParameters(paramPaste);

    appendAction(pasteAct);

    if(addWaitActionSeconds < 0.f)
        return;

    ActionParameters paramWait;
    paramWait.m_waitDuration = addWaitActionSeconds;

    WaitAction *waitAct = new WaitAction();
    waitAct->setParameters(paramWait);

    appendAction(waitAct);
}
