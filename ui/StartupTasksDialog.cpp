#include "StartupTasksDialog.h"
#include "ui_StartupTasksDialog.h"
#include "ui/StartupTasksTableWidget.h"

StartupTasksDialog::StartupTasksDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartupTasksDialog)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->resizeSection(1,80);
    ui->tableWidget->horizontalHeader()->resizeSection(2,50);
    connect(ui->addButton, &QPushButton::released, ui->tableWidget, &StartupTasksTableWidget::addStartupTaskReceived);
    connect(ui->removeButton, &QPushButton::released, ui->tableWidget, &StartupTasksTableWidget::removeStartupTaskReceived);
}

StartupTasksDialog::~StartupTasksDialog()
{
    delete ui;
}

void StartupTasksDialog::showDialog()
{
    ui->tableWidget->refresh();
    show();
}
