#include "KeysSelectorDialog.h"
#include "ui_KeysSelectorDialog.h"

KeysSelectorDialog::KeysSelectorDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::KeysSelectorDialog)
{
    ui->setupUi(this);
    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &KeysSelectorDialog::appendToList);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &KeysSelectorDialog::removeFromList);
}

KeysSelectorDialog::~KeysSelectorDialog()
{
    delete ui;
}

void KeysSelectorDialog::appendToList(QTreeWidgetItem *item, int)
{
    if(item==nullptr || item->parent() == nullptr)
        return;
    ui->listWidget->addItem(item->text(0));
}

void KeysSelectorDialog::removeFromList(QListWidgetItem *item)
{
    if(item == nullptr)
        return;
    delete ui->listWidget->takeItem(ui->listWidget->row(item));
}

void KeysSelectorDialog::accept()
{
    QStringList sentList;
    for(int k = 0; k < ui->listWidget->count(); ++k)
    {
        sentList.append(ui->listWidget->item(k)->text());
    }

    emit sendKeysList(sentList);
    QDialog::accept();
}

void KeysSelectorDialog::showDialog()
{
    QObject* sender = QObject::sender();
    if(sender == nullptr)
        return;

    ui->listWidget->clear();
    ui->listWidget->addItems(sender->property("keys").toStringList());
    show();
}
