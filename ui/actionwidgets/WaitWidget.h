#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"

class WaitWidget : public AbstractActionWidget
{
    Q_OBJECT
public:
    explicit WaitWidget(QWidget *parent = nullptr);
    void buildWidget() override;

signals:
};

#endif // WAITWIDGET_H
