#include "CreateWaitActionDialog.h"
#include "ui_CreateWaitActionDialog.h"
#include <QPushButton>

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

    // if shows from an existing Wait Widget
    auto mainButtonSender = qobject_cast<QPushButton*>(sender());
    if(mainButtonSender == nullptr)
        return;

    if(!mainButtonSender->property("duration").isValid() || mainButtonSender->property("duration").toString() == tr("ERROR"))
        return;

    double dur = mainButtonSender->property("duration").toDouble();
    ui->doubleSpinBox->setValue(dur);

    qint64 secs = (int)dur % 60;
    qint64 minNum = (dur-secs)/60;
    qint64 mins = minNum % 60;
    qint64 hourNum = (minNum-mins)/ 60 ;
    qint64 hours = hourNum %24;
    qint64 days = (hourNum - hours)/ 24;

    ui->timeEdit->setTime(QTime(hours,mins,secs));
    ui->spinBox->setValue(days);
}
