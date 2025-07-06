#include "StartupTaskEditDialog.h"
#include "ui_StartupTaskEditDialog.h"
#include "globals.h"

#include <QDir>
#include <QMessageBox>

StartupTaskEditDialog::StartupTaskEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartupTaskEditDialog)
{
    ui->setupUi(this);
    connect(ui->checkBox,&QCheckBox::toggled, this, &StartupTaskEditDialog::loopToggled);
}

StartupTaskEditDialog::~StartupTaskEditDialog()
{
    delete ui;
}

void StartupTaskEditDialog::showDialog()
{
    fillExistingTasksTable();
    ui->timeEdit->setTime(QTime(0,1,0));
    ui->spinBox->setValue(0);
    ui->checkBox->setChecked(false);
    ui->spinBox_2->setValue(1);
    ui->spinBox_2->setEnabled(true);
    exec();
}

void StartupTaskEditDialog::accept()
{
    if(ui->tableWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, tr("No file selected"), G_Sentences::NoFileSelected());
        return;
    };
    if(ui->timeEdit->time() == QTime(0,0,0))
    {
        QMessageBox::warning(this, tr("Delay can't be set to 0"), tr("The delay for the task to run after system startup can't be set to zero."));
        return;
    }

    m_filename = ui->tableWidget->selectedItems().at(0)->text().chopped(5);
    m_delay = QTime(0,0,0).secsTo(ui->timeEdit->time());
    m_delay+= ui->spinBox->value() * 86400;

    if(ui->checkBox->isChecked())
        m_loopTimes = -1;
    else
        m_loopTimes = ui->spinBox_2->value();

    QDialog::accept();
}

void StartupTaskEditDialog::fillExistingTasksTable()
{
    QDir tasksFolder = QApplication::applicationDirPath()+"/"+G_Files::TasksFolder;
    if(!tasksFolder.exists())
        return;

    QStringList tasksNamesList = tasksFolder.entryList(QStringList() << "*"+G_Files::TasksFileExtension, QDir::Files);

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

void StartupTaskEditDialog::loopToggled(bool state)
{
    if(state)
        ui->spinBox_2->setEnabled(false);
    else
        ui->spinBox_2->setEnabled(true);
}
