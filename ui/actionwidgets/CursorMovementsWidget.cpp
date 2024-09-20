#include "CursorMovementsWidget.h"
#include "actions/CursorMovementsAction.h"
#include "globals.h"
#include "actions/ActionsTools.h"

#include <QGridLayout>
#include <QPushButton>
#include <QSettings>
#include <QTimer>

CursorMovementsWidget::CursorMovementsWidget(QWidget *parent)
    : AbstractActionWidget{parent}
{

}

void CursorMovementsWidget::buildWidget()
{
    auto centralGridLayout = qobject_cast<QGridLayout*>(m_centralWidget->layout());
    if(m_centralWidget == nullptr || centralGridLayout == nullptr)
        return;

    auto loopFrame = new QFrame(m_centralWidget);
    auto loopLabel = new QLabel(tr("Loop "),loopFrame);
    m_loopSpin = new QSpinBox(loopFrame);
    auto looptimesLabel = new QLabel(tr(" times"),loopFrame);
    m_loopSpin->setAlignment(Qt::AlignCenter);
    m_loopSpin->setMinimum(1);
    m_loopSpin->setMaximum(9999);
    m_loopSpin->setMinimumWidth(60);
    m_loopSpin->setToolTip(tr("Set how many times this cursor movements should be executed"));

    auto loopLayout = new QHBoxLayout(loopFrame);
    loopLayout->addWidget(loopLabel,0,Qt::AlignVCenter | Qt::AlignRight);
    loopLayout->addWidget(m_loopSpin,0,Qt::AlignCenter);
    loopLayout->addWidget(looptimesLabel,0,Qt::AlignVCenter | Qt::AlignLeft);
    loopLayout->setContentsMargins(1,1,1,1);
    loopLayout->setSpacing(2);

    centralGridLayout->removeWidget(m_infoLabel);
    centralGridLayout->addWidget(loopFrame,1,0,Qt::AlignCenter);
    centralGridLayout->addWidget(m_infoLabel,2,0,Qt::AlignCenter);

    auto cursormovsaction =  dynamic_cast<CursorMovementsAction*>(m_action);

    QString seqStr = tr("ERROR on access to action");
    QString id = tr("ERROR");
    if(cursormovsaction != nullptr)
    {
        id = cursormovsaction->m_movementsId;
        seqStr = ActionsTools::fromCursorMovsMapToPrintedString(cursormovsaction->m_cursorMovementsMap);
        m_loopSpin->setValue(cursormovsaction->m_timesToRun);
    }

    m_mainButton->setIcon(QIcon(":/img/key.png"));
    m_mainButton->setText(tr("Cursor movements ~")+id);
    m_mainButton->setToolTip(seqStr);
    m_mainButton->setProperty("cursorMovsId", id);

    //m_createKeysSeqActionDialog = new CreateKeysSequenceActionDialog(m_centralWidget);

    //connect(m_mainButton, &QPushButton::released, m_createKeysSeqActionDialog, &CreateKeysSequenceActionDialog::showDialog);
    //connect(m_createKeysSeqActionDialog, &CreateKeysSequenceActionDialog::sendKeysSequence, this, &KeysSequenceWidget::keysSeqIdentityReceived);
    connect(m_loopSpin, &QSpinBox::valueChanged, this, &CursorMovementsWidget::timesToRunChanged);

}

void CursorMovementsWidget::cursorMovsIdentityReceived(QString id)
{
    auto cursormovsaction =  dynamic_cast<CursorMovementsAction*>(m_action);
    if(cursormovsaction == nullptr)
    {
        m_mainButton->setToolTip("ERROR on access to action");
        m_mainButton->setText("Cursor movements ~ERROR");
        return;
    }
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    auto cursorMovementsFromSettings = settings.value(G_Files::CursorMovementsDataCategory+id).toMap();
    DelaysMovementsMap movsMap = ActionsTools::fromStandardQMapToCursorMovsMap(cursorMovementsFromSettings);
    cursormovsaction->m_cursorMovementsMap = movsMap;
    cursormovsaction->m_movementsId = id;
    m_mainButton->setText(tr("Cursor movements ~")+id);
    m_mainButton->setToolTip(ActionsTools::fromCursorMovsMapToPrintedString(movsMap));
    m_mainButton->setProperty("cursorMovsId", id);

    emit anyParamChanged();
}

void CursorMovementsWidget::timesToRunChanged(int times)
{
    auto cursormovsaction =  dynamic_cast<CursorMovementsAction*>(m_action);
    cursormovsaction->m_timesToRun = times;
    emit anyParamChanged();
}

void CursorMovementsWidget::changedRunningState()
{
    refreshLoopsRemainingText(QDateTime::currentDateTime());
}

void CursorMovementsWidget::refreshLoopsRemainingText(const QDateTime &departureDate)
{
    if(m_runningState == RunningState::NotExecuted || m_runningState == RunningState::Done)
    {
        m_infoLabel->setText("");
        return;
    }

    auto cursormovsaction =  dynamic_cast<CursorMovementsAction*>(m_action);
    if(cursormovsaction == nullptr)
        return;

    int oneExecutionDuration = cursormovsaction->computeOneExecutionDuration();
    if(oneExecutionDuration == 0)
        return;

    qint64 timelaps = departureDate.msecsTo(QDateTime::currentDateTime());
    int timesExecuted = timelaps / oneExecutionDuration;

    m_infoLabel->setText(tr("Remaining ")+QString::number(cursormovsaction->m_timesToRun - timesExecuted)+tr(" executions"));

    QTimer::singleShot(200, this, [=]() {refreshLoopsRemainingText(departureDate);});

}
