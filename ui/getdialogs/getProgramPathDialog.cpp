#include "getProgramPathDialog.h"
#include "ui_getProgramPathDialog.h"
#include <QFileDialog>
#include <QApplication>

getProgramPathDialog::getProgramPathDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::getProgramPathDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &getProgramPathDialog::showBrowseFilesDialog);
}

getProgramPathDialog::~getProgramPathDialog()
{
    delete ui;
}

void getProgramPathDialog::showBrowseFilesDialog()
{
    QString dir;
    dir = QFileDialog::getOpenFileName(this, tr("Select a program"),ui->lineEdit->text());

    ui->lineEdit->setText(dir);
}

void getProgramPathDialog::accept()
{
    emit sendProgram(ui->lineEdit->text());

    QDialog::accept();
}

void getProgramPathDialog::showDialog()
{
    QDialog::show();
    auto buttonSender = qobject_cast<QPushButton*>(sender());
    if(buttonSender == nullptr)
        return;
    ui->lineEdit->setText(buttonSender->text());
}
