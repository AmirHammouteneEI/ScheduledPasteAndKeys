#include "StartupTasksTableWidget.h"
#include "globals.h"

#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>
#include <QDir>
#include <QApplication>

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
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    auto startupTasksParams = settings.value(G_Files::StartupTasksParams_KeyWord,"").toString();

    if(startupTasksParams.isEmpty())
        return;
    auto tasksParamsList = startupTasksParams.split("?");
    QStringList paramsInTable;
    for(auto& taskParam : tasksParamsList)
    {
        auto paramL = taskParam.split("/");
        if(paramL.size() != 2 || paramsInTable.contains(taskParam))
            continue;
        paramsInTable.append(taskParam);
        insertRow(rowCount());
        setItem(rowCount()-1,0,new QTableWidgetItem(paramL.at(0)));
        setItem(rowCount()-1,1,new QTableWidgetItem(paramL.at(1)));

        auto checkB = new QCheckBox(this);
        checkB->setProperty("params", taskParam);
        checkB->setChecked(doesEntryExistsInRegistry(taskParam));
        connect(checkB, &QCheckBox::clicked, this, &StartupTasksTableWidget::checkBoxToggled);
        setCellWidget(rowCount()-1,2,checkB);

        model()->setData(model()->index(rowCount()-1,1),Qt::AlignCenter,Qt::TextAlignmentRole);
        model()->setData(model()->index(rowCount()-1,2),Qt::AlignCenter,Qt::DecorationRole);
    }
}

void StartupTasksTableWidget::saveInSettings()
{
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);

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

    auto row = selection->selectedRows().at(0).row();
    deleteEntryFromRegistry(item(row,0)->text()+"/"+item(row,1)->text());

    removeRow(row);

    saveInSettings();
    refresh();
}

void StartupTasksTableWidget::editFromDialogReceived()
{
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    auto startupTasksParams = settings.value(G_Files::StartupTasksParams_KeyWord,"").toString();
    auto entryParams = m_startupTaskEditDialog->m_filename +"/" + QString::number(m_startupTaskEditDialog->m_delay);
    if(startupTasksParams.contains(entryParams))
    {
        QMessageBox::warning(this, tr("Entry already exists"),tr("This entry already exists in tasks to be executed at system startup."));
        return;
    }
    startupTasksParams += (startupTasksParams.isEmpty() ? "":"?")+entryParams;
    settings.setValue(G_Files::StartupTasksParams_KeyWord,startupTasksParams);

    addEntryToRegistry(entryParams);
    refresh();
}

bool StartupTasksTableWidget::doesEntryExistsInRegistry(const QString &taskParam)
{
    QSettings settings(G_Files::SystemStartupRegistry_Path, QSettings::NativeFormat);
    auto paramL = taskParam.split("/");
    if(paramL.size() == 2)
    {
        if(settings.childKeys().contains(G_Files::ProgramNameInRegistry_KeyWord+" "+paramL.at(0)+" "+paramL.at(1)))
            return true;
    }
    return false;
}

void StartupTasksTableWidget::addEntryToRegistry(const QString &taskParam)
{
    auto paramL = taskParam.split("/");
    if(paramL.size() != 2)
        return;

    QSettings settings(G_Files::SystemStartupRegistry_Path, QSettings::NativeFormat);
    settings.setValue(G_Files::ProgramNameInRegistry_KeyWord+" "+paramL.at(0)+" "+paramL.at(1),
                      QDir::toNativeSeparators(QApplication::applicationFilePath())+" \""+paramL.at(0)+"\" "+paramL.at(1));
    settings.sync();
}

void StartupTasksTableWidget::deleteEntryFromRegistry(const QString &taskParam)
{
    if(taskParam.isEmpty()) // have to check otherwise it removes all startup keys
        return;
    auto paramL = taskParam.split("/");
    if(paramL.size() != 2)
        return;

    QSettings settings(G_Files::SystemStartupRegistry_Path, QSettings::NativeFormat);
    settings.remove(G_Files::ProgramNameInRegistry_KeyWord+" "+paramL.at(0)+" "+paramL.at(1));
    settings.sync();
}


void StartupTasksTableWidget::checkBoxToggled(bool val)
{
    auto checkBSender = qobject_cast<QCheckBox*>(sender());
    if(checkBSender == nullptr)
        return;

    auto param = checkBSender->property("params").toString();
    if(val)
    {
        if(!doesEntryExistsInRegistry(param))
            addEntryToRegistry(param);
    }
    else
        deleteEntryFromRegistry(param);
}
