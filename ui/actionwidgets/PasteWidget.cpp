#include "PasteWidget.h"
#include "actions/PasteAction.h"
#include "globals.h"

#include <QGridLayout>
#include <QPushButton>
#include <QSettings>
#include <QTimer>
#include <QApplication>

PasteWidget::PasteWidget(QWidget *parent)
    : AbstractActionWidget{parent}
{

}

void PasteWidget::buildWidget()
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
    m_loopSpin->setToolTip(tr("Set how many times this paste text should be executed"));

    auto loopLayout = new QHBoxLayout(loopFrame);
    loopLayout->addWidget(loopLabel,0,Qt::AlignVCenter | Qt::AlignRight);
    loopLayout->addWidget(m_loopSpin,0,Qt::AlignCenter);
    loopLayout->addWidget(looptimesLabel,0,Qt::AlignVCenter | Qt::AlignLeft);
    loopLayout->setContentsMargins(1,1,1,1);
    loopLayout->setSpacing(2);

    centralGridLayout->removeWidget(m_infoLabel);
    centralGridLayout->addWidget(loopFrame,1,0,Qt::AlignCenter);
    centralGridLayout->addWidget(m_infoLabel,2,0,Qt::AlignCenter);

    auto pasteaction =  dynamic_cast<PasteAction*>(m_action.get());

    QString content = tr("ERROR on access to action");
    QString id = tr("ERROR");
    if(pasteaction != nullptr)
    {
        content = pasteaction->m_content;
        id = pasteaction->m_contentId;
        m_loopSpin->setValue(pasteaction->m_timesToRun);
    }

    m_mainButton->setObjectName("pastetextActionButton");
    m_mainButton->setText(tr("Paste text #")+id);
    m_mainButton->setToolTip(content);
    m_mainButton->setProperty("contentId", id);

    m_createPasteActionDialog = new CreatePasteActionDialog(m_centralWidget);

    connect(m_mainButton, &QPushButton::released, m_createPasteActionDialog, &CreatePasteActionDialog::showDialog);
    connect(m_createPasteActionDialog, &CreatePasteActionDialog::sendSentence, this, &PasteWidget::sentenceIdentityReceived);
    connect(m_loopSpin, &NoWheelFocusSpinBox::valueChanged, this, &PasteWidget::timesToRunChanged);
}

void PasteWidget::sentenceIdentityReceived(QString id)
{
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    QString content = settings.value(G_Files::SentencesDataCategory+id).toString();

    auto pasteaction =  dynamic_cast<PasteAction*>(m_action.get());
    if(pasteaction == nullptr)
    {
        m_mainButton->setToolTip("ERROR on access to action");
        m_mainButton->setText("Paste text #ERROR");
        return;
    }
    pasteaction->m_content = content;
    pasteaction->m_contentId = id;
    m_mainButton->setText(tr("Paste text #")+id);
    m_mainButton->setToolTip(content);
    m_mainButton->setProperty("contentId", id);

    emit anyParamChanged();
}

void PasteWidget::timesToRunChanged(int times)
{
    auto pasteaction =  dynamic_cast<PasteAction*>(m_action.get());
    if(pasteaction == nullptr)
        return;
    pasteaction->m_timesToRun = times;
    emit anyParamChanged();
}

void PasteWidget::changedRunningState()
{
    refreshLoopsRemainingText(QDateTime::currentDateTime());
}

void PasteWidget::refreshLoopsRemainingText(const QDateTime &departureDate)
{
    m_infoLabel->setText("");

    if(m_runningState == RunningState::NotExecuted || m_runningState == RunningState::Done)
        return;

    auto pasteaction =  dynamic_cast<PasteAction*>(m_action.get());
    if(pasteaction == nullptr)
        return;

    int oneExecutionDuration = pasteaction->computeOneExecutionDuration();
    if(oneExecutionDuration == 0)
        return;

    qint64 timelaps = departureDate.msecsTo(QDateTime::currentDateTime());
    int timesExecuted = timelaps / oneExecutionDuration;
    auto remainningTimes = pasteaction->m_timesToRun - timesExecuted;
    if(remainningTimes < 1)
        return;

    m_infoLabel->setText(tr("Remaining ")+QString::number(remainningTimes)+tr(" executions"));

    QTimer::singleShot(200, this, [=]() {refreshLoopsRemainingText(departureDate);});

}

