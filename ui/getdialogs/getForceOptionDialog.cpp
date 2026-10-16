#include "getForceOptionDialog.h"
#include "ui_getForceOptionDialog.h"
#include "globals.h"
#include <QPushButton>

getForceOptionDialog::getForceOptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::getForceOptionDialog)
{
    ui->setupUi(this);
}

getForceOptionDialog::~getForceOptionDialog()
{
    delete ui;
}

void getForceOptionDialog::accept()
{
    emit sendForce(ui->checkBox->isChecked());

    QDialog::accept();
}

void getForceOptionDialog::showDialog()
{
    QDialog::show();
    auto buttonSender = qobject_cast<QPushButton*>(sender());
    if(buttonSender == nullptr)
        return;
    ui->checkBox->setChecked(buttonSender->text() == G_Sentences::ForceOption);
}
