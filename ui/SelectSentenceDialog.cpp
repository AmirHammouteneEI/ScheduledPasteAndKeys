#include "SelectSentenceDialog.h"
#include "ui_SelectSentenceDialog.h"
#include "globals.h"

#include <QMessageBox>

SelectSentenceDialog::SelectSentenceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectSentenceDialog)
{
    ui->setupUi(this);
    ui->tableWidget->refresh();
    connect(ui->addSentenceButton, &QPushButton::released, ui->tableWidget, &SentencesTableWidget::createSentenceReceived);
}

SelectSentenceDialog::~SelectSentenceDialog()
{
    delete ui;
}

void SelectSentenceDialog::showDialog()
{
    ui->tableWidget->refresh();
    show();

    // if shows from an existing Paste Widget
    auto mainButtonSender = qobject_cast<QPushButton*>(sender());
    if(mainButtonSender == nullptr)
        return;

    if(!mainButtonSender->property("contentId").isValid())
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

void SelectSentenceDialog::accept()
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
           tr("An internal error occured : err03\nitem selected in SelectSentenceDialog is null !"));

        return;
    }

    QString trueId = idItem->text().remove(0,1);
    emit sendSentenceIdentity(trueId);

    QDialog::accept();
}

