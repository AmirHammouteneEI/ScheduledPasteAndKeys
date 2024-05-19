#include "KeysSequencesTableWidget.h"
#include "globals.h"

#include <QSettings>
#include <QMessageBox>

KeysSequencesTableWidget::KeysSequencesTableWidget(QWidget *parent)
    : QTableWidget{parent}
{
    //TODO next step MODEL OF THE TABLE : 2x doulblespinbox + button Open dialog for choose keys (treewidget of keys label)
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
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    m_keysSequenceEditDialog->setEditable(false);
    m_keysSequenceEditDialog->setIdentity(trueId);
    auto readMap = settings.value(G_Files::KeysSequencesDataCategory + trueId).toMap();
    PressedReleaseDelaysKeysMap sentMap;
    for(auto [key,val] : readMap.asKeyValueRange())
    {
        ReleaseDelayKeysPair pair = val.value<ReleaseDelayKeysPair>();
        sentMap.insert(key.toInt(),pair);
    }
    m_keysSequenceEditDialog->setTableKeysSequence(sentMap);
    m_keysSequenceEditDialog->show();
}

void KeysSequencesTableWidget::editFromDialogReceived()
{
    if(m_keysSequenceEditDialog->identity().isEmpty())
    {
        QMessageBox::warning(this, tr("Keys sequence has no identity"),
                             tr("The keys sequence you tried to add has no identity, cancelled operation."));
        return;
    }

    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    auto receivedMap = m_keysSequenceEditDialog->tableKeysSequence();
    QMap<QString, QVariant> writtenMap;
    for(auto [key,val] : receivedMap.asKeyValueRange())
    {
        QVariant variant = QVariant::fromValue(val);
        writtenMap.insert(QString::number(key),variant);
    }
    settings.setValue(G_Files::KeysSequencesDataCategory+m_keysSequenceEditDialog->identity(),
                      writtenMap);
    refresh();

    selectKeysSequenceFromIdentity(m_keysSequenceEditDialog->identity());
}

void KeysSequencesTableWidget::removeKeysSequenceReceived()
{
    QItemSelectionModel *selection = selectionModel();
    if(!selection->hasSelection())
    {
        QMessageBox::warning(this, tr("No keys sequence selected"),G_Sentences::NoSetenceSelected());
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
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    settings.remove(G_Files::KeysSequencesDataCategory + trueId);

    refresh();
}

void KeysSequencesTableWidget::refresh()
{
    clearContents();
    setRowCount(0);
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
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
            contentItem->setToolTip(fullContent);
            contentItem->setText(fullContent);
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
