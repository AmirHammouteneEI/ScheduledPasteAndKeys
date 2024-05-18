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

void KeysSequencesTableWidget::refresh()
{

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

void KeysSequencesTableWidget::createKeysSequenceReceived()
{
    m_keysSequenceEditDialog->setEditable(true);
    m_keysSequenceEditDialog->setIdentity("");
    m_keysSequenceEditDialog->setTableKeysSequence(QMap<double, ReleaseDelayKeysPair>());
    m_keysSequenceEditDialog->exec();
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

void KeysSequencesTableWidget::editFromDialogReceived()
{
    //TODO next step
}

void KeysSequencesTableWidget::editKeysSequenceSelected(int row, int)
{
    //TODO next step
}
