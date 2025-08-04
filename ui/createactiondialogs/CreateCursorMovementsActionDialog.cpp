#include "CreateCursorMovementsActionDialog.h"
#include "ui_CreateCursorMovementsActionDialog.h"
#include "globals.h"

#include <QMessageBox>

CreateCursorMovementsActionDialog::CreateCursorMovementsActionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateCursorMovementsActionDialog)
{
    ui->setupUi(this);
    connect(ui->addCursorMovementsButton, &QPushButton::released, ui->tableWidget, &CursorMovementsTableWidget::createCursorMovementsReceived);
}

CreateCursorMovementsActionDialog::~CreateCursorMovementsActionDialog()
{
    delete ui;
}

void CreateCursorMovementsActionDialog::showDialog()
{
    ui->tableWidget->refresh();
    show();

    // if shows from an existing Keys Sequence Widget
    auto mainButtonSender = qobject_cast<QPushButton*>(sender());
    if(mainButtonSender == nullptr)
        return;

    if(!mainButtonSender->property("cursorMovsId").isValid() || mainButtonSender->property("cursorMovsId").toString() == tr("ERROR"))
        return;

    ui->tableWidget->selectCursorMovementsFromIdentity(mainButtonSender->property("cursorMovsId").toString());
}

void CreateCursorMovementsActionDialog::accept()
{
    QItemSelectionModel *selection = ui->tableWidget->selectionModel();
    if(!selection->hasSelection())
    {
        QMessageBox::warning(this, tr("No cursor movements set selected"),G_Sentences::NoCursorMovementsSelected());
        return;
    }

    int row = selection->selectedRows().at(0).row();
    QTableWidgetItem *idItem = ui->tableWidget->item(row,0);
    if(idItem == nullptr)
    {
        QMessageBox::warning(nullptr, tr("Internal Error"),
                             tr("An internal error occured : err06\nitem selected in CreateCursorMovementsActionDialog is null !"));

        return;
    }

    QString trueId = idItem->text().remove(0,1);

    emit sendCursorMovements(trueId);

    QDialog::accept();
}
