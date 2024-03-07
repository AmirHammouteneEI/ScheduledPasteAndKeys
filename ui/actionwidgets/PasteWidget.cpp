#include "PasteWidget.h"
#include "actions/PasteAction.h"
#include "globals.h"

#include <QGridLayout>
#include <QPushButton>
#include <QSettings>

PasteWidget::PasteWidget(QWidget *parent)
    : AbstractActionWidget{parent}
{

}

void PasteWidget::buildWidget()
{
    if(m_centralWidget == nullptr)
        return;

    auto pasteaction =  dynamic_cast<PasteAction*>(m_action);

    QString content = tr("ERROR on accessing action");
    QString id = tr("ERROR");
    if(pasteaction != nullptr)
    {
        content = pasteaction->m_content;
        id = pasteaction->m_contentId;
    }

    auto gridLayout = new QGridLayout(m_centralWidget);
    m_mainButton = new QPushButton("Paste #"+id,m_centralWidget); //TODO title is the #id of the text + Icon
    m_mainButton->setToolTip(content);

    gridLayout->addItem(new QSpacerItem(5,5,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding),0,0);
    gridLayout->addWidget(m_mainButton,1,1,Qt::AlignCenter | Qt::AlignHCenter);
    gridLayout->addItem(new QSpacerItem(5,5,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding),2,2);

    gridLayout->setContentsMargins(1,1,1,1);
    gridLayout->setSpacing(2);

    m_selectSentenceDialog = new SelectSentenceDialog(m_centralWidget);

    connect(m_mainButton, &QPushButton::released, m_selectSentenceDialog, &SelectSentenceDialog::showDialog);
    connect(m_selectSentenceDialog, &SelectSentenceDialog::sendSentenceIdentity, this, &PasteWidget::sentenceIdentityReceived);
}

void PasteWidget::sentenceIdentityReceived(QString id)
{
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    QString content = settings.value(G_Files::SentencesDataCategory+id).toString();
    m_mainButton->setText("Paste #"+id);

    auto pasteaction =  dynamic_cast<PasteAction*>(m_action);
    if(pasteaction == nullptr)
    {
        m_mainButton->setToolTip("ERROR on access action");
        return;
    }
    pasteaction->m_content = content;
    m_mainButton->setToolTip(content);
}
