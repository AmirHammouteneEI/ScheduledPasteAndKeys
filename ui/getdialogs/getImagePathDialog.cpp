#include "getImagePathDialog.h"
#include "ui_getImagePathDialog.h"
#include <QFileDialog>
#include <QApplication>

getImagePathDialog::getImagePathDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::getImagePathDialog)
{
    ui->setupUi(this);
    ui->pushButton->setObjectName("browseButton");
    connect(ui->pushButton, &QPushButton::released, this, &getImagePathDialog::showBrowseFilesDialog);

}

getImagePathDialog::~getImagePathDialog()
{
    delete ui;
}

void getImagePathDialog::showBrowseFilesDialog()
{
    QString dir = QFileDialog::getSaveFileName(this, tr("Define the image file path"),ui->lineEdit->text(),tr("PNG (*.png)"));

    ui->lineEdit->setText(dir);
}

void getImagePathDialog::accept()
{
    emit sendImage(ui->lineEdit->text());

    QDialog::accept();
}

void getImagePathDialog::showDialog()
{
    QDialog::show();
    auto buttonSender = qobject_cast<QPushButton*>(sender());
    if(buttonSender == nullptr)
        return;
    ui->lineEdit->setText(buttonSender->text());
}
