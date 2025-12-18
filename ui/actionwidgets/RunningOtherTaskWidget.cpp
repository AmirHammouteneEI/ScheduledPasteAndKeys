#include "RunningOtherTaskWidget.h"

RunningOtherTaskWidget::RunningOtherTaskWidget(QWidget *parent)
    : AbstractActionWidget{parent}
{

}

void RunningOtherTaskWidget::buildWidget()
{
    if(m_centralWidget == nullptr)
        return;

    auto runOtherTaskaction =  dynamic_cast<RunningOtherTaskAction*>(m_action.get());

    QString runOtherTaskStr = tr("ERROR");
    if(runOtherTaskaction != nullptr)
    {
        runOtherTaskStr = tr("Run another task");

        m_mainButton->setToolTip(tr("Task name : ")+runOtherTaskaction->m_taskName+ tr("\nDelay : ")+QString::number(runOtherTaskaction->m_delay)+ tr("\nLoops : ")+QString::number(runOtherTaskaction->m_timesToRun));
        ActionParameters params = runOtherTaskaction->generateParameters();
        m_mainButton->setProperty("filename", params.m_taskName);
        m_mainButton->setProperty("delay", params.m_delay);
        m_mainButton->setProperty("loops", params.m_timesToRun);
    }

    m_mainButton->setObjectName("runAnotherTaskActionButton");
    m_mainButton->setText(runOtherTaskStr);

    m_editRunOtherTaskDialog = new CreateRunningOtherTaskActionDialog(this);

    connect(m_mainButton, &QPushButton::released, m_editRunOtherTaskDialog, &CreateRunningOtherTaskActionDialog::showDialog);
    connect(m_editRunOtherTaskDialog, &CreateRunningOtherTaskActionDialog::sendRunningOtherTask, this, &RunningOtherTaskWidget::runningOtherTaskReceived);
}

void RunningOtherTaskWidget::runningOtherTaskReceived(QString filename, int delay, int timesOfLoop)
{
    auto runOtherTaskaction =  dynamic_cast<RunningOtherTaskAction*>(m_action.get());
    if(runOtherTaskaction == nullptr)
    {
        m_mainButton->setToolTip("ERROR on access to action");
        m_mainButton->setText("ERROR");
        return;
    }

    ActionParameters params;
    params.m_taskName = filename;
    params.m_delay = delay;
    params.m_timesToRun = timesOfLoop;

    runOtherTaskaction->setParameters(params);

    m_mainButton->setToolTip(tr("Task name : ")+filename+ tr("\nDelay : ")+QString::number(delay)+ tr("\nLoops : ")+QString::number(timesOfLoop));
    m_mainButton->setProperty("filename", params.m_taskName);
    m_mainButton->setProperty("delay", params.m_delay);
    m_mainButton->setProperty("loops", params.m_timesToRun);

    emit anyParamChanged();
}
