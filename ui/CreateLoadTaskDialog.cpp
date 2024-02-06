#include "CreateLoadTaskDialog.h"
#include "ui_CreateLoadTaskDialog.h"
#include <QPushButton>

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
