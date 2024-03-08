#include "CreateWaitActionDialog.h"
#include "ui_CreateWaitActionDialog.h"

CreateWaitActionDialog::CreateWaitActionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateWaitActionDialog)
{
    ui->setupUi(this);
    ui->doubleSpinBox->setLocale(QLocale::English);

    connect(ui->bySecondsGroup, &QGroupBox::clicked, this, &CreateWaitActionDialog::uncheckByDayBasedGroup);
    connect(ui->byDayGroup, &QGroupBox::clicked, this, &CreateWaitActionDialog::uncheckBySecondsGroup);
}

CreateWaitActionDialog::~CreateWaitActionDialog()
{
    delete ui;
}

void CreateWaitActionDialog::uncheckBySecondsGroup()
{
    ui->bySecondsGroup->setChecked(false);
    ui->byDayGroup->setChecked(true);
}

void CreateWaitActionDialog::uncheckByDayBasedGroup()
{
    ui->byDayGroup->setChecked(false);
    ui->bySecondsGroup->setChecked(true);
}

void CreateWaitActionDialog::accept()
{
    if(ui->bySecondsGroup->isChecked())
        emit sendDuration(ui->doubleSpinBox->value());
    else
    {
        long double totalDuration = QTime(0,0,0).secsTo(ui->timeEdit->time());
        totalDuration += ui->spinBox->value() * 86400;
        emit sendDuration(totalDuration);
    }

    QDialog::accept();
}

void CreateWaitActionDialog::showDialog()
{
    show();
}
