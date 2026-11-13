#include "getQKeySequenceDialog.h"
#include "ui_getQKeySequenceDialog.h"

getQKeySequenceDialog::getQKeySequenceDialog(const QKeySequence &currentSeq, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::getQKeySequenceDialog)
{
    ui->setupUi(this);
    ui->keySequenceEdit->setKeySequence(currentSeq);
}

getQKeySequenceDialog::~getQKeySequenceDialog()
{
    delete ui;
}

QKeySequence getQKeySequenceDialog::keySequence() const
{
    return ui->keySequenceEdit->keySequence();
}
