#include "KeysSequenceSelectedEditDialog.h"
#include "ui_KeysSequenceSelectedEditDialog.h"
#include "KeysSelectorDialog.h"
#include <QDoubleSpinBox>
#include <QPushButton>

KeysSequenceSelectedEditDialog::KeysSequenceSelectedEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::KeysSequenceSelectedEditDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\w+"), this));
    connect(ui->addKeysButton, &QPushButton::released, this,&KeysSequenceSelectedEditDialog::addKeysRow);
    connect(ui->removeKeysButton, &QPushButton::released, this,&KeysSequenceSelectedEditDialog::removeLastKeysRow);
}

KeysSequenceSelectedEditDialog::~KeysSequenceSelectedEditDialog()
{
    delete ui;
}

void KeysSequenceSelectedEditDialog::setEditable(bool id)
{
    ui->lineEdit->setReadOnly(!id);
}

void KeysSequenceSelectedEditDialog::setIdentity(const QString &id)
{
    ui->lineEdit->setText(id);
}

QString KeysSequenceSelectedEditDialog::identity()
{
    return ui->lineEdit->text();
}

void KeysSequenceSelectedEditDialog::setTableKeysSequence(const PressedReleaseDelaysKeysMap &tableContent)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    for(auto [key,val] : tableContent.asKeyValueRange())
    {
        QList<QWidget*> widgetsList = addKeysRow();
        if(widgetsList.size() != 3)
            break;

        auto pressedDelaySpin = qobject_cast<QDoubleSpinBox*>(widgetsList.at(0));
        if(pressedDelaySpin == nullptr)
            break;
        pressedDelaySpin->setValue(key/1000.);

        auto releasedDelaySpin = qobject_cast<QDoubleSpinBox*>(widgetsList.at(1));
        if(releasedDelaySpin == nullptr)
            break;
        releasedDelaySpin->setValue(val.first/1000.);

        auto keysListButton = qobject_cast<QPushButton*>(widgetsList.at(2));
        if(keysListButton == nullptr)
            break;
        keysListButton->setProperty("keys",val.second);
        keysListButton->setText(val.second.join("+"));
    }
}

PressedReleaseDelaysKeysMap KeysSequenceSelectedEditDialog::tableKeysSequence()
{
    PressedReleaseDelaysKeysMap returnedMap;

    for(int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        auto pressedDelaySpin = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(row, 0));
        auto releasedDelaySpin = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(row, 1));
        auto keysListButton = qobject_cast<QPushButton*>(ui->tableWidget->cellWidget(row, 2));
        if(pressedDelaySpin == nullptr || releasedDelaySpin == nullptr || keysListButton == nullptr )
            break;
        returnedMap.insert(pressedDelaySpin->value()*1000,
        QPair<int,QStringList>(releasedDelaySpin->value()*1000,keysListButton->property("keys").toStringList()));
    }
    return returnedMap;
}

QList<QWidget*> KeysSequenceSelectedEditDialog::addKeysRow()
{
    int index = ui->tableWidget->rowCount();
    QDoubleSpinBox *pressedDelaySpin = new QDoubleSpinBox(this);
    pressedDelaySpin->setDecimals(2);
    pressedDelaySpin->setMaximum(999999999999.);
    pressedDelaySpin->setSingleStep(0.1);
    pressedDelaySpin->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    if(index == 0)
    {
        pressedDelaySpin->setValue(0.);
        pressedDelaySpin->setEnabled(false);
    }
    else
    {
        auto previousSpin = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(index-1,0));
        if(previousSpin!= nullptr)
            pressedDelaySpin->setMinimum(previousSpin->value()+0.1);
    }

    QDoubleSpinBox *releasedDelaySpin = new QDoubleSpinBox(this);
    releasedDelaySpin->setDecimals(2);
    releasedDelaySpin->setMinimum(0.1);
    releasedDelaySpin->setMaximum(999999999999.);
    releasedDelaySpin->setSingleStep(0.1);
    releasedDelaySpin->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);

    QPushButton *keysListButton = new QPushButton(this);

    KeysSelectorDialog *keysSelector = new KeysSelectorDialog(keysListButton);
    connect(keysListButton, &QPushButton::released, keysSelector, &KeysSelectorDialog::showDialog);
    connect(keysSelector, &KeysSelectorDialog::sendKeysList, this,[=](QStringList keysList)
        {
            keysListButton->setProperty("keys", keysList);
            keysListButton->setText(keysList.join("+"));
        });

    ui->tableWidget->insertRow(index);
    ui->tableWidget->setCellWidget(index,0,pressedDelaySpin);
    ui->tableWidget->setCellWidget(index,1,releasedDelaySpin);
    ui->tableWidget->setCellWidget(index,2,keysListButton);

    QList<QWidget*> returnedList;
    returnedList << pressedDelaySpin << releasedDelaySpin << keysListButton;

    return returnedList;
}

void KeysSequenceSelectedEditDialog::removeLastKeysRow()
{
    if(ui->tableWidget->rowCount() > 0)
        ui->tableWidget->removeRow(ui->tableWidget->rowCount()-1);
}
