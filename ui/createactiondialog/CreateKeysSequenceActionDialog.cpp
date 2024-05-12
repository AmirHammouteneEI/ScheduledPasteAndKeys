#include "CreateKeysSequenceActionDialog.h"
#include "ui_CreateKeysSequenceActionDialog.h"
#include "globals.h"

#include <QMessageBox>

CreateKeysSequenceActionDialog::CreateKeysSequenceActionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateKeysSequenceActionDialog)
{
    ui->setupUi(this);
    connect(ui->addKeysSequenceButton, &QPushButton::released, ui->tableWidget, &KeysSequencesTableWidget::createKeysSequenceReceived);
}

CreateKeysSequenceActionDialog::~CreateKeysSequenceActionDialog()
{
    delete ui;
}

void CreateKeysSequenceActionDialog::showDialog()
{
    //TODO ui->tableWidget->refresh();
    show();
    //TODO if shows from an existing Keys Sequence Widget
}

void CreateKeysSequenceActionDialog::accept()
{
    QItemSelectionModel *selection = ui->tableWidget->selectionModel();
    if(!selection->hasSelection())
    {
        QMessageBox::warning(this, tr("No keys sequence selected"),G_Sentences::NoKeysSequenceSelected());
        return;
    }

    int row = selection->selectedRows().at(0).row();
    QTableWidgetItem *idItem = ui->tableWidget->item(row,0);
    if(idItem == nullptr)
    {
        QMessageBox::warning(nullptr, tr("Internal Error"),
                             tr("An internal error occured : err03\nitem selected in CreateKeysSequenceActionDialog is null !"));

        return;
    }

    QString trueId = idItem->text().remove(0,1);

    emit sendKeysSequence(trueId);

    QDialog::accept();
}
