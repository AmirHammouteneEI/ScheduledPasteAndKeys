#include "CreateLoadTaskDialog.h"
#include "ui_CreateLoadTaskDialog.h"
#include <QPushButton>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

const QString CreateLoadTaskDialog::s_tasksFolder = "saved_tasks/";
const QString CreateLoadTaskDialog::s_tasksFileExtension = ".scht";

CreateLoadTaskDialog::CreateLoadTaskDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateLoadTaskDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Create"));
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
    QString filePathChosen = QDir::currentPath()+"/"+s_tasksFolder+ui->lineEdit->text()+s_tasksFileExtension;
    QFileInfo fileChosen(filePathChosen);

    if(fileChosen.exists() && fileChosen.isFile())
    {
        if(QMessageBox::question(this, tr("File with this name already exists"),
          tr("A task file with the chosen name already exists, do you confirm that you would like to overwrite this one ?"),
          QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
            return;
    }

    QFile fileToCreate(filePathChosen);
    if(!fileToCreate.open(QIODevice::WriteOnly | QIODevice::Text) || ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Cannot create file"),
                              tr("Some system or user action interferes with this file creation, sorry for the inconvenience."));
        return;
    }

    fileToCreate.close();

    QDialog::accept();
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
