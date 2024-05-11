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

    QString content = tr("ERROR on access to action");
    QString id = tr("ERROR");
    if(pasteaction != nullptr)
    {
        content = pasteaction->m_content;
        id = pasteaction->m_contentId;
    }

    m_mainButton->setIcon(QIcon(":/img/text.png"));
    m_mainButton->setText(tr("Paste text #")+id);
    m_mainButton->setToolTip(content);
    m_mainButton->setProperty("contentId", id);

    changeContentInfo(content);

    m_createPasteActionDialog = new CreatePasteActionDialog(m_centralWidget);

    connect(m_mainButton, &QPushButton::released, m_createPasteActionDialog, &CreatePasteActionDialog::showDialog);
    connect(m_createPasteActionDialog, &CreatePasteActionDialog::sendSentence, this, &PasteWidget::sentenceIdentityReceived);
}

void PasteWidget::sentenceIdentityReceived(QString id)
{
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    QString content = settings.value(G_Files::SentencesDataCategory+id).toString();

    auto pasteaction =  dynamic_cast<PasteAction*>(m_action);
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

    changeContentInfo(content);

    emit anyParamChanged();
}

void PasteWidget::changeContentInfo(const QString &content)
{
    m_infoLabel->setToolTip(content);
    QString simpliTruncContent = content.simplified();
    if(simpliTruncContent.size() > 60)
    {
        simpliTruncContent.truncate(55);
        simpliTruncContent+="[...]";
    }

    m_infoLabel->setText(simpliTruncContent);
    m_infoLabel->setFixedSize(m_infoLabel->sizeHint());
}
