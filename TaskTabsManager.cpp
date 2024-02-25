#include "TaskTabsManager.h"
#include "globals.h"
#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTabBar>

TaskTabsManager::TaskTabsManager(MainWindow *parent)
    : QObject{parent}, m_mainwindow(parent)
{
    if(m_mainwindow == nullptr)
    {
        QMessageBox::critical(nullptr, tr("Internal Error"),
            tr("An internal error occured : err01\nmainwindow is null when affecting to TaskTabsManager !"));
        qApp->quit();
        return;
    }
}

void TaskTabsManager::onOpenNewTabRequest(QString path)
{
    QFile fileToOpen(path);
    if(!fileToOpen.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(m_mainwindow, tr("Cannot open file"), tr("Trying to open file located in :\n")+
          path+"\n"+G_Sentences::OperationInterference);
        return;
    }

    QString fileContent = fileToOpen.readAll();
    fileToOpen.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent.toUtf8());
    if(jsonDoc.isNull())
    {
        QMessageBox::warning(m_mainwindow, tr("File format error"),G_Sentences::FileParsingError);
        return;
    }

    QJsonObject jsonContent = jsonDoc.object();

    if(jsonContent.value(G_Files::DocumentIdentification_KeyWord).toString()
        != G_Files::DocumentIdentification_Value)
    {
        QMessageBox::warning(m_mainwindow, tr("File format error"), G_Sentences::FileParsingError);
        return;
    }

    QString taskName = jsonContent.value(G_Files::DocumentTaskName_KeyWord).toString();

    int tabIfAlreadyOpen = getTabIndexforId(getIdforTaskName(taskName));
    if(tabIfAlreadyOpen != -1)
    {
        m_mainwindow->getTabWidget()->setCurrentIndex(tabIfAlreadyOpen);
        return;
    }

    int id = appendTaskInMap(createEmptyTaskAndOpenTab(taskName));

    m_taskFilePathsMap.insert(id, path);

    createAndLoadTaskObject(id);
}

TaskTab* TaskTabsManager::createEmptyTaskAndOpenTab(const QString &name)
{
    TaskTab* newEmptyTask = new TaskTab(m_mainwindow, name);
    m_mainwindow->getTabWidget()->insertTab(0,newEmptyTask,name);
    m_mainwindow->getTabWidget()->tabBar()->setTabToolTip(0,name);
    m_mainwindow->getTabWidget()->setCurrentIndex(0);

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

int TaskTabsManager::getIdforTaskName(const QString &name)
{
    for(auto it = m_taskTabsMap.keyValueBegin(); it != m_taskTabsMap.keyValueEnd(); ++it)
    {
        if(it->second->m_name == name)
            return it->first;
    }
    return -1;
}

int TaskTabsManager::getTabIndexforId(int id)
{
    if(id < 0 || !m_taskTabsMap.contains(id))
        return -1;

    return m_mainwindow->getTabWidget()->indexOf(m_taskTabsMap.value(id, nullptr));
}

void TaskTabsManager::onTabCloseRequest(int index)
{
    auto task = qobject_cast<TaskTab*>(m_mainwindow->getTabWidget()->widget(index));

    if(task != nullptr)
    {

        //TODO save before close request

        m_taskTabsMap.remove(task->m_ID);
        task->deleteLater();
    }

    m_mainwindow->getTabWidget()->removeTab(index);
    m_mainwindow->getTabWidget()->setCurrentIndex(0);
}

void TaskTabsManager::forceCloseTask(int id)
{
    if(!m_taskTabsMap.contains(id))
        return;

    m_mainwindow->getTabWidget()->removeTab(getTabIndexforId(id));
    m_mainwindow->getTabWidget()->setCurrentIndex(0);

    auto task = m_taskTabsMap.take(id);
    task->deleteLater();

}

void TaskTabsManager::onRefreshTabsRequest()
{
    for(auto it = m_taskTabsMap.keyValueBegin(); it != m_taskTabsMap.keyValueEnd();)
    {
        QFile fileToOpen(m_taskFilePathsMap.value(it->first));
        if(!fileToOpen.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            forceCloseTask((it++)->first);
            continue;
        }

        QString fileContent = fileToOpen.readAll();
        fileToOpen.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent.toUtf8());
        if(jsonDoc.isNull())
        {
            forceCloseTask((it++)->first);
            continue;
        }

        QJsonObject jsonContent = jsonDoc.object();

        if(jsonContent.value(G_Files::DocumentIdentification_KeyWord).toString()
            != G_Files::DocumentIdentification_Value)
        {
            forceCloseTask((it++)->first);
            continue;
        }

        QString taskName = jsonContent.value(G_Files::DocumentTaskName_KeyWord).toString();

        it->second->setName(taskName);

        m_mainwindow->getTabWidget()->setTabText(getTabIndexforId(it->first), taskName);

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

    Task *task = new Task();

    QJsonObject jsonContent = jsonDoc.object();

    //TODO go through actions to fill new Task

    TODELETE_fillTaskTest(task); //TODELETE testing prebuilt task

    m_taskTabsMap.value(id)->setTask(task);
    m_taskTabsMap.value(id)->refreshActionsList();
}

//TODELETE testing prebuilt task

#include "actions/PasteAction.h"
#include "actions/WaitAction.h"

void TaskTabsManager::TODELETE_fillTaskTest(Task *task)
{
    if(task == nullptr)
        return;

    ActionParameters paramPaste1;
    paramPaste1.m_pasteContent = "Maître Corbeau, sur un arbre perché,\n\n";
    ActionParameters paramWait;
    paramWait.m_waitDuration = 12;
    ActionParameters paramPaste2;
    paramPaste2.m_pasteContent = "Tenait en son bec un fromage.\n\n";

    PasteAction *paste1 = new PasteAction();
    paste1->setParameters(paramPaste1);
    WaitAction *wait = new WaitAction();
    wait->setParameters(paramWait);
    PasteAction *paste2 = new PasteAction();
    paste2->setParameters(paramPaste2);

    task->appendAction(paste1);
    task->appendAction(wait);
    task->appendAction(paste2);
}

//TODELETE end
