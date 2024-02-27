#ifndef PASTEWIDGET_H
#define PASTEWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"

class PasteWidget : public AbstractActionWidget
{
    Q_OBJECT
public:
    explicit PasteWidget(QWidget *parent = nullptr);
    void setAction(AbstractAction *action) override;
    void buildWidget() override;
signals:
};

#endif // PASTEWIDGET_H
