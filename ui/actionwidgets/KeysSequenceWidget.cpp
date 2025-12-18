#include "KeysSequenceWidget.h"
#include "actions/KeysSequenceAction.h"
#include "globals.h"
#include "actions/ActionsTools.h"

#include <QGridLayout>
#include <QPushButton>
#include <QSettings>
#include <QTimer>
#include <QApplication>

KeysSequenceWidget::KeysSequenceWidget(QWidget *parent)
    : AbstractActionWidget{parent}
{

}

void KeysSequenceWidget::buildWidget()
{
    if(m_centralWidget == nullptr)
        return;
    auto centralGridLayout = qobject_cast<QGridLayout*>(m_centralWidget->layout());
    if(centralGridLayout == nullptr)
        return;

    auto loopFrame = new QFrame(this);
    loopFrame->setObjectName("actionSubFrame");
    auto loopLabel = new QLabel(tr("Loop "),this);
    loopLabel->setObjectName("actionSubLabel");
    m_loopSpin = new NoWheelFocusSpinBox(this);
    m_loopSpin->setObjectName("actionSpin");
    auto looptimesLabel = new QLabel(tr(" times"),this);
    looptimesLabel->setObjectName("actionSubLabel");
    m_loopSpin->setAlignment(Qt::AlignCenter);
    m_loopSpin->setMinimum(1);
    m_loopSpin->setMaximum(9999);
    m_loopSpin->setMinimumWidth(60);
    m_loopSpin->setToolTip(tr("Set how many times this keys sequence should be executed"));

    auto loopLayout = new QHBoxLayout(loopFrame);
    loopLayout->addWidget(loopLabel,0,Qt::AlignVCenter | Qt::AlignRight);
    loopLayout->addWidget(m_loopSpin,0,Qt::AlignCenter);
    loopLayout->addWidget(looptimesLabel,0,Qt::AlignVCenter | Qt::AlignLeft);
    loopLayout->setContentsMargins(1,1,1,1);
    loopLayout->setSpacing(2);

    centralGridLayout->removeWidget(m_infoLabel);
    centralGridLayout->addWidget(loopFrame,1,0,Qt::AlignCenter);
    centralGridLayout->addWidget(m_infoLabel,2,0,Qt::AlignCenter);

    auto keysseqaction =  dynamic_cast<KeysSequenceAction*>(m_action.get());

    QString seqStr = tr("ERROR on access to action");
    QString id = tr("ERROR");
    if(keysseqaction != nullptr)
    {
        id = keysseqaction->m_sequenceId;
        seqStr = ActionsTools::fromKeysSeqMapToPrintedString(keysseqaction->m_keysSeqMap);
        m_loopSpin->setValue(keysseqaction->m_timesToRun);
    }

    m_mainButton->setObjectName("keyActionButton");
    m_mainButton->setText(tr("Keys sequence >")+id);
    m_mainButton->setToolTip(seqStr);
    m_mainButton->setProperty("keysSeqId", id);

    m_createKeysSeqActionDialog = new CreateKeysSequenceActionDialog(m_centralWidget);

    connect(m_mainButton, &QPushButton::released, m_createKeysSeqActionDialog, &CreateKeysSequenceActionDialog::showDialog);
    connect(m_createKeysSeqActionDialog, &CreateKeysSequenceActionDialog::sendKeysSequence, this, &KeysSequenceWidget::keysSeqIdentityReceived);
    connect(m_loopSpin, &NoWheelFocusSpinBox::valueChanged, this, &KeysSequenceWidget::timesToRunChanged);
}

void KeysSequenceWidget::keysSeqIdentityReceived(QString id)
{
    auto keysseqaction =  dynamic_cast<KeysSequenceAction*>(m_action.get());
    if(keysseqaction == nullptr)
    {
        m_mainButton->setToolTip("ERROR on access to action");
        m_mainButton->setText("Keys sequence >ERROR");
        return;
    }
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    auto keysSequenceFromSettings = settings.value(G_Files::KeysSequencesDataCategory+id).toMap();
    PressedReleaseDelaysKeysMap keysMap = ActionsTools::fromStandardQMapToKeysSeqMap(keysSequenceFromSettings);
    keysseqaction->m_keysSeqMap = keysMap;
    keysseqaction->m_sequenceId = id;
    keysseqaction->generateTimeline();
    m_mainButton->setText(tr("Keys sequence >")+id);
    m_mainButton->setToolTip(ActionsTools::fromKeysSeqMapToPrintedString(keysMap));
    m_mainButton->setProperty("keysSeqId", id);

    emit anyParamChanged();
}

void KeysSequenceWidget::timesToRunChanged(int times)
{
    auto keysseqaction =  dynamic_cast<KeysSequenceAction*>(m_action.get());
    if(keysseqaction == nullptr)
        return;
    keysseqaction->m_timesToRun = times;
    emit anyParamChanged();
}

void KeysSequenceWidget::changedRunningState()
{
    refreshLoopsRemainingText(QDateTime::currentDateTime());
}

void KeysSequenceWidget::refreshLoopsRemainingText(const QDateTime &departureDate)
{
    m_infoLabel->setText("");
    if(m_runningState == RunningState::NotExecuted || m_runningState == RunningState::Done)
        return;

    auto keysseqaction =  dynamic_cast<KeysSequenceAction*>(m_action.get());
    if(keysseqaction == nullptr)
        return;

    int oneExecutionDuration = keysseqaction->computeOneExecutionDuration();
    if(oneExecutionDuration == 0)
        return;

    qint64 timelaps = departureDate.msecsTo(QDateTime::currentDateTime());
    int timesExecuted = timelaps / oneExecutionDuration;
    auto remainningTimes = keysseqaction->m_timesToRun - timesExecuted;
    if(remainningTimes < 1)
        return;

    m_infoLabel->setText(tr("Remaining ")+QString::number(remainningTimes)+tr(" executions"));

    QTimer::singleShot(200, this, [=]() {refreshLoopsRemainingText(departureDate);});
}
