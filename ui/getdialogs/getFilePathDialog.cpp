#include "getFilePathDialog.h"
#include "ui_getFilePathDialog.h"
#include <QFileDialog>

getFilePathDialog::getFilePathDialog(QWidget *parent, bool getSaved)
    : QDialog(parent), m_getSavedFileName(getSaved)
    , ui(new Ui::getFilePathDialog)
{
    ui->setupUi(this);
    ui->pushButton->setObjectName("browseButton");
    connect(ui->pushButton, &QPushButton::released, this, &getFilePathDialog::showBrowseFilesDialog);
}

getFilePathDialog::~getFilePathDialog()
{
    delete ui;
}

void getFilePathDialog::showBrowseFilesDialog()
{
    QString path;
    if(m_getSavedFileName)
        path = QFileDialog::getSaveFileName(this, tr("Select a file"),ui->lineEdit->text());
    else
        path = QFileDialog::getOpenFileName(this, tr("Select a file"),ui->lineEdit->text());

    if(!path.isEmpty())
        ui->lineEdit->setText(path);
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
