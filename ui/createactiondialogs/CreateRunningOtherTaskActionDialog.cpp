#include "CreateRunningOtherTaskActionDialog.h"
#include "ui_CreateRunningOtherTaskActionDialog.h"
#include "globals.h"

#include <QDir>
#include <QMessageBox>
#include <QPushButton>

CreateRunningOtherTaskActionDialog::CreateRunningOtherTaskActionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateRunningOtherTaskActionDialog)
{
    ui->setupUi(this);
    connect(ui->checkBox_2,&QCheckBox::toggled, this, &CreateRunningOtherTaskActionDialog::loopToggled);
}

CreateRunningOtherTaskActionDialog::~CreateRunningOtherTaskActionDialog()
{
    delete ui;
}

void CreateRunningOtherTaskActionDialog::showDialog()
{
    fillExistingTasksTable();
    ui->timeEdit_2->setTime(QTime(0,1,0));
    ui->spinBox_4->setValue(0);
    ui->checkBox_2->setChecked(false);
    ui->spinBox_3->setValue(1);
    ui->spinBox_3->setEnabled(true);
    show();

    // if shows from an existing Running Other Task Widget
    auto mainButtonSender = qobject_cast<QPushButton*>(sender());
    if(mainButtonSender == nullptr)
        return;

    if(!mainButtonSender->property("filename").isValid() || !mainButtonSender->property("delay").isValid() ||
        !mainButtonSender->property("loops").isValid())
        return;

    m_filename = mainButtonSender->property("filename").toString();
    m_delay = mainButtonSender->property("delay").toInt();
    m_loopTimes = mainButtonSender->property("loops").toInt();

    qint64 secs = (int)m_delay % 60;
    qint64 minNum = (m_delay-secs)/60;
    qint64 mins = minNum % 60;
    qint64 hourNum = (minNum-mins)/ 60 ;
    qint64 hours = hourNum %24;
    qint64 days = (hourNum - hours)/ 24;

    ui->timeEdit_2->setTime(QTime(hours,mins,secs));
    ui->spinBox_4->setValue(days);

    if(m_loopTimes < 0)
    {
        ui->spinBox_3->setEnabled(false);
        ui->checkBox_2->setChecked(true);
    }
    else
    {
        ui->spinBox_3->setValue(m_loopTimes);
    }

    QTableWidgetItem *idItem = nullptr;
    int rowFound = -1, columnFound = -1;
    for(int k=0; k<ui->tableWidget->rowCount(); ++k)
    {
        for(int l=0; l<ui->tableWidget->columnCount(); ++l)
        {
            idItem = ui->tableWidget->item(k,l);
            if(idItem != nullptr && idItem->text() == m_filename+G_Files::TasksFileExtension)
            {
                rowFound = k;
                columnFound = l;
                break;
            }
        }
        if(rowFound > -1)
            break;
    }

    if(rowFound > -1)
    {
        auto index = ui->tableWidget->model()->index(rowFound, columnFound);
        ui->tableWidget->selectionModel()->select(index, QItemSelectionModel::SelectCurrent);
        ui->tableWidget->scrollToItem(idItem,QAbstractItemView::PositionAtCenter);
    }

}

void CreateRunningOtherTaskActionDialog::accept()
{
    if(ui->tableWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, tr("No file selected"), G_Sentences::NoFileSelected());
        return;
    };


    m_filename = ui->tableWidget->selectedItems().at(0)->text().chopped(5);
    m_delay = QTime(0,0,0).secsTo(ui->timeEdit_2->time());
    m_delay+= ui->spinBox_4->value() * 86400;

    if(ui->checkBox_2->isChecked())
        m_loopTimes = -1;
    else
        m_loopTimes = ui->spinBox_3->value();

    emit sendRunningOtherTask(m_filename,m_delay,m_loopTimes);
    QDialog::accept();
}

void CreateRunningOtherTaskActionDialog::fillExistingTasksTable()
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

void CreateRunningOtherTaskActionDialog::loopToggled(bool state)
{
    if(state)
        ui->spinBox_3->setEnabled(false);
    else
        ui->spinBox_3->setEnabled(true);
}
