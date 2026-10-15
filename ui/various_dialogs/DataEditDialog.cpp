#include "DataEditDialog.h"
#include "ui_DataEditDialog.h"
#include "globals.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

DataEditDialog::DataEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DataEditDialog)
{
    ui->setupUi(this);

    ui->tabWidget->tabBar()->setObjectName("dataTabBar");
    ui->sentencesTableWidget->m_belongsToDataEditDialog = true;
    ui->keysSequencesTableWidget->m_belongsToDataEditDialog = true;
    ui->cursorMovementsTableWidget->m_belongsToDataEditDialog = true;

    ui->addSentenceButton->setObjectName("plusminusButton");
    ui->removeSentenceButton->setObjectName("plusminusButton");
    ui->addKeysSequenceButton->setObjectName("plusminusButton");
    ui->removeKeysSequenceButton->setObjectName("plusminusButton");
    ui->addCursorMovementsButton->setObjectName("plusminusButton");
    ui->removeCursorMovementsButton->setObjectName("plusminusButton");

    connect(ui->addSentenceButton, &QPushButton::released, ui->sentencesTableWidget, &SentencesTableWidget::createSentenceReceived);
    connect(ui->removeSentenceButton, &QPushButton::released, ui->sentencesTableWidget, &SentencesTableWidget::removeSentenceReceived);
    connect(ui->addKeysSequenceButton, &QPushButton::released, ui->keysSequencesTableWidget, &KeysSequencesTableWidget::createKeysSequenceReceived);
    connect(ui->removeKeysSequenceButton, &QPushButton::released, ui->keysSequencesTableWidget, &KeysSequencesTableWidget::removeKeysSequenceReceived);
    connect(ui->addCursorMovementsButton, &QPushButton::released, ui->cursorMovementsTableWidget, &CursorMovementsTableWidget::createCursorMovementsReceived);
    connect(ui->removeCursorMovementsButton, &QPushButton::released, ui->cursorMovementsTableWidget, &CursorMovementsTableWidget::removeCursorMovementsReceived);

    connect(ui->exportButton, &QPushButton::released, this, &DataEditDialog::onExportData);
    connect(ui->importButton, &QPushButton::released, this, &DataEditDialog::onImportData);
}

DataEditDialog::~DataEditDialog()
{
    delete ui;
}

void DataEditDialog::onExportData()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Define where to save the data file"),G_Files::DataFilePath,tr("INI file (*.ini)"));

    if(!path.isEmpty())
    {
        QFile f(path);
        f.remove();
        if(!QFile::copy(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath,path))
        {
            QMessageBox::warning(this, tr("Cannot save this file"), G_Sentences::OperationInterference());
            return;
        }
    }
}

void DataEditDialog::onImportData()
{
    if(QMessageBox::question(this, tr("Confirm data copy"),
                              tr("Data will be added from the programData.ini file you will select.\nIf some data already exist, the '_other' suffix will be added to identities."),
                              QMessageBox::StandardButtons(QMessageBox::Ok | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::Cancel)) == QMessageBox::Cancel)
        return;
    else
    {
        QString path = QFileDialog::getOpenFileName(this, tr("Select a data file"),"",tr("INI file (*.ini)"));

        if(path.isEmpty())
            return;
        QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::DataFilePath, QSettings::IniFormat);
        QSettings settingsToCopy(path, QSettings::IniFormat);
        QStringList keys = settingsToCopy.allKeys();
        for(const auto &key : keys)
        {
            if(!key.startsWith(G_Files::SentencesDataCategory) && !key.startsWith(G_Files::KeysSequencesDataCategory) && !key.startsWith(G_Files::CursorMovementsDataCategory))
                continue;
            QString keyToSave = key;
            if(settings.contains(key))
                keyToSave+="_other";
            settings.setValue(keyToSave,settingsToCopy.value(key));
        }

        ui->sentencesTableWidget->refresh();
        ui->keysSequencesTableWidget->refresh();
        ui->cursorMovementsTableWidget->refresh();
    }
}

void DataEditDialog::showDialog()
{
    ui->sentencesTableWidget->refresh();
    ui->keysSequencesTableWidget->refresh();
    ui->cursorMovementsTableWidget->refresh();
    show();
}
