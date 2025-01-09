#include "getAutoRenameOptionDialog.h"
#include "ui_getAutoRenameOptionDialog.h"
#include <QPushButton>

getAutoRenameOptionDialog::getAutoRenameOptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::getAutoRenameOptionDialog)
{
    ui->setupUi(this);
}

getAutoRenameOptionDialog::~getAutoRenameOptionDialog()
{
    delete ui;
}

void getAutoRenameOptionDialog::accept()
{
    emit sendAutorename(ui->checkBox->isChecked());

    QDialog::accept();
}

void getAutoRenameOptionDialog::showDialog()
{
    QDialog::show();
    auto buttonSender = qobject_cast<QPushButton*>(sender());
    if(buttonSender == nullptr)
        return;
    ui->checkBox->setChecked(buttonSender->text() == "Auto-rename if exists");
}
