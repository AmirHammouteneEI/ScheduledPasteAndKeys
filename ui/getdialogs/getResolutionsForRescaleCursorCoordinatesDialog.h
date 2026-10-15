#ifndef GETRESOLUTIONSFORRESCALECURSORCOORDINATESDIALOG_H
#define GETRESOLUTIONSFORRESCALECURSORCOORDINATESDIALOG_H

#include <QDialog>

namespace Ui {
class getResolutionsForRescaleCursorCoordinatesDialog;
}

class getResolutionsForRescaleCursorCoordinatesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getResolutionsForRescaleCursorCoordinatesDialog(QWidget *parent = nullptr);
    ~getResolutionsForRescaleCursorCoordinatesDialog();
    float getWidthRatio() const;
    float getHeightRatio() const;

public slots:
    void showDialog();

private:
    Ui::getResolutionsForRescaleCursorCoordinatesDialog *ui;
};

#endif // GETRESOLUTIONSFORRESCALECURSORCOORDINATESDIALOG_H
