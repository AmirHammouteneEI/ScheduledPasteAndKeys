#include "CreatePasteActionDialog.h"
#include "ui_CreatePasteActionDialog.h"
#include "globals.h"

#include <QMessageBox>

CreatePasteActionDialog::CreatePasteActionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreatePasteActionDialog)
{
    ui->setupUi(this);
    connect(ui->addSentenceButton, &QPushButton::released, ui->tableWidget, &SentencesTableWidget::createSentenceReceived);
}

CreatePasteActionDialog::~CreatePasteActionDialog()
{
    delete ui;
}

void CreatePasteActionDialog::showDialog()
{
    ui->tableWidget->refresh();
    show();

    // if shows from an existing Paste Widget
    auto mainButtonSender = qobject_cast<QPushButton*>(sender());
    if(mainButtonSender == nullptr)
        return;

    if(!mainButtonSender->property("contentId").isValid() || mainButtonSender->property("contentId").toString() == tr("ERROR"))
        return;

    QTableWidgetItem *idItem = nullptr;
    int rowFound = -1;
    for(int k=0; k< ui->tableWidget->rowCount(); ++k)
    {
        idItem = ui->tableWidget->item(k,0);
        if(idItem != nullptr && idItem->text() == "#"+mainButtonSender->property("contentId").toString())
        {
            rowFound = k;
            break;
        }
    }

    if(rowFound >= 0)
    {
        ui->tableWidget->selectRow(rowFound);
        ui->tableWidget->scrollToItem(idItem,QAbstractItemView::PositionAtCenter);
    }
}

void CreatePasteActionDialog::accept()
{
    QItemSelectionModel *selection = ui->tableWidget->selectionModel();
    if(!selection->hasSelection())
    {
        QMessageBox::warning(this, tr("No sentence selected"),G_Sentences::NoSetenceSelected());
        return;
    }

    int row = selection->selectedRows().at(0).row();
    QTableWidgetItem *idItem = ui->tableWidget->item(row,0);
    if(idItem == nullptr)
    {
        QMessageBox::warning(nullptr, tr("Internal Error"),
          tr("An internal error occured : err04\nitem selected in CreatePasteActionDialog is null !"));

        return;
    }

    QString trueId = idItem->text().remove(0,1);

    emit sendSentence(trueId);

    QDialog::accept();
}
