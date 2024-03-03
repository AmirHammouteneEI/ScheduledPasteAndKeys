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

    auto pasteaction =  dynamic_cast<WaitAction*>(m_action);

    QString durationStr = "ERROR on access duration";
    if(pasteaction != nullptr)
        durationStr = QString::number(pasteaction->m_duration);

    auto gridLayout = new QGridLayout(m_centralWidget);
    auto mainButton = new QPushButton(tr("Wait for ") + durationStr + tr(" secs"),m_centralWidget); //TODO icon
    mainButton->setToolTip(durationStr+tr(" seconds"));
    m_timeRemainingLabel = new QLabel("",m_centralWidget);
    m_timeRemainingLabel->setObjectName("actionSubLabel");
    m_timeRemainingLabel->setWordWrap(true);
    m_timeRemainingLabel->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);

    gridLayout->addItem(new QSpacerItem(5,5,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding),0,0);
    gridLayout->addWidget(mainButton,1,1,Qt::AlignCenter | Qt::AlignHCenter);
    gridLayout->addWidget(m_timeRemainingLabel,2,1,Qt::AlignCenter | Qt::AlignHCenter);
    gridLayout->addItem(new QSpacerItem(5,5,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding),3,2);

    gridLayout->setContentsMargins(1,1,1,1);
    gridLayout->setSpacing(2);
}

void WaitWidget::changedRunningState()
{
    refreshTimeRemainingText(QDateTime::currentDateTime());
}

void WaitWidget::refreshTimeRemainingText(const QDateTime &departureDate)
{
    if(m_runningState == RunningState::NotExecuted || m_runningState == RunningState::Done)
    {
        m_timeRemainingLabel->setText("");
        return;
    }

    auto pasteaction =  dynamic_cast<WaitAction*>(m_action);
    if(pasteaction == nullptr)
        return;

    QDateTime finishDate = departureDate.addSecs(pasteaction->m_duration);
    qint64 dateDelay = QDateTime::currentDateTime().secsTo(finishDate);

    if(dateDelay < 5)
    {
        m_timeRemainingLabel->setText(tr("Remaining less than 5 seconds"));
        return;
    }

    qint64 secs = dateDelay % 60;
    qint64 minNum = (dateDelay-secs)/60;
    qint64 mins = minNum % 60;
    qint64 hourNum = (minNum-mins)/ 60 ;
    qint64 hours = hourNum %24;
    qint64 days = (hourNum - hours)/ 24;
    m_timeRemainingLabel->setText(tr("Remaining :\n")
                           +QString::number(days)+tr(" days ")+QString::number(hours)+tr(" hours ")
                           +QString::number(mins)+tr(" mins ")+QString::number(secs)+tr(" secs"));
    QTimer::singleShot(200, this, [=]() {refreshTimeRemainingText(departureDate);});
}
