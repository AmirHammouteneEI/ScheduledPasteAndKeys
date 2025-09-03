#include "StartupTasksTableWidget.h"
#include "globals.h"

#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>
#include <QDir>
#include <QApplication>

StartupTasksTableWidget::StartupTasksTableWidget(QWidget *parent)
    : NoFocusCellTableWidget{parent}
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
    QStringList paramsInTable;

    if(!startupTasksParams.isEmpty())
    {
        auto tasksParamsList = startupTasksParams.split("?");
        for(auto& taskParam : tasksParamsList)
        {
            auto paramL = taskParam.split("/");
            if(paramL.size() != 3 || paramsInTable.contains(taskParam))
                continue;
            insertRow(rowCount());
            setItem(rowCount()-1,0,new QTableWidgetItem(paramL.at(0)));
            setItem(rowCount()-1,1,new QTableWidgetItem(paramL.at(1)));
            QString loopTimesStr = "";
            loopTimesStr = paramL.at(2);
            if(loopTimesStr == "-1")
                setItem(rowCount()-1,2,new QTableWidgetItem(tr("Infinite")));
            else
                setItem(rowCount()-1,2,new QTableWidgetItem(loopTimesStr));

            auto checkB = new QCheckBox(this);
            checkB->setProperty("params", taskParam);
            checkB->setChecked(doesEntryExistsInRegistry(taskParam));
            connect(checkB, &QCheckBox::clicked, this, &StartupTasksTableWidget::checkBoxToggled);
            setCellWidget(rowCount()-1,3,checkB);

            paramsInTable.append(taskParam);

            model()->setData(model()->index(rowCount()-1,1),Qt::AlignCenter,Qt::TextAlignmentRole);
            model()->setData(model()->index(rowCount()-1,2),Qt::AlignCenter,Qt::TextAlignmentRole);
            model()->setData(model()->index(rowCount()-1,3),Qt::AlignCenter,Qt::DecorationRole);
        }
    }

    bool haveToSaveNewRegistryEntries = false;

    QSettings settingsReg(G_Files::SystemStartupRegistry_Path, QSettings::NativeFormat);
    for(auto &entry : settingsReg.childKeys())
    {
        bool entryMatches = false;
        if(entry.contains(G_Files::ProgramNameInRegistry_KeyWord))
        {
            QString taskName, delay, loopTimes;
            QRegularExpression regexp(G_Files::ProgramNameInRegistry_KeyWord+" (.*) (\\d+) (-?\\d+)");
            QRegularExpressionMatch match = regexp.match(entry);
            if (match.hasMatch()) {
                taskName = match.captured(1);
                delay = match.captured(2);
                loopTimes = match.captured(3);

                entryMatches = true;
            }

            QString taskParam = taskName+"/"+delay+"/"+loopTimes;
            if(!entryMatches || paramsInTable.contains(taskParam))
                continue;

            insertRow(rowCount());
            setItem(rowCount()-1,0,new QTableWidgetItem(taskName));
            setItem(rowCount()-1,1,new QTableWidgetItem(delay));
            if(loopTimes == "-1")
                setItem(rowCount()-1,2,new QTableWidgetItem(tr("Infinite")));
            else
                setItem(rowCount()-1,2,new QTableWidgetItem(loopTimes));

            auto checkB = new QCheckBox(this);
            checkB->setProperty("params", taskParam);
            checkB->setChecked(true);
            connect(checkB, &QCheckBox::clicked, this, &StartupTasksTableWidget::checkBoxToggled);
            setCellWidget(rowCount()-1,3,checkB);

            model()->setData(model()->index(rowCount()-1,1),Qt::AlignCenter,Qt::TextAlignmentRole);
            model()->setData(model()->index(rowCount()-1,2),Qt::AlignCenter,Qt::TextAlignmentRole);
            model()->setData(model()->index(rowCount()-1,3),Qt::AlignCenter,Qt::DecorationRole);

            haveToSaveNewRegistryEntries = true;
        }
    }

    if(haveToSaveNewRegistryEntries)
        saveInSettings();
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
        QString loopTimesStr = item(row,2)->text();
        if(loopTimesStr == tr("Infinite"))
            loopTimesStr = "-1";
        strToWrite += item(row,0)->text()+"/"+item(row,1)->text()+"/"+loopTimesStr+"?";
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
    QString loopTimesStr = item(row,2)->text();
    if(loopTimesStr == tr("Infinite"))
        loopTimesStr = "-1";
    deleteEntryFromRegistry(item(row,0)->text()+"/"+item(row,1)->text()+"/"+loopTimesStr);

    removeRow(row);

    saveInSettings();
    refresh();
}

void StartupTasksTableWidget::editFromDialogReceived()
{
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    auto startupTasksParams = settings.value(G_Files::StartupTasksParams_KeyWord,"").toString();
    auto entryParams = m_startupTaskEditDialog->m_filename +"/" + QString::number(m_startupTaskEditDialog->m_delay)+"/"+QString::number(m_startupTaskEditDialog->m_loopTimes);
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
    if(paramL.size() == 3)
    {
        if(settings.childKeys().contains(G_Files::ProgramNameInRegistry_KeyWord+" "+paramL.at(0)+" "+paramL.at(1)+" "+paramL.at(2)))
            return true;
    }
    return false;
}

void StartupTasksTableWidget::addEntryToRegistry(const QString &taskParam)
{
    auto paramL = taskParam.split("/");
    if(paramL.size() != 3)
        return;

    QSettings settings(G_Files::SystemStartupRegistry_Path, QSettings::NativeFormat);
    settings.setValue(G_Files::ProgramNameInRegistry_KeyWord+" "+paramL.at(0)+" "+paramL.at(1)+" "+paramL.at(2),
                      QDir::toNativeSeparators(QApplication::applicationFilePath())+" \""+paramL.at(0)+"\" "+paramL.at(1)+" "+paramL.at(2));
    settings.sync();
}

void StartupTasksTableWidget::deleteEntryFromRegistry(const QString &taskParam)
{
    if(taskParam.isEmpty()) // have to check otherwise it removes all startup keys
        return;
    auto paramL = taskParam.split("/");
    if(paramL.size() != 3)
        return;

    QSettings settings(G_Files::SystemStartupRegistry_Path, QSettings::NativeFormat);
    settings.remove(G_Files::ProgramNameInRegistry_KeyWord+" "+paramL.at(0)+" "+paramL.at(1)+" "+paramL.at(2));
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
