#include "GlobalHotKeyEditDialog.h"
#include "ui_GlobalHotKeyEditDialog.h"
#include "globals.h"

#include <QDir>
#include <QMessageBox>
#include <QPushButton>

GlobalHotKeyEditDialog::GlobalHotKeyEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GlobalHotKeyEditDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Add"));
    connect(ui->checkBox_,&QCheckBox::toggled, this, &GlobalHotKeyEditDialog::loopToggled);
}

GlobalHotKeyEditDialog::~GlobalHotKeyEditDialog()
{
    delete ui;
}

void GlobalHotKeyEditDialog::showDialog()
{
    fillExistingTasksTable();
    ui->timeEdit_->setTime(QTime(0,1,0));
    ui->spinBox_->setValue(0);
    ui->checkBox_->setChecked(false);
    ui->spinBox_2_->setValue(1);
    ui->spinBox_2_->setEnabled(true);
    ui->keySequenceEdit->clear();
    show();
}

void GlobalHotKeyEditDialog::accept()
{
    if(ui->tableWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, tr("No file selected"), G_Sentences::NoFileSelected());
        return;
    }

    m_filename = ui->tableWidget->selectedItems().at(0)->text().chopped(5);
    m_delay = QTime(0,0,0).secsTo(ui->timeEdit_->time());
    m_delay+= ui->spinBox_->value() * 86400;

    if(ui->checkBox_->isChecked())
        m_loopTimes = -1;
    else
        m_loopTimes = ui->spinBox_2_->value();

    if(ui->keySequenceEdit->keySequence().isEmpty())
    {
        QMessageBox::warning(this, tr("No hotkey defined"), QObject::tr("No hotkey has been defined to run the task. Please define one."));
        return;
    }
    m_keySeq = ui->keySequenceEdit->keySequence();

    QDialog::accept();
}

void GlobalHotKeyEditDialog::loopToggled(bool state)
{
    if(state)
        ui->spinBox_2_->setEnabled(false);
    else
        ui->spinBox_2_->setEnabled(true);
}

void GlobalHotKeyEditDialog::fillExistingTasksTable()
{
    QDir tasksFolder = QApplication::applicationDirPath()+"/"+G_Files::TasksFolder;
    if(!tasksFolder.exists())
        return;

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
