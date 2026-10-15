#include "CreateLoadTaskDialog.h"
#include "ui_CreateLoadTaskDialog.h"
#include "globals.h"

#include <QPushButton>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>

CreateLoadTaskDialog::CreateLoadTaskDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateLoadTaskDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Create"));

    connect(ui->openButton, &QPushButton::released, this, &CreateLoadTaskDialog::onOpenFile);
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &CreateLoadTaskDialog::onOpenFile);

    connect(ui->renameButton, &QPushButton::released, this, &CreateLoadTaskDialog::onRenameFilename);
    connect(ui->deleteButton, &QPushButton::released, this, &CreateLoadTaskDialog::onDeleteFile);
    connect(ui->duplicateButton, &QPushButton::released, this, &CreateLoadTaskDialog::onDuplicateFilename);
    connect(ui->exportButton, &QPushButton::released, this, &CreateLoadTaskDialog::onExportFile);
    connect(ui->importButton, &QPushButton::released, this, &CreateLoadTaskDialog::onImportFile);
}

CreateLoadTaskDialog::~CreateLoadTaskDialog()
{
    delete ui;
}

void CreateLoadTaskDialog::showDialog()
{
    createTasksFolderIfNotExist();
    fillExistingTasksTable();
    show();
}

void CreateLoadTaskDialog::accept()
{
    auto filenameText = ui->lineEdit->text();
    if(filenameText.isEmpty())
    {
        QMessageBox::warning(this, tr("Empty filename"), tr("Task file must have a name."));
        return;
    }

    if(doesFilenameContainForbiddenChar(filenameText))
    {
        QMessageBox::warning(this, tr("Forbidden character"), G_Sentences::ForbiddenCharacter());
        return;
    }

    QString filePathChosen = QApplication::applicationDirPath()+"/"+G_Files::TasksFolder+filenameText+G_Files::TasksFileExtension;
    QFileInfo fileChosen(filePathChosen);

    if(fileChosen.exists() && fileChosen.isFile())
    {
        if(QMessageBox::question(this, tr("File with this name already exists"), G_Sentences::AlreadyExists(),
          QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
            return;
    }

    QFile fileToCreate(filePathChosen);
    if(!fileToCreate.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Cannot create file"), G_Sentences::OperationInterference());
        return;
    }

    QJsonObject newContentJson;
    newContentJson.insert(G_Files::DocumentIdentification_KeyWord,QJsonValue::fromVariant(G_Files::DocumentIdentification_Value));
    QJsonArray emptyArray;
    newContentJson.insert(G_Files::DocumentActionsArray_KeyWord, emptyArray);

    QJsonDocument newJsondoc(newContentJson);
    fileToCreate.write(newJsondoc.toJson());

    fileToCreate.close();

    emit requestOpenNewTab(filePathChosen);
    QDialog::accept();
}

void CreateLoadTaskDialog::onOpenFile()
{
    if(ui->tableWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, tr("No file selected"), G_Sentences::NoFileSelected());
        return;
    }


    QString filePathSelected = QApplication::applicationDirPath()+"/"+G_Files::TasksFolder+ui->tableWidget->selectedItems().at(0)->text();

    emit requestOpenNewTab(filePathSelected);
    QDialog::accept();
}


void CreateLoadTaskDialog::onRenameFilename()
{
    if(ui->tableWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, tr("No file selected"), G_Sentences::NoFileSelected());
        return;
    }

    QString oldFileName = ui->tableWidget->selectedItems().at(0)->text();
    QString oldFilePath = QApplication::applicationDirPath()+"/"+G_Files::TasksFolder+oldFileName;

    renameFilename(oldFileName, oldFilePath);
}

void CreateLoadTaskDialog::onDuplicateFilename()
{
    if(ui->tableWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, tr("No file selected"), G_Sentences::NoFileSelected());
        return;
    }

    QString oldFileName = ui->tableWidget->selectedItems().at(0)->text();
    QString oldFilePath = QApplication::applicationDirPath()+"/"+G_Files::TasksFolder+oldFileName;

    renameFilename(oldFileName, oldFilePath, true);
}

void CreateLoadTaskDialog::onExportFile()
{
    if(ui->tableWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, tr("No file selected"), G_Sentences::NoFileSelected());
        return;
    }

    QString fileName = ui->tableWidget->selectedItems().at(0)->text();
    QString path = QFileDialog::getSaveFileName(this, tr("Define where to save the task file"),fileName,tr("ScheduledTask file (*.scht)"));

    if(!path.isEmpty())
    {
        QFile f(path);
        f.remove();
        if(!QFile::copy(QApplication::applicationDirPath()+"/"+G_Files::TasksFolder+fileName,path))
        {
            QMessageBox::warning(this, tr("Cannot save this file"), G_Sentences::OperationInterference());
            return;
        }
    }
}

void CreateLoadTaskDialog::onImportFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select a task file"),"",tr("ScheduledTask file (*.scht)"));

    if(path.isEmpty())
        return;

    QFileInfo fileChosen(path);
    QString taskName = fileChosen.fileName().chopped(5);

    if(doesFilenameContainForbiddenChar(taskName))
    {
        QMessageBox::warning(this, tr("Forbidden character"), G_Sentences::ForbiddenCharacter());
        return;
    }

    if(!fileChosen.exists() || !fileChosen.isFile())
    {
        QMessageBox::warning(this, tr("Cannot find the file"), G_Sentences::OperationInterference());
        return;
    }

    QString taskNameToSave = taskName;
    QString pathToSave = QApplication::applicationDirPath()+"/"+G_Files::TasksFolder+taskNameToSave+G_Files::TasksFileExtension;
    QFileInfo fileToSave(pathToSave);
    bool okButPushed = true;
    while(fileToSave.exists() && fileToSave.isFile() && okButPushed)
    {
        taskNameToSave = QInputDialog::getText(this, tr("New name for this file"),tr("A task with the same name already exists.\nPlease choose a new name for this file to import : \n"),
                                                    QLineEdit::Normal, taskNameToSave,&okButPushed);

        pathToSave = QApplication::applicationDirPath()+"/"+G_Files::TasksFolder+taskNameToSave+G_Files::TasksFileExtension;
        fileToSave.setFile(pathToSave);
    }

    if(!okButPushed)
        return;

    QFile f(pathToSave);
    f.remove();
    if(!QFile::copy(path, pathToSave))
    {
        QMessageBox::warning(this, tr("Cannot save this file"), G_Sentences::OperationInterference());
        return;
    }

    fillExistingTasksTable();
}

void CreateLoadTaskDialog::renameFilename(const QString &oldFileName, const QString &oldFilePath, bool duplicate)
{
    QString actionName = tr("rename");
    if(duplicate)
        actionName = tr("duplicate");

    bool okButPushed;
    QString newFileName = QInputDialog::getText(this, tr("New name for this file"),tr("Please choose a new name for this file : \n")+oldFileName.chopped(5),
       QLineEdit::Normal, oldFileName.chopped(5),&okButPushed);

    if(!okButPushed)
        return;

    if(newFileName.isEmpty())
    {
        QMessageBox::warning(this, tr("Empty filename"),
          tr("Task file must have a name."));
        return;
    }

    if(doesFilenameContainForbiddenChar(newFileName))
    {
        QMessageBox::warning(this, tr("Forbidden character"), G_Sentences::ForbiddenCharacter());
        return;
    }

    if(newFileName == oldFileName.chopped(5))
    {
        QMessageBox::warning(this, tr("Same filename"),
          tr("You are trying to ")+actionName+tr(" this file with the same old name. Canceled operation."));
        return;
    }

    QString filePathChosen = QApplication::applicationDirPath()+"/"+G_Files::TasksFolder+newFileName+G_Files::TasksFileExtension;
    QFileInfo fileChosen(filePathChosen);

    if(fileChosen.exists() && fileChosen.isFile())
    {
        if(QMessageBox::question(this, tr("File with this name already exists"), G_Sentences::AlreadyExists(),
             QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
            return;
        else
        {
            if(!QFile::remove(filePathChosen))
            {
                QMessageBox::warning(this, tr("Cannot delete file with same name"), G_Sentences::OperationInterference());
                return;
            }
        }
    }

    if(!duplicate)
    {
        QFile f(filePathChosen);
        f.remove();
        if(!QFile::rename(oldFilePath, filePathChosen))
        {
            QMessageBox::warning(this, tr("Cannot rename this file"), G_Sentences::OperationInterference());
            return;
        }
    }
    else
    {
        if(!QFile::copy(oldFilePath, filePathChosen))
        {
            QMessageBox::warning(this, tr("Cannot duplicate this file"), G_Sentences::OperationInterference());
            return;
        }
    }

    fillExistingTasksTable();
    if(!duplicate)
        emit taskfilePathChanged(oldFilePath,filePathChosen);

    emit requestRefreshTabs();
}

void CreateLoadTaskDialog::onDeleteFile()
{
    if(ui->tableWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, tr("No file selected"), G_Sentences::NoFileSelected());
        return;
    }

    QString fileName = ui->tableWidget->selectedItems().at(0)->text();
    QString filePath = QApplication::applicationDirPath()+"/"+G_Files::TasksFolder+fileName;

    QFileInfo fileChosen(filePath);

    if(fileChosen.exists() && fileChosen.isFile())
    {
        if(QMessageBox::question(this, tr("Confirm deleting file"),
            tr("Do you confirm that you would like to delete this file :\n")+fileName,
            QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
            return;
        else
        {
            if(!QFile::remove(filePath))
            {
                QMessageBox::warning(this, tr("Cannot delete file"), G_Sentences::OperationInterference());
                return;
            }
        }
    }

    fillExistingTasksTable();

    emit requestRefreshTabs();
}


void CreateLoadTaskDialog::fillExistingTasksTable()
{
    QDir tasksFolder = QApplication::applicationDirPath()+"/"+G_Files::TasksFolder;
    QStringList tasksNamesList = tasksFolder.entryList(QStringList() << "*"+G_Files::TasksFileExtension, QDir::Files);

    int nextLineNum, nextColumnNum;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    for(int k = 0; k < tasksNamesList.size(); k++)
    {
        nextLineNum = k/2;
        nextColumnNum = k % 2;

        if(nextColumnNum == 0)
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());

        auto * item = new QTableWidgetItem(tasksNamesList.at(k));
        item->setToolTip(tasksNamesList.at(k));

        ui->tableWidget->setItem(nextLineNum, nextColumnNum, item);
    }
}

void CreateLoadTaskDialog::createTasksFolderIfNotExist()
{
    QDir tasksFolder = QApplication::applicationDirPath()+"/"+G_Files::TasksFolder;
    if(!tasksFolder.exists())
        tasksFolder.mkdir(QApplication::applicationDirPath()+"/"+G_Files::TasksFolder);
}

bool CreateLoadTaskDialog::doesFilenameContainForbiddenChar(const QString &filename)
{
    if(filename.contains("<") || filename.contains(">") || filename.contains(":")
        || filename.contains("\\") || filename.contains("\"") || filename.contains("|")
        || filename.contains("?") || filename.contains("*") || filename.contains("/"))
    {
        return true;
    }
    return false;
}
