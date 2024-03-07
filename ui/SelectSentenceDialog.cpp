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
}

void SelectSentenceDialog::accept()
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
    emit sendSentenceIdentity(trueId);

    QDialog::accept();
}

