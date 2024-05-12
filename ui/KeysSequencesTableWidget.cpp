#include "KeysSequencesTableWidget.h"
#include "globals.h"

#include <QSettings>
#include <QMessageBox>

KeysSequencesTableWidget::KeysSequencesTableWidget(QWidget *parent)
    : QTableWidget{parent}
{
    //TODO next step MODEL OF THE TABLE : 2x doulblespinbox + button Open dialog for choose keys (treewidget of keys label)
    m_keysSequenceEditDialog = new KeysSequenceSelectedEditDialog(this);
    connect(m_keysSequenceEditDialog, &QDialog::accepted, this, &KeysSequencesTableWidget::editFromDialogReceived);
    connect(this, &QTableWidget::cellDoubleClicked, this, &KeysSequencesTableWidget::editKeysSequenceSelected);
}

void KeysSequencesTableWidget::createKeysSequenceReceived()
{
    //TODO next step
}

void KeysSequencesTableWidget::removeKeysSequenceReceived()
{
    //TODO next step
}

void KeysSequencesTableWidget::editFromDialogReceived()
{
    //TODO next step
}

void KeysSequencesTableWidget::editKeysSequenceSelected(int row, int)
{
    //TODO next step
}
