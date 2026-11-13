#include "StartupTasksDialog.h"
#include "ui_StartupTasksDialog.h"
#include "ui/tablewidgets/StartupTasksTableWidget.h"

StartupTasksDialog::StartupTasksDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartupTasksDialog)
{
    ui->setupUi(this);
    ui->addButton->setObjectName("plusminusButton");
    ui->removeButton->setObjectName("plusminusButton");
    ui->tableWidget->horizontalHeader()->resizeSection(0,300);
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
