#include "StartupTasksTableWidget.h"
#include "globals.h"

#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>

StartupTasksTableWidget::StartupTasksTableWidget(QWidget *parent)
    : QTableWidget{parent}
{
    m_startupTaskEditDialog = new StartupTaskEditDialog(this);
    connect(m_startupTaskEditDialog, &QDialog::accepted, this, &StartupTasksTableWidget::editFromDialogReceived);
}

void StartupTasksTableWidget::refresh()
{
    clearContents();
    setRowCount(0);
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    auto startupTasksParams = settings.value(G_Files::StartupTasksParams_KeyWord,"").toString();

    if(startupTasksParams.isEmpty())
        return;
    auto tasksParamsList = startupTasksParams.split("?");

    for(auto& taskParam : tasksParamsList)
    {
        auto paramL = taskParam.split("/");
        if(paramL.size() != 2)
            continue;
        insertRow(rowCount());
        setItem(rowCount()-1,0,new QTableWidgetItem(paramL.at(0)));
        setItem(rowCount()-1,1,new QTableWidgetItem(paramL.at(1)));
        setCellWidget(rowCount()-1,2,new QCheckBox(this));
        model()->setData(model()->index(rowCount()-1,1),Qt::AlignCenter,Qt::TextAlignmentRole);
        model()->setData(model()->index(rowCount()-1,2),Qt::AlignCenter,Qt::DecorationRole);
    }
}

void StartupTasksTableWidget::saveInSettings()
{
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);

    if(rowCount() == 0)
    {
        settings.setValue(G_Files::StartupTasksParams_KeyWord,QString());
        return;
    }
    QString strToWrite;
    for(int row = 0; row < rowCount() ; ++row)
    {
        strToWrite += item(row,0)->text()+"/"+item(row,1)->text()+"?";
    }
    strToWrite.chop(1);

    settings.setValue(G_Files::StartupTasksParams_KeyWord,strToWrite);
}

void StartupTasksTableWidget::addStartupTaskReceived()
{
    m_startupTaskEditDialog->showDialog();
}

void StartupTasksTableWidget::removeStartupTaskReceived()
{
    QItemSelectionModel *selection = selectionModel();
    if(!selection->hasSelection())
    {
        QMessageBox::warning(this, tr("No startup task selected"),G_Sentences::NoStartupTaskSelected());
        return;
    }
    removeRow(selection->selectedRows().at(0).row());
    saveInSettings();
    refresh();
}

void StartupTasksTableWidget::editFromDialogReceived()
{
    QSettings settings(G_Files::DataFilePath, QSettings::IniFormat);
    auto startupTasksParams = settings.value(G_Files::StartupTasksParams_KeyWord,"").toString();
    startupTasksParams += (startupTasksParams.isEmpty() ? "":"?")+m_startupTaskEditDialog->m_filename +"/" + QString::number(m_startupTaskEditDialog->m_delay);
    settings.setValue(G_Files::StartupTasksParams_KeyWord,startupTasksParams);
    refresh();
}
