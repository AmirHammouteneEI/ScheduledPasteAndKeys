#include "getFolderPathDialog.h"
#include "ui_getFolderPathDialog.h"
#include <QFileDialog>
#include <QApplication>

getFolderPathDialog::getFolderPathDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::getFolderPathDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &getFolderPathDialog::showBrowseFoldersDialog);
}

getFolderPathDialog::~getFolderPathDialog()
{
    delete ui;
}

void getFolderPathDialog::showBrowseFoldersDialog()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select a directory"),ui->lineEdit->text(),QFileDialog::ShowDirsOnly);

    ui->lineEdit->setText(dir);
}

void getFolderPathDialog::accept()
{
    emit sendDirectory(ui->lineEdit->text());

    QDialog::accept();
}

void getFolderPathDialog::showDialog()
{
    QDialog::show();
    auto buttonSender = qobject_cast<QPushButton*>(sender());
    if(buttonSender == nullptr)
        return;
    ui->lineEdit->setText(buttonSender->text());
}
