#include "WaitWidget.h"
#include "actions/WaitAction.h"

#include <QGridLayout>
#include <QPushButton>
#include <QTimer>

WaitWidget::WaitWidget(QWidget *parent)
    : AbstractActionWidget{parent}
{

}

void WaitWidget::buildWidget()
{

    if(m_centralWidget == nullptr)
        return;

    auto waitaction =  dynamic_cast<WaitAction*>(m_action);

    QString durationStr = tr("ERROR");
    if(waitaction != nullptr)
        durationStr = QString::number((double)waitaction->m_duration);

    m_mainButton->setIcon(QIcon(":/img/wait.png"));
    m_mainButton->setText(tr("Wait for ") + durationStr + tr(" secs"));
    m_mainButton->setToolTip(durationStr+tr(" seconds"));
    m_mainButton->setProperty("duration", durationStr);

    m_editDurationDialog = new CreateWaitActionDialog(this);

    connect(m_mainButton, &QPushButton::released, m_editDurationDialog, &CreateWaitActionDialog::showDialog);
    connect(m_editDurationDialog, &CreateWaitActionDialog::sendDuration, this, &WaitWidget::durationReceived);

}

void WaitWidget::changedRunningState()
{
    refreshTimeRemainingText(QDateTime::currentDateTime());
}

void WaitWidget::refreshTimeRemainingText(const QDateTime &departureDate)
{
    if(m_runningState == RunningState::NotExecuted || m_runningState == RunningState::Done)
    {
        m_infoLabel->setText("");
        return;
    }

    auto waitaction =  dynamic_cast<WaitAction*>(m_action);
    if(waitaction == nullptr)
        return;

    QDateTime finishDate = departureDate.addSecs(waitaction->m_duration);
    qint64 dateDelay = QDateTime::currentDateTime().secsTo(finishDate);

    if(dateDelay < 5)
    {
        m_infoLabel->setText(tr("Remaining less than 5 seconds"));
        m_infoLabel->setFixedSize(m_infoLabel->sizeHint());
        return;
    }

    qint64 secs = dateDelay % 60;
    qint64 minNum = (dateDelay-secs)/60;
    qint64 mins = minNum % 60;
    qint64 hourNum = (minNum-mins)/ 60 ;
    qint64 hours = hourNum %24;
    qint64 days = (hourNum - hours)/ 24;
    m_infoLabel->setText(QString::number(days)+tr(" days ")+QString::number(hours)+tr(" hours ")
                           +QString::number(mins)+tr(" mins ")+QString::number(secs)+tr(" secs"));
    m_infoLabel->setFixedSize(m_infoLabel->sizeHint());
    QTimer::singleShot(200, this, [=]() {refreshTimeRemainingText(departureDate);});
}

void WaitWidget::durationReceived(long double dur)
{
    auto waitaction =  dynamic_cast<WaitAction*>(m_action);
    if(waitaction == nullptr)
    {
        m_mainButton->setToolTip("ERROR seconds");
        m_mainButton->setText("Wait for ERROR secs");
        return;
    }

    waitaction->m_duration = dur;
    QString durationStr = QString::number((double)dur);
    m_mainButton->setText(tr("Wait for ") + durationStr + tr(" secs"));
    m_mainButton->setToolTip(durationStr + tr(" seconds"));
    m_mainButton->setProperty("duration", durationStr);

    emit anyParamChanged();
}
