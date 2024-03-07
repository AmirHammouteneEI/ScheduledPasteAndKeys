#include "SentencesTableWidget.h"
#include "globals.h"

#include <QSettings>
#include <QMessageBox>


SentencesTableWidget::SentencesTableWidget(QWidget *parent)
    : QTableWidget{parent}
{
    m_sentenceEditDialog = new SentenceSelectedEditDialog(this);
    connect(m_sentenceEditDialog, &QDialog::accepted, this, &SentencesTableWidget::editFromDialogReceived);
    connect(this, &QTableWidget::cellDoubleClicked, this, &SentencesTableWidget::editSentenceSelected);
}

void SentencesTableWidget::createSentenceReceived()
{
    m_sentenceEditDialog->setEditable(true, true);
    m_sentenceEditDialog->setIdentity("");
    m_sentenceEditDialog->setContent("");
    m_sentenceEditDialog->show();
}

void SentencesTableWidget::editSentenceSelected(int row, int)
{
    QTableWidgetItem *idItem = item(row,0);
    if(idItem == nullptr)
        return;

    QString trueId = idItem->text().remove(0,1);
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    m_sentenceEditDialog->setEditable(false, true);
    m_sentenceEditDialog->setIdentity(trueId);
    m_sentenceEditDialog->setContent(settings.value(G_Files::SentencesDataCategory + trueId).toString());
    m_sentenceEditDialog->show();
}

void SentencesTableWidget::editFromDialogReceived()
{
    if(m_sentenceEditDialog->identity().isEmpty())
    {
        QMessageBox::warning(this, tr("Sentence has no identity"),
                             tr("The sentence you tried to add has no identity, cancelled operation."));
        return;
    }

    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    settings.setValue(G_Files::SentencesDataCategory+m_sentenceEditDialog->identity(),
                      m_sentenceEditDialog->content());
    refresh();
}

void SentencesTableWidget::removeSentenceReceived()
{
    QItemSelectionModel *selection = selectionModel();
    if(!selection->hasSelection())
    {
        QMessageBox::warning(this, tr("No sentence selected"),G_Sentences::NoSetenceSelected);
        return;
    }

    if(QMessageBox::question(this, tr("Confirm removing sentence"), tr("You are about to remove this sentence from data stored, are you sure ?"),
         QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
        return;

    int row = selection->selectedRows().at(0).row();
    QTableWidgetItem *idItem = item(row,0);
    if(idItem == nullptr)
        return;
    QString trueId = idItem->text().remove(0,1);
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    settings.remove(G_Files::SentencesDataCategory + trueId);

    refresh();
}

void SentencesTableWidget::refresh()
{
    clear();
    setRowCount(0);
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    QStringList keys = settings.allKeys();
    foreach(const QString &key, keys)
    {
        if(key.startsWith(G_Files::SentencesDataCategory))
        {
            insertRow(rowCount());

            QString id = key;
            id.remove(G_Files::SentencesDataCategory);
            setItem(rowCount()-1,0,new QTableWidgetItem("#"+id));

            QTableWidgetItem *contentItem = new QTableWidgetItem();
            QString content = settings.value(key).toString();
            contentItem->setToolTip(content);
            contentItem->setText(content.simplified());
            setItem(rowCount()-1,1,contentItem);
        }
    }
}
