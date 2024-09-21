#include "DataEditDialog.h"
#include "ui_DataEditDialog.h"

DataEditDialog::DataEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DataEditDialog)
{
    ui->setupUi(this);

    QFont font = ui->tabWidget->tabBar()->font();
    font.setPointSize(14);
    ui->tabWidget->tabBar()->setFont(font);
    ui->sentencesTableWidget->m_belongsToDataEditDialog = true;
    ui->keysSequencesTableWidget->m_belongsToDataEditDialog = true;
    ui->cursorMovementsTableWidget->m_belongsToDataEditDialog = true;

    connect(ui->addSentenceButton, &QPushButton::released, ui->sentencesTableWidget, &SentencesTableWidget::createSentenceReceived);
    connect(ui->removeSentenceButton, &QPushButton::released, ui->sentencesTableWidget, &SentencesTableWidget::removeSentenceReceived);
    connect(ui->addKeysSequenceButton, &QPushButton::released, ui->keysSequencesTableWidget, &KeysSequencesTableWidget::createKeysSequenceReceived);
    connect(ui->removeKeysSequenceButton, &QPushButton::released, ui->keysSequencesTableWidget, &KeysSequencesTableWidget::removeKeysSequenceReceived);
    connect(ui->addCursorMovementsButton, &QPushButton::released, ui->cursorMovementsTableWidget, &CursorMovementsTableWidget::createCursorMovementsReceived);
    connect(ui->removeCursorMovementsButton, &QPushButton::released, ui->cursorMovementsTableWidget, &CursorMovementsTableWidget::removeCursorMovementsReceived);
}

DataEditDialog::~DataEditDialog()
{
    delete ui;
}

void DataEditDialog::showDialog()
{
    ui->sentencesTableWidget->refresh();
    ui->keysSequencesTableWidget->refresh();
    ui->cursorMovementsTableWidget->refresh();
    show();
}
