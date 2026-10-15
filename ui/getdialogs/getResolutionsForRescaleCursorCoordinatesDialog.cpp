#include "getResolutionsForRescaleCursorCoordinatesDialog.h"
#include "ui_getResolutionsForRescaleCursorCoordinatesDialog.h"
#include <QApplication>
#include <QScreen>

getResolutionsForRescaleCursorCoordinatesDialog::getResolutionsForRescaleCursorCoordinatesDialog(
    QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::getResolutionsForRescaleCursorCoordinatesDialog)
{
    ui->setupUi(this);
}

getResolutionsForRescaleCursorCoordinatesDialog::~getResolutionsForRescaleCursorCoordinatesDialog()
{
    delete ui;
}

void getResolutionsForRescaleCursorCoordinatesDialog::showDialog()
{
    ui->widthTargetspinBox->setValue(QApplication::primaryScreen()->geometry().width());
    ui->heightTargetspinBox->setValue(QApplication::primaryScreen()->geometry().height());
    exec();
}

float getResolutionsForRescaleCursorCoordinatesDialog::getWidthRatio() const
{
    float returnedVal = 0.;
    auto originalWidth = ui->widthOriginalspinBox->value();
    auto targetWidth = ui->widthTargetspinBox->value();

    if(originalWidth != 0)
        returnedVal = targetWidth / (float)originalWidth;

    return returnedVal;
}

float getResolutionsForRescaleCursorCoordinatesDialog::getHeightRatio() const
{
    float returnedVal = 0.;
    auto originalHeight = ui->heightOriginalspinBox->value();
    auto targetHeight = ui->heightTargetspinBox->value();

    if(originalHeight != 0)
        returnedVal = targetHeight / (float)originalHeight;

    return returnedVal;
}
