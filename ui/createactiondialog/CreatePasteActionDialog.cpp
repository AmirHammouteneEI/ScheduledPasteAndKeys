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
}

void CreatePasteActionDialog::accept()
{
    QItemSelectionModel *selection = ui->tableWidget->selectionModel();
    if(!selection->hasSelection())
    {
        QMessageBox::warning(this, tr("No sentence selected"),G_Sentences::NoSetenceSelected);
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
    float waitSec = -1.f;
    if(ui->groupBox->isChecked())
        waitSec = ui->doubleSpinBox->value();

    emit createPasteActionRequest(trueId,waitSec);

    QDialog::accept();
}
