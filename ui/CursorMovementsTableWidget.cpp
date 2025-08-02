#include "CursorMovementsTableWidget.h"
#include "globals.h"
#include "actions/ActionsTools.h"

#include <QApplication>
#include <QSettings>
#include <QMessageBox>

CursorMovementsTableWidget::CursorMovementsTableWidget(QWidget *parent):NoFocusCellTableWidget(parent)
{
    m_cursorMovementsEditDialog = new CursorMovementsSelectedEditDialog(this);
    connect(m_cursorMovementsEditDialog, &QDialog::accepted, this, &CursorMovementsTableWidget::editFromDialogReceived);
    connect(this, &QTableWidget::cellDoubleClicked, this, &CursorMovementsTableWidget::editCursorMovementsSelected);
}

void CursorMovementsTableWidget::createCursorMovementsReceived()
{
    m_cursorMovementsEditDialog->setEditable(true);
    m_cursorMovementsEditDialog->setIdentity("");
    m_cursorMovementsEditDialog->setTableCursorMovements(CursorMovementsList());
    m_cursorMovementsEditDialog->setOptionalKeysStroke(QStringList());
    m_cursorMovementsEditDialog->exec();
}

void CursorMovementsTableWidget::editFromDialogReceived()
{
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    auto receivedList = m_cursorMovementsEditDialog->tableCursorMovements();
    QList<QVariant> writtenList;
    for(auto &el : receivedList)
    {
        QVariant variant = QVariant::fromValue(el);
        writtenList.append(variant);
    }
    writtenList.append(m_cursorMovementsEditDialog->optionalKeysStroke());
    settings.setValue(G_Files::CursorMovementsDataCategory+m_cursorMovementsEditDialog->identity(),writtenList);
    refresh();

    selectCursorMovementsFromIdentity(m_cursorMovementsEditDialog->identity());
}

void CursorMovementsTableWidget::editCursorMovementsSelected(int row, int)
{
    QTableWidgetItem *idItem = item(row,0);
    if(idItem == nullptr)
        return;

    QString trueId = idItem->text().remove(0,1);
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);

    m_cursorMovementsEditDialog->setEditable(m_belongsToDataEditDialog);
    m_cursorMovementsEditDialog->setIdentity(trueId);
    auto readList = settings.value(G_Files::CursorMovementsDataCategory + trueId).toList();
    m_cursorMovementsEditDialog->setTableCursorMovements(ActionsTools::fromStandardQMapToCursorMovsMap(readList));
    m_cursorMovementsEditDialog->setOptionalKeysStroke(readList.last().toStringList());
    m_cursorMovementsEditDialog->show();
}

void CursorMovementsTableWidget::removeCursorMovementsReceived()
{
    QItemSelectionModel *selection = selectionModel();
    if(!selection->hasSelection())
    {
        QMessageBox::warning(this, tr("No cursor movements set selected"),G_Sentences::NoCursorMovementsSelected());
        return;
    }

    if(QMessageBox::question(this, tr("Confirm removing cursor movements set"), tr("You are about to remove this cursor movements set from data stored, are you sure ?"),
                              QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
        return;

    int row = selection->selectedRows().at(0).row();
    QTableWidgetItem *idItem = item(row,0);
    if(idItem == nullptr)
        return;
    QString trueId = idItem->text().remove(0,1);
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    settings.remove(G_Files::CursorMovementsDataCategory + trueId);

    refresh();
}

void CursorMovementsTableWidget::refresh()
{
    clearContents();
    setRowCount(0);
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    QStringList keys = settings.allKeys();
    for(const auto &key : keys)
    {
        if(key.startsWith(G_Files::CursorMovementsDataCategory))
        {
            insertRow(rowCount());

            QString id = key;
            id.remove(G_Files::CursorMovementsDataCategory);
            setItem(rowCount()-1,0,new QTableWidgetItem("~"+id));

            QTableWidgetItem *contentItem = new QTableWidgetItem();
            auto readList = settings.value(G_Files::CursorMovementsDataCategory + id).toList();
            QStringList contentList;
            for(auto &el : readList)
            {
                if(el==readList.last())
                    break;
                MovementList movelist = el.value<MovementList>();
                if(movelist.size() >= 4)
                    contentList.append("["+QString::number(movelist[2])+","+QString::number(movelist[3])+"]");
            }
            QString fullContent = contentList.join(" ; ") + " (Keys stroke : "+readList.last().toStringList().join("+")+")";
            contentItem->setText(fullContent);
            contentItem->setToolTip(ActionsTools::fromCursorMovsMapToPrintedString(ActionsTools::fromStandardQMapToCursorMovsMap(readList))+
                                    "\nKeys stroke : "+readList.last().toStringList().join("+"));
            setItem(rowCount()-1,1,contentItem);
        }
    }
}

void CursorMovementsTableWidget::selectCursorMovementsFromIdentity(const QString &id)
{
    QTableWidgetItem *idItem = nullptr;
    int rowFound = -1;
    for(int k=0; k< rowCount(); ++k)
    {
        idItem = item(k,0);
        if(idItem != nullptr && idItem->text() == "~"+id)
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
