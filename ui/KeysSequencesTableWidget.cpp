#include "KeysSequencesTableWidget.h"
#include "globals.h"
#include "actions/ActionsTools.h"

#include <QSettings>
#include <QMessageBox>
#include <QApplication>

KeysSequencesTableWidget::KeysSequencesTableWidget(QWidget *parent)
    : QTableWidget{parent}
{
    m_keysSequenceEditDialog = new KeysSequenceSelectedEditDialog(this);
    connect(m_keysSequenceEditDialog, &QDialog::accepted, this, &KeysSequencesTableWidget::editFromDialogReceived);
    connect(this, &QTableWidget::cellDoubleClicked, this, &KeysSequencesTableWidget::editKeysSequenceSelected);
}

void KeysSequencesTableWidget::createKeysSequenceReceived()
{
    m_keysSequenceEditDialog->setEditable(true);
    m_keysSequenceEditDialog->setIdentity("");
    m_keysSequenceEditDialog->setTableKeysSequence(PressedReleaseDelaysKeysMap());
    m_keysSequenceEditDialog->exec();
}

void KeysSequencesTableWidget::editKeysSequenceSelected(int row, int)
{
    QTableWidgetItem *idItem = item(row,0);
    if(idItem == nullptr)
        return;

    QString trueId = idItem->text().remove(0,1);
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    m_keysSequenceEditDialog->setEditable(m_belongsToDataEditDialog);
    m_keysSequenceEditDialog->setIdentity(trueId);
    auto readMap = settings.value(G_Files::KeysSequencesDataCategory + trueId).toMap();
    m_keysSequenceEditDialog->setTableKeysSequence(ActionsTools::fromStandardQMapToKeysSeqMap(readMap));
    m_keysSequenceEditDialog->show();
}

void KeysSequencesTableWidget::editFromDialogReceived()
{
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    auto receivedMap = m_keysSequenceEditDialog->tableKeysSequence();
    QMap<QString, QVariant> writtenMap;
    for(auto [key,val] : receivedMap.asKeyValueRange())
    {
        QVariant variant = QVariant::fromValue(val);
        writtenMap.insert(QString::number(key),variant);
    }
    settings.setValue(G_Files::KeysSequencesDataCategory+m_keysSequenceEditDialog->identity(), writtenMap);
    refresh();

    selectKeysSequenceFromIdentity(m_keysSequenceEditDialog->identity());
}

void KeysSequencesTableWidget::removeKeysSequenceReceived()
{
    QItemSelectionModel *selection = selectionModel();
    if(!selection->hasSelection())
    {
        QMessageBox::warning(this, tr("No keys sequence selected"),G_Sentences::NoKeysSequenceSelected());
        return;
    }

    if(QMessageBox::question(this, tr("Confirm removing keys sequence"), tr("You are about to remove this keys sequence from data stored, are you sure ?"),
                              QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
        return;

    int row = selection->selectedRows().at(0).row();
    QTableWidgetItem *idItem = item(row,0);
    if(idItem == nullptr)
        return;
    QString trueId = idItem->text().remove(0,1);
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    settings.remove(G_Files::KeysSequencesDataCategory + trueId);

    refresh();
}

void KeysSequencesTableWidget::refresh()
{
    clearContents();
    setRowCount(0);
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    QStringList keys = settings.allKeys();
    for(const auto &key : keys)
    {
        if(key.startsWith(G_Files::KeysSequencesDataCategory))
        {
            insertRow(rowCount());

            QString id = key;
            id.remove(G_Files::KeysSequencesDataCategory);
            setItem(rowCount()-1,0,new QTableWidgetItem(">"+id));

            QTableWidgetItem *contentItem = new QTableWidgetItem();
            auto readMap = settings.value(G_Files::KeysSequencesDataCategory + id).toMap();
            QStringList contentList;
            for(auto [key,val] : readMap.asKeyValueRange())
            {
                ReleaseDelayKeysPair pair = val.value<ReleaseDelayKeysPair>();
                contentList.append(pair.second.join("+"));
            }
            QString fullContent = contentList.join(" ; ");
            contentItem->setText(fullContent);
            contentItem->setToolTip(ActionsTools::fromKeysSeqMapToPrintedString(ActionsTools::fromStandardQMapToKeysSeqMap(readMap)));
            setItem(rowCount()-1,1,contentItem);
        }
    }
}

void KeysSequencesTableWidget::selectKeysSequenceFromIdentity(const QString &id)
{
    QTableWidgetItem *idItem = nullptr;
    int rowFound = -1;
    for(int k=0; k< rowCount(); ++k)
    {
        idItem = item(k,0);
        if(idItem != nullptr && idItem->text() == ">"+id)
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
