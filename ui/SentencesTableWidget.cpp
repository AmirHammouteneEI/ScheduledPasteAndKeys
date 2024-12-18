#include "SentencesTableWidget.h"
#include "globals.h"

#include <QSettings>
#include <QMessageBox>
#include <QApplication>

SentencesTableWidget::SentencesTableWidget(QWidget *parent)
    : QTableWidget{parent}
{
    m_sentenceEditDialog = new SentenceSelectedEditDialog(this);
    connect(m_sentenceEditDialog, &QDialog::accepted, this, &SentencesTableWidget::editFromDialogReceived);
    connect(this, &QTableWidget::cellDoubleClicked, this, &SentencesTableWidget::editSentenceSelected);
}

void SentencesTableWidget::createSentenceReceived()
{
    m_sentenceEditDialog->setEditable(true);
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
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    m_sentenceEditDialog->setEditable(m_belongsToDataEditDialog);
    m_sentenceEditDialog->setIdentity(trueId);
    m_sentenceEditDialog->setContent(settings.value(G_Files::SentencesDataCategory + trueId).toString());
    m_sentenceEditDialog->show();
}

void SentencesTableWidget::editFromDialogReceived()
{
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    settings.setValue(G_Files::SentencesDataCategory+m_sentenceEditDialog->identity(),
                      m_sentenceEditDialog->content());
    refresh();

    selectSentenceFromIdentity(m_sentenceEditDialog->identity());
}

void SentencesTableWidget::removeSentenceReceived()
{
    QItemSelectionModel *selection = selectionModel();
    if(!selection->hasSelection())
    {
        QMessageBox::warning(this, tr("No sentence selected"),G_Sentences::NoSetenceSelected());
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
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    settings.remove(G_Files::SentencesDataCategory + trueId);

    refresh();
}

void SentencesTableWidget::refresh()
{
    clearContents();
    setRowCount(0);
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    QStringList keys = settings.allKeys();
    for(const auto &key : keys)
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

void SentencesTableWidget::selectSentenceFromIdentity(const QString &id)
{
    QTableWidgetItem *idItem = nullptr;
    int rowFound = -1;
    for(int k=0; k< rowCount(); ++k)
    {
        idItem = item(k,0);
        if(idItem != nullptr && idItem->text() == "#"+id)
        {
            rowFound = k;
            break;
        }
    }

    if(rowFound >= 0)
    {
        selectRow(rowFound);
        scrollToItem(idItem,QAbstractItemView::PositionAtCenter);
    }
}
