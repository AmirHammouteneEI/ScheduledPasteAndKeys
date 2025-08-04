#include "CursorMovementsSelectedEditDialog.h"
#include "ui_CursorMovementsSelectedEditDialog.h"
#include "ui/CustomPrimaryWidgets.h"

#include <QPushButton>
#include <QMessageBox>

CursorMovementsSelectedEditDialog::CursorMovementsSelectedEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CursorMovementsSelectedEditDialog)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->resizeSection(0,220);
    ui->tableWidget->horizontalHeader()->resizeSection(4,40);
    ui->lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\w+"), this));
    m_getCursorCoordinatesWidget = new getCursorCoordinatesWidget(this);
    m_keySelectorDialog = new KeysSelectorDialog(ui->keysStrokeButton);

    connect(ui->addMovsButton, &QPushButton::released, this,&CursorMovementsSelectedEditDialog::addMovsRow);
    connect(ui->removeMovsButton, &QPushButton::released, this,&CursorMovementsSelectedEditDialog::removeLastMovsRow);
    connect(ui->keysStrokeButton, &QPushButton::released, m_keySelectorDialog, &KeysSelectorDialog::showDialog);
    connect(m_keySelectorDialog, &KeysSelectorDialog::sendKeysList, this,[=](QStringList keysList)
            {
                ui->keysStrokeButton->setProperty("keys", keysList);
                ui->keysStrokeButton->setText(keysList.join("+"));
            });
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
        if(widgetsList.size() < 4)
            break;

        auto delaySpin = qobject_cast<NoWheelFocusDoubleSpinBox*>(widgetsList.at(0));
        if(delaySpin == nullptr)
            break;
        delaySpin->setValue(el[0]/1000.);

        auto movingTimeSpin = qobject_cast<NoWheelFocusDoubleSpinBox*>(widgetsList.at(1));
        if(movingTimeSpin == nullptr)
            break;
        movingTimeSpin->setValue(el[1]/1000.);

        auto xCoordSpin = qobject_cast<NoWheelFocusSpinBox*>(widgetsList.at(2));
        if(xCoordSpin == nullptr)
            break;
        xCoordSpin->setValue(el[2]);

        auto yCoordSpin = qobject_cast<NoWheelFocusSpinBox*>(widgetsList.at(3));
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
        auto delaySpin = qobject_cast<NoWheelFocusDoubleSpinBox*>(ui->tableWidget->cellWidget(row, 0));
        auto movingTimeSpin = qobject_cast<NoWheelFocusDoubleSpinBox*>(ui->tableWidget->cellWidget(row, 1));
        auto xCoordSpin = qobject_cast<NoWheelFocusSpinBox*>(ui->tableWidget->cellWidget(row, 2));
        auto yCoordSpin = qobject_cast<NoWheelFocusSpinBox*>(ui->tableWidget->cellWidget(row, 3));
        if(delaySpin == nullptr || movingTimeSpin == nullptr || xCoordSpin == nullptr  || yCoordSpin == nullptr)
            break;
        MovementList movlist;
        movlist << delaySpin->value()*1000 << movingTimeSpin->value()*1000 << xCoordSpin->value() << yCoordSpin->value();
        returnedList.append(movlist);
    }
    return returnedList;
}

void CursorMovementsSelectedEditDialog::setOptionalKeysStroke(const QStringList &keysStroke)
{
    ui->keysStrokeButton->setProperty("keys", keysStroke);
    ui->keysStrokeButton->setText(keysStroke.join("+"));
}

QStringList CursorMovementsSelectedEditDialog::optionalKeysStroke()
{
    return ui->keysStrokeButton->property("keys").toStringList();
}

QList<QWidget *> CursorMovementsSelectedEditDialog::addMovsRow()
{
    int index = ui->tableWidget->rowCount();
    NoWheelFocusDoubleSpinBox *delaySpin = new NoWheelFocusDoubleSpinBox(this);
    delaySpin->setDecimals(2);
    delaySpin->setMinimum(0.);
    delaySpin->setMaximum(999999999999.);
    delaySpin->setSingleStep(0.1);
    delaySpin->setAlignment(Qt::AlignCenter);
    delaySpin->setLocale(QLocale::English);
    delaySpin->setValue(0.);
    if(index == 0)
        delaySpin->setEnabled(false);

    NoWheelFocusDoubleSpinBox *movingTimeSpin = new NoWheelFocusDoubleSpinBox(this);
    movingTimeSpin->setDecimals(2);
    movingTimeSpin->setMinimum(0.1);
    movingTimeSpin->setMaximum(999999999999.);
    movingTimeSpin->setSingleStep(0.1);
    movingTimeSpin->setAlignment(Qt::AlignCenter);
    movingTimeSpin->setLocale(QLocale::English);
    movingTimeSpin->setValue(1.);

    NoWheelFocusSpinBox *xCoordSpin = new NoWheelFocusSpinBox(this);
    xCoordSpin->setMinimum(0);
    xCoordSpin->setMaximum(9999);//TODO check screens max width
    xCoordSpin->setSingleStep(1);
    xCoordSpin->setAlignment(Qt::AlignCenter);
    xCoordSpin->setLocale(QLocale::English);
    xCoordSpin->setValue(0);

    NoWheelFocusSpinBox *yCoordSpin = new NoWheelFocusSpinBox(this);
    yCoordSpin->setMinimum(0);
    yCoordSpin->setMaximum(9999);//TODO check screens max height
    yCoordSpin->setSingleStep(1);
    yCoordSpin->setAlignment(Qt::AlignCenter);
    yCoordSpin->setLocale(QLocale::English);
    yCoordSpin->setValue(0);

    QPushButton *captureCoordsButton = new QPushButton(this);
    captureCoordsButton->setFlat(true);
    captureCoordsButton->setIcon(QIcon(":/img/cursor-cross.png"));
    captureCoordsButton->setProperty("index",index);
    connect(captureCoordsButton,&QPushButton::released, m_getCursorCoordinatesWidget,&getCursorCoordinatesWidget::showWidget);
    connect(m_getCursorCoordinatesWidget,&getCursorCoordinatesWidget::sendCoordinates, this,&CursorMovementsSelectedEditDialog::coordinatesReceived);

    ui->tableWidget->insertRow(index);
    ui->tableWidget->setCellWidget(index,0,delaySpin);
    ui->tableWidget->setCellWidget(index,1,movingTimeSpin);
    ui->tableWidget->setCellWidget(index,2,xCoordSpin);
    ui->tableWidget->setCellWidget(index,3,yCoordSpin);
    ui->tableWidget->setCellWidget(index,4,captureCoordsButton);

    QList<QWidget*> returnedList;
    returnedList << delaySpin << movingTimeSpin << xCoordSpin << yCoordSpin << captureCoordsButton;

    return returnedList;
}

void CursorMovementsSelectedEditDialog::removeLastMovsRow()
{
    if(ui->tableWidget->rowCount() > 0)
        ui->tableWidget->removeRow(ui->tableWidget->rowCount()-1);
}

void CursorMovementsSelectedEditDialog::coordinatesReceived(int index, int x, int y)
{
    auto xCoordSpin = qobject_cast<NoWheelFocusSpinBox*>(ui->tableWidget->cellWidget(index, 2));
    auto yCoordSpin = qobject_cast<NoWheelFocusSpinBox*>(ui->tableWidget->cellWidget(index, 3));
    if(xCoordSpin == nullptr  || yCoordSpin == nullptr)
        return;
    xCoordSpin->setValue(x);
    yCoordSpin->setValue(y);
}

void CursorMovementsSelectedEditDialog::accept()
{
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Cursor movements set has no identity"),
                             tr("The cursor movement set you tried to add has no identity, please define one."));
        return;
    }
    QDialog::accept();
}
