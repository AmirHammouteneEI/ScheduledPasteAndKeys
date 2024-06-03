#include "KeysSequenceWidget.h"
#include "actions/KeysSequenceAction.h"
#include "globals.h"
#include "actions/ActionsTools.h"

#include <QGridLayout>
#include <QPushButton>
#include <QSettings>

KeysSequenceWidget::KeysSequenceWidget(QWidget *parent)
    : AbstractActionWidget{parent}
{

}

void KeysSequenceWidget::buildWidget()
{
    if(m_centralWidget == nullptr)
        return;

    auto keysseqaction =  dynamic_cast<KeysSequenceAction*>(m_action);

    QString seqStr = tr("ERROR on access to action");
    QString id = tr("ERROR");
    if(keysseqaction != nullptr)
    {
        id = keysseqaction->m_sequenceId;
        seqStr = ActionsTools::fromKeysSeqMapToPrintedString(keysseqaction->m_keysSeqMap);
    }

    //TODO find icon for keysSeq
    //m_mainButton->setIcon(QIcon(":/img/wait.png"));
    m_mainButton->setText(tr("Keys sequence >")+id);
    m_mainButton->setToolTip(seqStr);
    m_mainButton->setProperty("keysSeqId", id);

    refreshLoopsRemainingText();

    m_createKeysSeqActionDialog = new CreateKeysSequenceActionDialog(m_centralWidget);

    connect(m_mainButton, &QPushButton::released, m_createKeysSeqActionDialog, &CreateKeysSequenceActionDialog::showDialog);
    connect(m_createKeysSeqActionDialog, &CreateKeysSequenceActionDialog::sendKeysSequence, this, &KeysSequenceWidget::keysSeqIdentityReceived);
}

void KeysSequenceWidget::keysSeqIdentityReceived(QString id)
{
    auto keysseqaction =  dynamic_cast<KeysSequenceAction*>(m_action);
    if(keysseqaction == nullptr)
    {
        m_mainButton->setToolTip("ERROR on access to action");
        m_mainButton->setText("Keys sequence >ERROR");
        return;
    }
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    auto keysSequenceFromSettings = settings.value(G_Files::KeysSequencesDataCategory+id).toMap();
    PressedReleaseDelaysKeysMap keysMap = ActionsTools::fromStandardQMapToKeysSeqMap(keysSequenceFromSettings);
    keysseqaction->m_keysSeqMap = keysMap;
    keysseqaction->m_sequenceId = id;
    keysseqaction->generateTimeline();
    m_mainButton->setText(tr("Keys sequence >")+id);
    m_mainButton->setToolTip(ActionsTools::fromKeysSeqMapToPrintedString(keysMap));
    m_mainButton->setProperty("keysSeqId", id);

    refreshLoopsRemainingText();

    emit anyParamChanged();
}

void KeysSequenceWidget::refreshLoopsRemainingText()
{
    m_infoLabel->setText("");
    m_infoLabel->setFixedSize(m_infoLabel->sizeHint());
}
