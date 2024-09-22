#include "CursorMovementsSelectedEditDialog.h"
#include "ui_CursorMovementsSelectedEditDialog.h"

#include <QDoubleSpinBox>
#include <QSpinBox>

CursorMovementsSelectedEditDialog::CursorMovementsSelectedEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CursorMovementsSelectedEditDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\w+"), this));
    connect(ui->addMovsButton, &QPushButton::released, this,&CursorMovementsSelectedEditDialog::addMovsRow);
    connect(ui->removeMovsButton, &QPushButton::released, this,&CursorMovementsSelectedEditDialog::removeLastMovsRow);

}

CursorMovementsSelectedEditDialog::~CursorMovementsSelectedEditDialog()
{
    delete ui;
}

void CursorMovementsSelectedEditDialog::setEditable(bool id)
{
    ui->lineEdit->setReadOnly(!id);
}

void CursorMovementsSelectedEditDialog::setIdentity(const QString &id)
{
    ui->lineEdit->setText(id);
}

QString CursorMovementsSelectedEditDialog::identity()
{
    return ui->lineEdit->text();
}

void CursorMovementsSelectedEditDialog::setTableCursorMovements(const CursorMovementsList &tableContent)
{

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    for(auto el : tableContent)
    {
        if(el.size() < 4)
            continue;

        QList<QWidget*> widgetsList = addMovsRow();
        if(widgetsList.size() != 4)
            break;

        auto delaySpin = qobject_cast<QDoubleSpinBox*>(widgetsList.at(0));
        if(delaySpin == nullptr)
            break;
        delaySpin->setValue(el[0]/1000.);

        auto movingTimeSpin = qobject_cast<QDoubleSpinBox*>(widgetsList.at(1));
        if(movingTimeSpin == nullptr)
            break;
        movingTimeSpin->setValue(el[1]/1000.);

        auto xCoordSpin = qobject_cast<QSpinBox*>(widgetsList.at(2));
        if(xCoordSpin == nullptr)
            break;
        xCoordSpin->setValue(el[2]);

        auto yCoordSpin = qobject_cast<QSpinBox*>(widgetsList.at(3));
        if(yCoordSpin == nullptr)
            break;
        yCoordSpin->setValue(el[3]);
    }
}

CursorMovementsList CursorMovementsSelectedEditDialog::tableCursorMovements()
{
    CursorMovementsList returnedList;

    for(int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        auto delaySpin = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(row, 0));
        auto movingTimeSpin = qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(row, 1));
        auto xCoordSpin = qobject_cast<QSpinBox*>(ui->tableWidget->cellWidget(row, 2));
        auto yCoordSpin = qobject_cast<QSpinBox*>(ui->tableWidget->cellWidget(row, 3));
        if(delaySpin == nullptr || movingTimeSpin == nullptr || xCoordSpin == nullptr  || yCoordSpin == nullptr)
            break;
        MovementList movlist;
        movlist << delaySpin->value()*1000 << movingTimeSpin->value()*1000 << xCoordSpin->value() << yCoordSpin->value();
        returnedList.append(movlist);
    }
    return returnedList;
}

QList<QWidget *> CursorMovementsSelectedEditDialog::addMovsRow()
{
    int index = ui->tableWidget->rowCount();
    QDoubleSpinBox *delaySpin = new QDoubleSpinBox(this);
    delaySpin->setDecimals(2);
    delaySpin->setMinimum(0.);
    delaySpin->setMaximum(999999999999.);
    delaySpin->setSingleStep(0.1);
    delaySpin->setAlignment(Qt::AlignCenter);
    delaySpin->setLocale(QLocale::English);
    delaySpin->setValue(0.);
    if(index == 0)
        delaySpin->setEnabled(false);

    QDoubleSpinBox *movingTimeSpin = new QDoubleSpinBox(this);
    movingTimeSpin->setDecimals(2);
    movingTimeSpin->setMinimum(0.1);
    movingTimeSpin->setMaximum(999999999999.);
    movingTimeSpin->setSingleStep(0.1);
    movingTimeSpin->setAlignment(Qt::AlignCenter);
    movingTimeSpin->setLocale(QLocale::English);
    movingTimeSpin->setValue(1.);

    QSpinBox *xCoordSpin = new QSpinBox(this);
    xCoordSpin->setMinimum(0);
    xCoordSpin->setMaximum(9999);//TODO check screens max width
    xCoordSpin->setSingleStep(1);
    xCoordSpin->setAlignment(Qt::AlignCenter);
    xCoordSpin->setLocale(QLocale::English);
    xCoordSpin->setValue(0);

    QSpinBox *yCoordSpin = new QSpinBox(this);
    yCoordSpin->setMinimum(0);
    yCoordSpin->setMaximum(9999);//TODO check screens max height
    yCoordSpin->setSingleStep(1);
    yCoordSpin->setAlignment(Qt::AlignCenter);
    yCoordSpin->setLocale(QLocale::English);
    yCoordSpin->setValue(0);

    ui->tableWidget->insertRow(index);
    ui->tableWidget->setCellWidget(index,0,delaySpin);
    ui->tableWidget->setCellWidget(index,1,movingTimeSpin);
    ui->tableWidget->setCellWidget(index,2,xCoordSpin);
    ui->tableWidget->setCellWidget(index,3,yCoordSpin);

    QList<QWidget*> returnedList;
    returnedList << delaySpin << movingTimeSpin << xCoordSpin << yCoordSpin;

    return returnedList;
}

void CursorMovementsSelectedEditDialog::removeLastMovsRow()
{
    if(ui->tableWidget->rowCount() > 0)
        ui->tableWidget->removeRow(ui->tableWidget->rowCount()-1);
}
