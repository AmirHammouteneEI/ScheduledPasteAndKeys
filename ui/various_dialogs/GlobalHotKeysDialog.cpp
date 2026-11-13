#include "GlobalHotKeysDialog.h"
#include "ui_GlobalHotKeysDialog.h"
#include "ui/tablewidgets/GlobalHotKeysTableWidget.h"

GlobalHotKeysDialog::GlobalHotKeysDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GlobalHotKeysDialog)
{
    ui->setupUi(this);
    ui->addButton->setObjectName("plusminusButton");
    ui->removeButton->setObjectName("plusminusButton");
    ui->tableWidget->horizontalHeader()->resizeSection(0,150);
    ui->tableWidget->horizontalHeader()->resizeSection(1,300);
    connect(ui->addButton, &QPushButton::released, ui->tableWidget, &GlobalHotKeysTableWidget::addHotkeyReceived);
    connect(ui->removeButton, &QPushButton::released, ui->tableWidget, &GlobalHotKeysTableWidget::removeHotkeyReceived);
}

GlobalHotKeysDialog::~GlobalHotKeysDialog()
{
    delete ui;
}

void GlobalHotKeysDialog::refreshTable()
{
    ui->tableWidget->refresh();
}

void GlobalHotKeysDialog::showDialog()
{
    ui->tableWidget->refresh();
    show();
}
