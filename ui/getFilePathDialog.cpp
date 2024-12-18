#include "getFilePathDialog.h"
#include "ui_getFilePathDialog.h"
#include <QFileDialog>
#include <QApplication>

getFilePathDialog::getFilePathDialog(QWidget *parent, bool getSaved)
    : QDialog(parent), m_getSavedFileName(getSaved)
    , ui(new Ui::getFilePathDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &getFilePathDialog::showBrowseFilesDialog);
}

getFilePathDialog::~getFilePathDialog()
{
    delete ui;
}

void getFilePathDialog::showBrowseFilesDialog()
{
    QString dir;
    if(m_getSavedFileName)
        dir = QFileDialog::getSaveFileName(this, tr("Select a file"),ui->lineEdit->text());
    else
        dir = QFileDialog::getOpenFileName(this, tr("Select a file"),ui->lineEdit->text());

    ui->lineEdit->setText(dir);
}

void getFilePathDialog::accept()
{
    emit sendFile(ui->lineEdit->text());

    QDialog::accept();
}

void getFilePathDialog::showDialog()
{
    QDialog::show();
    auto buttonSender = qobject_cast<QPushButton*>(sender());
    if(buttonSender == nullptr)
        return;
    ui->lineEdit->setText(buttonSender->text());
}
