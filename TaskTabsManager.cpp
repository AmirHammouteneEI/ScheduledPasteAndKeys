#include "TaskTabsManager.h"
#include "globals.h"
#include "mainwindow.h"
#include "actions/PasteAction.h"
#include "actions/WaitAction.h"
#include "actions/KeysSequenceAction.h"
#include "actions/SystemCommandsAction.h"
#include "actions/CursorMovementsAction.h"
#include "actions/RunningOtherTaskAction.h"
#include <qt_windows.h>

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTabBar>
#include <QFileInfo>

TaskTabsManager::TaskTabsManager()
    : QObject{MainWindow::getInstance().get()}
{
}

TaskTabsManager::~TaskTabsManager()
{
    m_taskTabsMap.clear();
    m_taskFilePathsMap.clear();
}

int TaskTabsManager::onOpenNewTabRequest(const QString &path)
{
    QFile fileToOpen(path);
    if(!fileToOpen.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(MainWindow::getInstance().get(), tr("Cannot open file"), tr("Trying to open file located in :\n")+
          path+"\n"+G_Sentences::OperationInterference());
        return -1;
    }

    QString fileContent = fileToOpen.readAll();
    fileToOpen.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent.toUtf8());
    if(jsonDoc.isNull())
    {
        QMessageBox::warning(MainWindow::getInstance().get(), tr("File format error"),G_Sentences::FileParsingError());
        return -1;
    }

    QJsonObject jsonContent = jsonDoc.object();

    if(jsonContent.value(G_Files::DocumentIdentification_KeyWord).toString()
        != G_Files::DocumentIdentification_Value)
    {
        QMessageBox::warning(MainWindow::getInstance().get(), tr("File format error"), G_Sentences::FileParsingError());
        return -1;
    }

    QFileInfo finfo(fileToOpen.fileName());
    QString taskName = finfo.fileName().chopped(5);

    int tabIfAlreadyOpen = getTabIndexfomId(getIdfromTaskName(taskName));
    if(tabIfAlreadyOpen != -1)
    {
        MainWindow::getInstance()->getTabWidget()->setCurrentIndex(tabIfAlreadyOpen);
        return -1;
    }

    int id = appendTaskInMap(createEmptyTaskAndOpenTab(taskName));

    m_taskFilePathsMap.insert(id, path);

    createAndLoadTaskObject(id);

    return id;
}

TaskTab* TaskTabsManager::createEmptyTaskAndOpenTab(const QString &name)
{
    TaskTab* newEmptyTask = new TaskTab(MainWindow::getInstance().get(), name);
    MainWindow::getInstance()->getTabWidget()->insertTab(0,newEmptyTask,name);
    MainWindow::getInstance()->getTabWidget()->tabBar()->setTabToolTip(0,name);
    MainWindow::getInstance()->getTabWidget()->setCurrentIndex(0);

    connect(newEmptyTask, &TaskTab::saveTaskRequest, this, &TaskTabsManager::saveTaskReceived);

    return newEmptyTask;
}

int TaskTabsManager::appendTaskInMap(TaskTab *task)
{
    if(task == nullptr)
        return -1;

    m_taskTabsMap.insert(m_idCounter,task);
    task->m_ID = m_idCounter;

    return m_idCounter++;
}

int TaskTabsManager::getIdfromTaskName(const QString &name) const
{
    for(auto it = m_taskTabsMap.keyValueBegin(); it != m_taskTabsMap.keyValueEnd(); ++it)
    {
        if(it->second->m_name == name)
            return it->first;
    }
    return -1;
}

int TaskTabsManager::getTabIndexfomId(int id) const
{
    if(id < 0 || !m_taskTabsMap.contains(id))
        return -1;

    return MainWindow::getInstance()->getTabWidget()->indexOf(m_taskTabsMap.value(id, nullptr));
}

void TaskTabsManager::onTabCloseRequest(int index)
{
    auto task = qobject_cast<TaskTab*>(MainWindow::getInstance()->getTabWidget()->widget(index));

    if(task != nullptr)
    {
        if(task->m_scheduleState != ScheduleState::NotScheduled)
        {
            if(QMessageBox::question(MainWindow::getInstance().get(),tr("Task is scheduled or is running"),
              tr("This task is scheduled for execution or is currently running, by pressing \"Yes\" you are going to stop it.\n"\
                "\nIf you want to close it, you will have to request again."),
              QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
                return;
            task->forcedStop();
            return;
        }

        if(task->m_taskModifiedFromLastSave)
        {
            QMessageBox::StandardButton response = QMessageBox::question(MainWindow::getInstance().get(),tr("Saving changes"),
              tr("This Task has been modified since the last save, would you like to save changes ?"),
              QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Yes));

            if(response == QMessageBox::Cancel)
                return;
            if(response == QMessageBox::Yes)
                saveTaskReceived(task->m_ID, true);
        }

        m_taskTabsMap.remove(task->m_ID);
        task->deleteLater();
    }

    MainWindow::getInstance()->getTabWidget()->removeTab(index);
    MainWindow::getInstance()->getTabWidget()->setCurrentIndex(0);
}

void TaskTabsManager::forceCloseTask(int id)
{
    if(!m_taskTabsMap.contains(id))
        return;

    MainWindow::getInstance()->getTabWidget()->removeTab(getTabIndexfomId(id));
    MainWindow::getInstance()->getTabWidget()->setCurrentIndex(0);

    auto task = m_taskTabsMap.take(id);
    task->deleteLater();

}

void TaskTabsManager::scheduleTaskFromId(int id, qint64 delay, int loopTimes)
{
    if(!m_taskTabsMap.contains(id))
        return;

    auto taskTab = m_taskTabsMap.value(id, nullptr);

    if(taskTab == nullptr)
        return;

    taskTab->setTimesToRunValue(loopTimes);
    taskTab->scheduleTaskAfterDelay(delay);
}

void TaskTabsManager::onRefreshTabsRequest()
{
    for(auto it = m_taskTabsMap.keyValueBegin(); it != m_taskTabsMap.keyValueEnd();)
    {
        if(it->second == nullptr)
        {
            forceCloseTask((it++)->first);
            continue;
        }

        QFile fileToCheck(m_taskFilePathsMap.value(it->first));

        if(!fileToCheck.exists())
        {
            it->second->forcedStop();
            forceCloseTask((it++)->first);
            continue;
        }

        QFileInfo finfo(fileToCheck.fileName());
        QString taskName = finfo.fileName().chopped(5);

        it->second->setName(taskName);

        MainWindow::getInstance()->getTabWidget()->setTabText(getTabIndexfomId(it->first), taskName);

        if(!it->second->m_taskModifiedFromLastSave && it->second->m_scheduleState == ScheduleState::NotScheduled)
            createAndLoadTaskObject(it->first);

        ++it;
    }
}

void TaskTabsManager::onTaskfilePathChanged(QString oldpath, QString newpath)
{
    for(auto it = m_taskFilePathsMap.keyValueBegin(); it != m_taskFilePathsMap.keyValueEnd(); ++it)
    {
        if(it->second == oldpath)
            m_taskFilePathsMap.insert(it->first, newpath);
    }
}

void TaskTabsManager::forceStopAllRunningTasksReceived()
{
    for(auto it = m_taskTabsMap.keyValueBegin(); it != m_taskTabsMap.keyValueEnd(); ++it)
    {
        if(it->second == nullptr)
            continue;

        if(it->second->m_scheduleState == ScheduleState::NotScheduled)
            continue;

        it->second->forcedStop();
    }
}

void TaskTabsManager::createAndLoadTaskObject(int id)
{
    QFile fileToOpen(m_taskFilePathsMap.value(id));
    if(!fileToOpen.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString fileContent = fileToOpen.readAll();
    fileToOpen.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent.toUtf8());
    if(jsonDoc.isNull())
        return;

    std::shared_ptr<Task> task = std::make_shared<Task>();

    QJsonObject jsonContent = jsonDoc.object();

    if(jsonContent.value(G_Files::DocumentIdentification_KeyWord).toString()
        != G_Files::DocumentIdentification_Value)
    {
        QMessageBox::warning(MainWindow::getInstance().get(), tr("File format error"),
                             tr("The file is not in the good format, no action can't be loaded."));
    }
    else
    {
        QJsonArray actionsArray = jsonContent.value(G_Files::DocumentActionsArray_KeyWord).toArray();
        for(auto jvalue : actionsArray)
        {
            QJsonObject jobj = jvalue.toObject();
            auto actionToAdd = jsonToAction(jobj);
            if(actionToAdd != nullptr)
                task->appendAction(actionToAdd);
        }
    }

    m_taskTabsMap.value(id)->setDescription(jsonContent.value(G_Files::DocumentTaskDescription_KeyWord).toString());

    m_taskTabsMap.value(id)->setTask(task);
}

void TaskTabsManager::saveTaskReceived(int taskTabId, bool verbose)
{
    QFile fileToModify(m_taskFilePathsMap.value(taskTabId));
    if(!fileToModify.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        if(verbose)
            QMessageBox::warning(MainWindow::getInstance().get(),tr("Cannot access file"), G_Sentences::OperationInterference());
        return;
    }

    QString fileContent = fileToModify.readAll();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent.toUtf8());
    if(jsonDoc.isNull())
    {
        if(verbose)
            QMessageBox::warning(MainWindow::getInstance().get(),tr("File isn't in the good format anymore"), G_Sentences::FileParsingError());
        return;
    }

    QJsonObject jsonContent = jsonDoc.object();

    if(jsonContent.value(G_Files::DocumentIdentification_KeyWord).toString()
        != G_Files::DocumentIdentification_Value)
    {
        if(verbose)
            QMessageBox::warning(MainWindow::getInstance().get(), tr("File isn't in the good format anymore"), G_Sentences::FileParsingError());
        return;
    }
    fileToModify.resize(0);

    QJsonArray emptyArray;
    TaskTab *task = m_taskTabsMap.value(taskTabId);
    if(task == nullptr || task->m_task == nullptr)
    {
        if(verbose)
            QMessageBox::warning(MainWindow::getInstance().get(), tr("Internal Error"),
             tr("An internal error occured : err05\nTaskTab or Task associated is null in TaskTabsManager !"));
        return;
    }

    for(auto it = task->m_task->m_actionsOrderedList.begin(); it != task->m_task->m_actionsOrderedList.end(); ++it)
    {
        emptyArray.append(actionToJson(*it));
    }

    jsonContent.insert(G_Files::DocumentActionsArray_KeyWord, emptyArray);
    jsonContent.insert(G_Files::DocumentTaskDescription_KeyWord, task->m_description);

    QJsonDocument newJsondoc(jsonContent);
    fileToModify.write(newJsondoc.toJson());
    fileToModify.close();

    m_taskTabsMap.value(taskTabId)->setTaskModified(false);
}

QJsonObject TaskTabsManager::actionToJson(const std::shared_ptr<AbstractAction> &act)
{
    QJsonObject jsonToReturn;

    switch(act->e_type)
    {
        case ActionType::Paste:
        {
            jsonToReturn.insert(G_Files::ActionType_KeyWord, QJsonValue::fromVariant(G_Files::ActionPasteType_Value));
            auto pasteaction =  dynamic_cast<PasteAction*>(act.get());
            if(pasteaction != nullptr)
            {
                auto params = pasteaction->generateParameters();
                jsonToReturn.insert(G_Files::ActionContent_KeyWord, QJsonValue::fromVariant(params.m_pasteContent));
                jsonToReturn.insert(G_Files::ActionContentId_KeyWord, QJsonValue::fromVariant(params.m_dataId));
                jsonToReturn.insert(G_Files::ActionPasteTextLoop_KeyWord, QJsonValue::fromVariant(params.m_timesToRun));
            }
        }
        break;
        case ActionType::Wait:
        {
            jsonToReturn.insert(G_Files::ActionType_KeyWord, QJsonValue::fromVariant(G_Files::ActionWaitType_Value));
            auto waitaction =  dynamic_cast<WaitAction*>(act.get());
            if(waitaction != nullptr)
            {
              auto params = waitaction->generateParameters();
              jsonToReturn.insert(G_Files::ActionDuration_KeyWord, QJsonValue::fromVariant((double)params.m_waitDuration));
            }
        }
        break;
        case ActionType::KeysSequence:
        {
            jsonToReturn.insert(G_Files::ActionType_KeyWord, QJsonValue::fromVariant(G_Files::ActionKeysSequenceType_Value));
            auto keySeqaction =  dynamic_cast<KeysSequenceAction*>(act.get());
            if(keySeqaction != nullptr)
            {
                auto params = keySeqaction->generateParameters();
                QJsonObject writtenMapJObj;
                for(auto [key,val] : params.m_keysSeqMap.asKeyValueRange())
                {
                    QJsonArray jarr;
                    jarr.append(val.first);
                    jarr.append(QJsonValue::fromVariant(val.second));
                    writtenMapJObj.insert(QString::number(key),jarr);
                }
                jsonToReturn.insert(G_Files::ActionKeysSeqMap_KeyWord, writtenMapJObj);
                jsonToReturn.insert(G_Files::ActionKeysSeqId_KeyWord, QJsonValue::fromVariant(params.m_dataId));
                jsonToReturn.insert(G_Files::ActionKeysSeqLoop_KeyWord, QJsonValue::fromVariant(params.m_timesToRun));
            }
        }
        break;
        case ActionType::SystemCommand:
        {
            jsonToReturn.insert(G_Files::ActionType_KeyWord, QJsonValue::fromVariant(G_Files::ActionSystemCommandeType_Value));
            auto sysCmdaction =  dynamic_cast<SystemCommandAction*>(act.get());
            if(sysCmdaction != nullptr)
            {
                auto params = sysCmdaction->generateParameters();
                jsonToReturn.insert(G_Files::ActionSysCommandType_KeyWord, QJsonValue::fromVariant(params.m_sysCmdTypeStr));
                jsonToReturn.insert(G_Files::ActionSysCommandParam1_KeyWord, QJsonValue::fromVariant(params.m_sysCmdParam1));
                jsonToReturn.insert(G_Files::ActionSysCommandParam2_KeyWord, QJsonValue::fromVariant(params.m_sysCmdParam2));
            }
        }
        break;
        case ActionType::CursorMovements:
        {
            jsonToReturn.insert(G_Files::ActionType_KeyWord, QJsonValue::fromVariant(G_Files::ActionCursorMovementsType_Value));
            auto cursorMovsaction =  dynamic_cast<CursorMovementsAction*>(act.get());
            if(cursorMovsaction != nullptr)
            {
                auto params = cursorMovsaction->generateParameters();
                QJsonArray writtenListJArr;
                for(auto mov : params.m_cursorMovementsList)
                {
                    if(mov.size() < 4)
                        continue;
                    QJsonArray jarr;
                    jarr.append(mov[0]);
                    jarr.append(mov[1]);
                    jarr.append(mov[2]);
                    jarr.append(mov[3]);
                    writtenListJArr.append(jarr);
                }
                jsonToReturn.insert(G_Files::ActionCursorMovsMap_KeyWord, writtenListJArr);
                jsonToReturn.insert(G_Files::ActionCursorMovsId_KeyWord, QJsonValue::fromVariant(params.m_dataId));
                jsonToReturn.insert(G_Files::ActionCursorMovsLoop_KeyWord, QJsonValue::fromVariant(params.m_timesToRun));
                jsonToReturn.insert(G_Files::ActionCursorMovsOptKeysStroke_KeyWord, QJsonValue::fromVariant(params.m_cursorMovementsOptionalKeysStroke));
            }
        }
        break;
        case ActionType::RunningOtherTask:
        {
            jsonToReturn.insert(G_Files::ActionType_KeyWord, QJsonValue::fromVariant(G_Files::ActionRunningOtherTaskType_Value));
            auto runOtherTaskaction =  dynamic_cast<RunningOtherTaskAction*>(act.get());
            if(runOtherTaskaction != nullptr)
            {
                auto params = runOtherTaskaction->generateParameters();
                jsonToReturn.insert(G_Files::RunningOtherTaskFilename_KeyWord, QJsonValue::fromVariant(params.m_taskName));
                jsonToReturn.insert(G_Files::RunningOtherTaskDelay_KeyWord, QJsonValue::fromVariant(params.m_delay));
                jsonToReturn.insert(G_Files::RunningOtherTaskLoops_KeyWord, QJsonValue::fromVariant(params.m_timesToRun));
            }
        }
        break;
        default:
          break;
    }

    return jsonToReturn;
}

std::shared_ptr<AbstractAction> TaskTabsManager::jsonToAction(const QJsonObject &jobj)
{
    std::shared_ptr<AbstractAction> actionToReturn = nullptr;
    ActionParameters params;

    QString type = jobj.value(G_Files::ActionType_KeyWord).toString();
    if(type == G_Files::ActionPasteType_Value)
    {
        actionToReturn = std::make_shared<PasteAction>();
        params.m_pasteContent = jobj.value(G_Files::ActionContent_KeyWord).toString();
        params.m_dataId = jobj.value(G_Files::ActionContentId_KeyWord).toString();
        params.m_timesToRun = jobj.value(G_Files::ActionPasteTextLoop_KeyWord).toInt(1);
    }
    else if(type == G_Files::ActionWaitType_Value)
    {
        actionToReturn = std::make_shared<WaitAction>();
        params.m_waitDuration = jobj.value(G_Files::ActionDuration_KeyWord).toDouble();
    }
    else if(type == G_Files::ActionKeysSequenceType_Value)
    {
        actionToReturn = std::make_shared<KeysSequenceAction>();
        auto readMap = jobj.value(G_Files::ActionKeysSeqMap_KeyWord).toVariant().toMap();
        PressedReleaseDelaysKeysMap actMap;
        for(auto [key,val] : readMap.asKeyValueRange())
        {
            auto jarr = val.toJsonArray();
            if(jarr.size()<2)
                continue;
            ReleaseDelayKeysPair pair;
            pair.first = jarr[0].toInt();
            pair.second = jarr[1].toVariant().toStringList();
            actMap.insert(key.toInt(),pair);
        }
        params.m_keysSeqMap = actMap;
        params.m_dataId = jobj.value(G_Files::ActionKeysSeqId_KeyWord).toString();
        params.m_timesToRun = jobj.value(G_Files::ActionKeysSeqLoop_KeyWord).toInt();
    }
    else if(type == G_Files::ActionSystemCommandeType_Value)
    {
        actionToReturn = std::make_shared<SystemCommandAction>();
        params.m_sysCmdTypeStr = jobj.value(G_Files::ActionSysCommandType_KeyWord).toString();
        params.m_sysCmdParam1 = jobj.value(G_Files::ActionSysCommandParam1_KeyWord).toString();
        params.m_sysCmdParam2 = jobj.value(G_Files::ActionSysCommandParam2_KeyWord).toString();
    }
    else if(type == G_Files::ActionCursorMovementsType_Value)
    {
        actionToReturn = std::make_shared<CursorMovementsAction>();
        auto readList = jobj.value(G_Files::ActionCursorMovsMap_KeyWord).toVariant().toList();
        CursorMovementsList actList;
        for(auto &el : readList)
        {
            auto jarr = el.toJsonArray();
            if(jarr.size()<4)
                continue;
            MovementList movList;
            movList<< jarr[0].toInt() << jarr[1].toInt() << jarr[2].toInt() << jarr[3].toInt();
            actList.append(movList);
        }
        params.m_cursorMovementsList = actList;
        params.m_dataId = jobj.value(G_Files::ActionCursorMovsId_KeyWord).toString();
        params.m_timesToRun = jobj.value(G_Files::ActionCursorMovsLoop_KeyWord).toInt();
        params.m_cursorMovementsOptionalKeysStroke = jobj.value(G_Files::ActionCursorMovsOptKeysStroke_KeyWord).toVariant().toStringList();
    }
    else if(type == G_Files::ActionRunningOtherTaskType_Value)
    {
        actionToReturn = std::make_shared<RunningOtherTaskAction>();
        params.m_taskName = jobj.value(G_Files::RunningOtherTaskFilename_KeyWord).toString();
        params.m_delay = jobj.value(G_Files::RunningOtherTaskDelay_KeyWord).toInt();
        params.m_timesToRun = jobj.value(G_Files::RunningOtherTaskLoops_KeyWord).toInt();
    }
    else
        return nullptr;

    actionToReturn->setParameters(params);
    actionToReturn->optionalProcesses();

    return actionToReturn;
}

void TaskTabsManager::saveAllTasks()
{
    for(auto it = m_taskTabsMap.keyValueBegin(); it != m_taskTabsMap.keyValueEnd(); ++it)
    {
        if(it->second->m_taskModifiedFromLastSave)
            saveTaskReceived(it->first);
    }
}

bool TaskTabsManager::isAnyTaskModified()
{
    for(auto it = m_taskTabsMap.keyValueBegin(); it != m_taskTabsMap.keyValueEnd(); ++it)
    {
        if(it->second->m_taskModifiedFromLastSave)
            return true;
    }
    return false;
}
