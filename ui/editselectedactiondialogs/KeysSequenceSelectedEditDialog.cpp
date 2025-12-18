#include "KeysSequenceSelectedEditDialog.h"
#include "ui_KeysSequenceSelectedEditDialog.h"
#include "ui/various_dialogs//KeysSelectorDialog.h"
#include "ui/CustomPrimaryWidgets.h"
#include <QPushButton>
#include <QMessageBox>

KeysSequenceSelectedEditDialog::KeysSequenceSelectedEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::KeysSequenceSelectedEditDialog)
{
    ui->setupUi(this);
    ui->addKeysButton->setObjectName("plusminusButton");
    ui->removeKeysButton->setObjectName("plusminusButton");
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

        auto pressedDelaySpin = qobject_cast<NoWheelFocusDoubleSpinBox*>(widgetsList.at(0));
        if(pressedDelaySpin == nullptr)
            break;
        pressedDelaySpin->setValue(key/1000.);

        auto releasedDelaySpin = qobject_cast<NoWheelFocusDoubleSpinBox*>(widgetsList.at(1));
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
        auto pressedDelaySpin = qobject_cast<NoWheelFocusDoubleSpinBox*>(ui->tableWidget->cellWidget(row, 0));
        auto releasedDelaySpin = qobject_cast<NoWheelFocusDoubleSpinBox*>(ui->tableWidget->cellWidget(row, 1));
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
    NoWheelFocusDoubleSpinBox *pressedDelaySpin = new NoWheelFocusDoubleSpinBox(ui->tableWidget);
    pressedDelaySpin->setObjectName("dspinInTable");
    pressedDelaySpin->setDecimals(2);
    pressedDelaySpin->setMaximum(0.);
    pressedDelaySpin->setMaximum(999999999999.);
    pressedDelaySpin->setSingleStep(0.1);
    pressedDelaySpin->setAlignment(Qt::AlignCenter);
    pressedDelaySpin->setLocale(QLocale::English);
    if(index == 0)
    {
        pressedDelaySpin->setValue(0.);
        pressedDelaySpin->setEnabled(false);
    }
    else
    {
        auto previousSpin = qobject_cast<NoWheelFocusDoubleSpinBox*>(ui->tableWidget->cellWidget(index-1,0));
        if(previousSpin!= nullptr)
            pressedDelaySpin->setValue(previousSpin->value()+0.1);
    }

    NoWheelFocusDoubleSpinBox *releasedDelaySpin = new NoWheelFocusDoubleSpinBox(ui->tableWidget);
    releasedDelaySpin->setObjectName("dspinInTable");
    releasedDelaySpin->setDecimals(2);
    releasedDelaySpin->setMinimum(0.1);
    releasedDelaySpin->setMaximum(999999999999.);
    releasedDelaySpin->setSingleStep(0.1);
    releasedDelaySpin->setAlignment(Qt::AlignCenter);
    releasedDelaySpin->setLocale(QLocale::English);

    QPushButton *keysListButton = new QPushButton(ui->tableWidget);
    keysListButton->setFlat(true);
    keysListButton->setObjectName("buttonInTable");

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

void KeysSequenceSelectedEditDialog::accept()
{
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Keys sequence has no identity"),
                             tr("The keys sequence you tried to add has no identity, please define one."));
        return;
    }

    QList<int> delaysList;
    for(int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        auto pressedDelaySpin = qobject_cast<NoWheelFocusDoubleSpinBox*>(ui->tableWidget->cellWidget(row, 0));
        if(pressedDelaySpin == nullptr)
            continue;
        int delay = pressedDelaySpin->value()*1000;
        if(delaysList.contains(delay))
        {
            QMessageBox::warning(this, tr("Delay rehearsal"),
                                 tr("Delays before press must be unique, but some of them have the same value, please modify the entries."));
            return;
        }
        delaysList.append(delay);
    }

    QDialog::accept();
}
