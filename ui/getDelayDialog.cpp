#include "getDelayDialog.h"
#include "ui_getDelayDialog.h"

#include <QPushButton>
#include <QMessageBox>

getDelayDialog::getDelayDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::getDelayDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Schedule task for run"));

    connect(ui->delayGroup, &QGroupBox::clicked, this, &getDelayDialog::uncheckDateGroup);
    connect(ui->dateTimeGroup, &QGroupBox::clicked, this, &getDelayDialog::uncheckDelayGroup);
    connect(ui->dateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &getDelayDialog::refreshDateDelayDetails);
}

getDelayDialog::~getDelayDialog()
{
    delete ui;
}

void getDelayDialog::uncheckDelayGroup()
{
    ui->delayGroup->setChecked(false);
    ui->dateTimeGroup->setChecked(true);
}

void getDelayDialog::uncheckDateGroup()
{
    ui->dateTimeGroup->setChecked(false);
    ui->delayGroup->setChecked(true);
}

void getDelayDialog::accept()
{
    m_savedDelay = ui->timeEdit->time();

    if(ui->delayGroup->isChecked())
        emit sendDelay(QTime(0,0,0).secsTo(m_savedDelay));
    else
    {
        QDateTime cur = QDateTime::currentDateTime();
        qint64 delayInSec = cur.secsTo(ui->dateTimeEdit->dateTime());
        if(delayInSec < 0)
        {
            QMessageBox::warning(this, tr("Date selected not correct"), tr("Date/time selected is not later than the current date/time, please set a correct date."));
            return;
        }
        emit sendDelay(delayInSec);
    }

    QDialog::accept();
}

void getDelayDialog::refreshDateDelayDetails(const QDateTime & datet)
{
    qint64 dateDelay = QDateTime::currentDateTime().secsTo(datet);
    qint64 secs = dateDelay % 60;
    qint64 minNum = (dateDelay-secs)/60;
    qint64 mins = minNum % 60;
    qint64 hourNum = (minNum-mins)/ 60 ;
    qint64 hours = hourNum %24;
    qint64 days = (hourNum - hours)/ 24;
    ui->delayDetailsLabel->setText(QString::number(days)+tr(" days ")+QString::number(hours)+tr(" hours ")
                                   +QString::number(mins)+tr(" mins ")+QString::number(secs)+tr(" secs "));
}

void getDelayDialog::showDialog()
{
    ui->timeEdit->setTime(m_savedDelay);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(60));
    refreshDateDelayDetails(ui->dateTimeEdit->dateTime());
    show();
}
