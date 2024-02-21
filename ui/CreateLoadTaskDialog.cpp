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

const QString CreateLoadTaskDialog::s_tasksFolder = "saved_tasks/";
const QString CreateLoadTaskDialog::s_tasksFileExtension = ".scht";

CreateLoadTaskDialog::CreateLoadTaskDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateLoadTaskDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Create"));

    connect(ui->openButton, &QPushButton::released, this, &CreateLoadTaskDialog::onOpenFilename);
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &CreateLoadTaskDialog::onOpenFilename);

    connect(ui->renameButton, &QPushButton::released, this, &CreateLoadTaskDialog::onRenameFilename);
    connect(ui->deleteButton, &QPushButton::released, this, &CreateLoadTaskDialog::onDeleteFilename);
}

CreateLoadTaskDialog::~CreateLoadTaskDialog()
{
    delete ui;
}

void CreateLoadTaskDialog::showDialog()
{
    fillExistingTasksTable();
    show();
}

void CreateLoadTaskDialog::accept()
{
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Empty filename"), tr("Task file must have a name."));
        return;
    }

    QString filePathChosen = QDir::currentPath()+"/"+s_tasksFolder+ui->lineEdit->text()+s_tasksFileExtension;
    QFileInfo fileChosen(filePathChosen);

    if(fileChosen.exists() && fileChosen.isFile())
    {
        if(QMessageBox::question(this, tr("File with this name already exists"), G_Sentences::AlreadyExists,
          QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
            return;
    }

    QFile fileToCreate(filePathChosen);
    if(!fileToCreate.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Cannot create file"), G_Sentences::OperationInterference);
        return;
    }

    QJsonObject newContentJson;
    newContentJson.insert(G_Files::DocumentIdentification_KeyWord,QJsonValue::fromVariant(G_Files::DocumentIdentification_Value));
    newContentJson.insert(G_Files::DocumentTaskName_KeyWord,QJsonValue::fromVariant(ui->lineEdit->text()));
    QJsonArray emptyArray;
    newContentJson.insert(G_Files::DocumentActionsArray_KeyWord, emptyArray);

    QJsonDocument newJsondoc(newContentJson);
    fileToCreate.write(newJsondoc.toJson());

    fileToCreate.close();

    emit requestOpenNewTab(filePathChosen);
    QDialog::accept();
}

void CreateLoadTaskDialog::onOpenFilename()
{
    if(ui->tableWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, tr("No file selected"), G_Sentences::NoFileSelected);
        return;
    }


    QString filePathSelected = QDir::currentPath()+"/"+s_tasksFolder+ui->tableWidget->selectedItems().at(0)->text()+s_tasksFileExtension;

    emit requestOpenNewTab(filePathSelected);
    QDialog::accept();
}

void CreateLoadTaskDialog::onRenameFilename()
{
    if(ui->tableWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, tr("No file selected"), G_Sentences::NoFileSelected);
        return;
    }

    QString oldFileName = ui->tableWidget->selectedItems().at(0)->text();
    QString oldFilePath = QDir::currentPath()+"/"+s_tasksFolder+oldFileName;

    bool okButPushed;
    QString newFileName = QInputDialog::getText(this, tr("New name for this file"),tr("Please choose a new name for this file : \n")+oldFileName.chopped(5),
       QLineEdit::Normal, QString(),&okButPushed);

    if(!okButPushed)
        return;

    if(newFileName.isEmpty())
    {
        QMessageBox::warning(this, tr("Empty filename"),
          tr("Task file must have a name."));
        return;
    }

    if(newFileName == oldFileName.chopped(5))
    {
        QMessageBox::warning(this, tr("Same filename"),
          tr("You are trying to rename this file with the same old name. Canceled operation."));
        return;
    }

    QString filePathChosen = QDir::currentPath()+"/"+s_tasksFolder+newFileName+s_tasksFileExtension;
    QFileInfo fileChosen(filePathChosen);

    if(fileChosen.exists() && fileChosen.isFile())
    {
        if(QMessageBox::question(this, tr("File with this name already exists"), G_Sentences::AlreadyExists,
             QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
            return;
        else
        {
            if(!QFile::remove(filePathChosen))
            {
                QMessageBox::warning(this, tr("Cannot delete file with same name"), G_Sentences::OperationInterference);
                return;
            }
        }
    }

    if(!QFile::rename(oldFilePath, filePathChosen))
    {
        QMessageBox::warning(this, tr("Cannot rename this file"), G_Sentences::OperationInterference);
        return;
    }

    fillExistingTasksTable();
}

void CreateLoadTaskDialog::onDeleteFilename()
{
    if(ui->tableWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, tr("No file selected"), G_Sentences::NoFileSelected);
        return;
    }

    QString fileName = ui->tableWidget->selectedItems().at(0)->text();
    QString filePath = QDir::currentPath()+"/"+s_tasksFolder+fileName;

    QFileInfo fileChosen(filePath);

    if(fileChosen.exists() && fileChosen.isFile())
    {
        if(QMessageBox::question(this, tr("Confirm deleting file"),
            tr("Do you confirm that you would like to delete this file :\n")+fileName.chopped(5),
            QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
            return;
        else
        {
            if(!QFile::remove(filePath))
            {
                QMessageBox::warning(this, tr("Cannot delete file"), G_Sentences::OperationInterference);
                return;
            }
        }
    }

    fillExistingTasksTable();
}

void CreateLoadTaskDialog::fillExistingTasksTable()
{
    QDir tasksFolder = QDir::currentPath()+"/"+s_tasksFolder;
    QStringList tasksNamesList = tasksFolder.entryList(QStringList() << "*"+s_tasksFileExtension, QDir::Files);

    int nextLineNum, nextColumnNum;

    ui->tableWidget->setRowCount(0);

    for(int k = 0; k < tasksNamesList.size(); k++)
    {
        nextLineNum = k/2;
        nextColumnNum = k % 2;

        if(nextColumnNum == 0)
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());

        ui->tableWidget->setItem(nextLineNum, nextColumnNum, new QTableWidgetItem(tasksNamesList.at(k)));
    }
}
