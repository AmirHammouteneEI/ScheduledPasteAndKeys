#include "GlobalHotKeysTableWidget.h"
#include "globals.h"
#include "GlobalHotkeyRegisterManager.h"
#include "ui/getdialogs/getQKeySequenceDialog.h"

#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>
#include <QPushButton>
#include <QDir>
#include <QApplication>

GlobalHotKeysTableWidget::GlobalHotKeysTableWidget(QWidget *parent)
    : NoFocusCellTableWidget{parent}
{
    m_hotkeyEditDialog = new GlobalHotKeyEditDialog(this);
    connect(m_hotkeyEditDialog, &QDialog::accepted, this, &GlobalHotKeysTableWidget::editFromDialogReceived);
}

void GlobalHotKeysTableWidget::refresh()
{
    clearContents();
    setRowCount(0);
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    auto hotkeysParams = settings.value(G_Files::HotkeysParams_KeyWord,"").toString();
    QStringList paramsInTable;

    bool haveToCreateStopAllTasksEntry = true;
    if(!hotkeysParams.isEmpty())
    {
        auto hotkeysParamsList = hotkeysParams.split("?");
        for(auto& hotkeyParam : hotkeysParamsList)
        {
            auto paramL = hotkeyParam.split("/");
            QString shortHotkeyParam = hotkeyParam.chopped(2);
            if(paramL.size() != 5 || paramsInTable.contains(shortHotkeyParam))
                continue;
            insertRow(rowCount());
            QString keySeqStr = paramL.at(0);
            QString taskName = paramL.at(1);
            QString delayStr = paramL.at(2);
            QString loopTimesStr = paramL.at(3);
            bool activateHotkey = paramL.at(4) == "1";

            bool isStopAllTasks = taskName == "Stop All Tasks";
            if(isStopAllTasks)
                haveToCreateStopAllTasksEntry = false;

            auto button = new QPushButton(this);
            button->setFlat(true);
            button->setObjectName("buttonInTable");
            button->setText(keySeqStr);
            connect(button, &QPushButton::released, this, &GlobalHotKeysTableWidget::onHotkeyButtonPushed);
            setCellWidget(rowCount()-1,0,button);
            setItem(rowCount()-1,1,new QTableWidgetItem(taskName));
            setItem(rowCount()-1,2,new QTableWidgetItem(delayStr));
            if(loopTimesStr == "-1")
                setItem(rowCount()-1,3,new QTableWidgetItem(tr("Infinite")));
            else
                setItem(rowCount()-1,3,new QTableWidgetItem(loopTimesStr));

            auto checkB = new QCheckBox(this);
            checkB->setObjectName("checkboxInTable");
            checkB->setProperty("params", shortHotkeyParam);
            checkB->setChecked(activateHotkey);
            connect(checkB, &QCheckBox::clicked, this, &GlobalHotKeysTableWidget::checkBoxToggled);
            setCellWidget(rowCount()-1,4,checkB);

            paramsInTable.append(shortHotkeyParam);

            model()->setData(model()->index(rowCount()-1,1),Qt::AlignCenter,Qt::TextAlignmentRole);
            model()->setData(model()->index(rowCount()-1,2),Qt::AlignCenter,Qt::TextAlignmentRole);
            model()->setData(model()->index(rowCount()-1,3),Qt::AlignCenter,Qt::TextAlignmentRole);
            model()->setData(model()->index(rowCount()-1,4),Qt::AlignCenter,Qt::DecorationRole);

            if(!m_hotkeyParamsToObjectIdMap.contains(shortHotkeyParam))
            {
                auto taskHotKeyObj = std::make_shared<TaskHotKey>();
                taskHotKeyObj->setAllAttributes(QKeySequence(keySeqStr), taskName, delayStr.toInt(), loopTimesStr.toInt());
                if(isStopAllTasks)
                    taskHotKeyObj->setAsStopAllTasksType();

                GlobalHotkeyRegisterManager::getInstance()->appendTaskHotKey(taskHotKeyObj);
                m_hotkeyParamsToObjectIdMap.insert(shortHotkeyParam, taskHotKeyObj->getID());

                if(activateHotkey)
                    registerHotKey(shortHotkeyParam);

            }
        }
    }

    if(haveToCreateStopAllTasksEntry)
    {
        insertRow(0);
        QString keySeqStr = "Ctrl+Alt+S";
        QString taskName = "Stop All Tasks";
        QString hotkeyParam = keySeqStr+"/"+taskName+"/0/1/1";
        QString shortHotkeyParam = hotkeyParam.chopped(2);

        auto button = new QPushButton(this);
        button->setFlat(true);
        button->setObjectName("buttonInTable");
        button->setText(keySeqStr);
        connect(button, &QPushButton::released, this, &GlobalHotKeysTableWidget::onHotkeyButtonPushed);
        setCellWidget(0,0,button);
        setItem(0,1,new QTableWidgetItem(taskName));
        setItem(0,2,new QTableWidgetItem("0"));
        setItem(0,3,new QTableWidgetItem("1"));
        auto checkB = new QCheckBox(this);
        checkB->setObjectName("checkboxInTable");
        checkB->setProperty("params", shortHotkeyParam);
        checkB->setChecked(true);
        connect(checkB, &QCheckBox::clicked, this, &GlobalHotKeysTableWidget::checkBoxToggled);
        setCellWidget(0,4,checkB);

        model()->setData(model()->index(0,0),Qt::AlignCenter,Qt::DecorationRole);
        model()->setData(model()->index(0,1),Qt::AlignCenter,Qt::TextAlignmentRole);
        model()->setData(model()->index(0,2),Qt::AlignCenter,Qt::TextAlignmentRole);
        model()->setData(model()->index(0,3),Qt::AlignCenter,Qt::TextAlignmentRole);
        model()->setData(model()->index(0,4),Qt::AlignCenter,Qt::DecorationRole);

        auto stopAllTasksHotKeyObj = std::make_shared<TaskHotKey>();
        stopAllTasksHotKeyObj->setAllAttributes(QKeySequence(keySeqStr), taskName, 0, 1);
        stopAllTasksHotKeyObj->setAsStopAllTasksType();
        GlobalHotkeyRegisterManager::getInstance()->appendTaskHotKey(stopAllTasksHotKeyObj);
        m_hotkeyParamsToObjectIdMap.insert(shortHotkeyParam, stopAllTasksHotKeyObj->getID());
        registerHotKey(shortHotkeyParam);

        saveInSettings();
    }
}

void GlobalHotKeysTableWidget::saveInSettings()
{
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);

    if(rowCount() == 0)
    {
        settings.setValue(G_Files::HotkeysParams_KeyWord,QString());
        return;
    }
    QString strToWrite;
    for(int row = 0; row < rowCount() ; ++row)
    {
        QString keySeqStr;
        auto *butWidget = cellWidget(row, 0);
        if(butWidget)
        {
            QPushButton *button = qobject_cast<QPushButton*>(butWidget);
            if(button)
                keySeqStr = button->text();
        }

        QString loopTimesStr = item(row,3)->text();
        if(loopTimesStr == tr("Infinite"))
            loopTimesStr = "-1";

        bool checked = true;
        auto *chkWidget = cellWidget(row, 4);
        if(chkWidget)
        {
            QCheckBox *checkBox = qobject_cast<QCheckBox*>(chkWidget);
            if(checkBox)
                checked = checkBox->isChecked();
        }
        QString activatedStr = checked ? "1" : "0";

        strToWrite += keySeqStr+"/"+item(row,1)->text()+"/"+item(row,2)->text()+"/"+loopTimesStr+"/"+activatedStr+"?";
    }
    strToWrite.chop(1);

    settings.setValue(G_Files::HotkeysParams_KeyWord,strToWrite);
}

void GlobalHotKeysTableWidget::addHotkeyReceived()
{
    m_hotkeyEditDialog->showDialog();
}

void GlobalHotKeysTableWidget::removeHotkeyReceived()
{
    QItemSelectionModel *selection = selectionModel();
    if(!selection->hasSelection())
    {
        QMessageBox::warning(this, tr("No global hotkey selected"),G_Sentences::NoHotKeySelected());
        return;
    }

    auto row = selection->selectedRows().at(0).row();

    QString keySeqStr;
    auto *butWidget = cellWidget(row, 0);
    if(butWidget)
    {
        QPushButton *button = qobject_cast<QPushButton*>(butWidget);
        if(button)
            keySeqStr = button->text();
    }

    QString loopTimesStr = item(row,3)->text();
    if(loopTimesStr == tr("Infinite"))
        loopTimesStr = "-1";

    bool checked = true;
    auto *widget = cellWidget(row, 4);
    if(widget)
    {
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(widget);
        if(checkBox)
            checked = checkBox->isChecked();
    }
    QString activatedStr = checked ? "1" : "0";

    QString shortHotkeyParam = keySeqStr+"/"+item(row,1)->text()+"/"+item(row,2)->text()+"/"+loopTimesStr;
    unregisterHotKey(shortHotkeyParam);
    m_hotkeyParamsToObjectIdMap.remove(shortHotkeyParam);

    removeRow(row);

    saveInSettings();
    refresh();
}

void GlobalHotKeysTableWidget::editFromDialogReceived()
{
    QString newKeySeqStr = m_hotkeyEditDialog->m_keySeq.toString(QKeySequence::PortableText);

    for(int row = 0; row < rowCount(); ++row)
    {
        QString keySeqStr;
        auto *butWidget = cellWidget(row, 0);
        if(butWidget)
        {
            QPushButton *button = qobject_cast<QPushButton*>(butWidget);
            if(button)
                keySeqStr = button->text();
        }

        if(keySeqStr == newKeySeqStr)
        {
            QMessageBox::warning(this, tr("Global hotkey already in use"),G_Sentences::HotKeyAlreadyUsed());
            return;
        }
    }

    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
    auto hotkeysParams = settings.value(G_Files::HotkeysParams_KeyWord,"").toString();
    auto entryParams = newKeySeqStr+"/"+m_hotkeyEditDialog->m_filename+"/"+QString::number(m_hotkeyEditDialog->m_delay)+"/"+QString::number(m_hotkeyEditDialog->m_loopTimes)+"/1";

    hotkeysParams += (hotkeysParams.isEmpty() ? "":"?")+entryParams;
    settings.setValue(G_Files::HotkeysParams_KeyWord,hotkeysParams);

    refresh();
}

bool GlobalHotKeysTableWidget::doesHotKeyIsRegistered(const QString &hotkeyParam)
{
    if(m_hotkeyParamsToObjectIdMap.contains(hotkeyParam))
        return GlobalHotkeyRegisterManager::getInstance()->m_registeredHotkeysSet.contains(m_hotkeyParamsToObjectIdMap.value(hotkeyParam, 0));
    return false;
}

bool GlobalHotKeysTableWidget::registerHotKey(const QString &hotkeyParam)
{
    if(m_hotkeyParamsToObjectIdMap.contains(hotkeyParam))
        return GlobalHotkeyRegisterManager::getInstance()->registerHotkey(m_hotkeyParamsToObjectIdMap.value(hotkeyParam, 0));
    return false;
}

bool GlobalHotKeysTableWidget::unregisterHotKey(const QString &hotkeyParam)
{
    if(m_hotkeyParamsToObjectIdMap.contains(hotkeyParam))
        return GlobalHotkeyRegisterManager::getInstance()->unregisterHotkey(m_hotkeyParamsToObjectIdMap.value(hotkeyParam, 0));
    return false;
}

void GlobalHotKeysTableWidget::checkBoxToggled(bool val)
{
    auto checkBSender = qobject_cast<QCheckBox*>(sender());
    if(checkBSender == nullptr)
        return;

    auto param = checkBSender->property("params").toString();
    if(val)
    {
        if(!doesHotKeyIsRegistered(param))
        {
            if(!registerHotKey(param))
            {
                QMessageBox::warning(this, tr("Registration failed"), tr("Windows could not register this global hotkey. It may already be in use by another application."));
                checkBSender->setChecked(false);
            }
        }
    }
    else
    {
        if(!unregisterHotKey(param))
        {
            QMessageBox::warning(this, tr("Unregistration failed"), tr("Windows could not unregister this global hotkey."));
            checkBSender->setChecked(true);
        }
    }

    saveInSettings();
}

void GlobalHotKeysTableWidget::onHotkeyButtonPushed()
{
    QPushButton *buttonSender = qobject_cast<QPushButton*>(sender());
    if (buttonSender == nullptr)
        return;

    int currentRow = -1;
    for(int row = 0; row < rowCount(); ++row)
    {
        if(cellWidget(row,0) == buttonSender)
        {
            currentRow = row;
            break;
        }
    }
    if (currentRow == -1)
        return;

    QString oldKeyseqStr = buttonSender->text();

    auto *getKeyseqDialog = new getQKeySequenceDialog(QKeySequence(oldKeyseqStr, QKeySequence::PortableText), this);

    connect(getKeyseqDialog, &QDialog::accepted, this, [=]() {
        QString newKeyseqStr = getKeyseqDialog->keySequence().toString(QKeySequence::PortableText);

        if (newKeyseqStr.isEmpty() || newKeyseqStr == oldKeyseqStr)
            return;

        //If key already exists on another listed hotkey
        for(int row = 0; row < rowCount(); ++row)
        {
            if (row == currentRow)
                continue;
            auto *but = qobject_cast<QPushButton*>(cellWidget(row, 0));
            if (but && but->text() == newKeyseqStr)
            {
                QMessageBox::warning(this, tr("Global hotkey already in use"), G_Sentences::HotKeyAlreadyUsed());
                return;
            }
        }

        QString loopTimesStr = item(currentRow, 3)->text();
        if (loopTimesStr == tr("Infinite"))
            loopTimesStr = "-1";

        QString oldShortParam = oldKeyseqStr+"/"+item(currentRow,1)->text()+"/"+item(currentRow,2)->text()+"/"+loopTimesStr;
        unregisterHotKey(oldShortParam);

        buttonSender->setText(newKeyseqStr);

        saveInSettings();
        refresh();
    });

    // destroy dialog after close
    connect(getKeyseqDialog, &QDialog::finished, getKeyseqDialog, &QObject::deleteLater);
    getKeyseqDialog->show();
}