#ifndef PASTEWIDGET_H
#define PASTEWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"
#include "ui/createactiondialog/CreatePasteActionDialog.h"

class PasteWidget : public AbstractActionWidget
{
    Q_OBJECT
    CreatePasteActionDialog *m_createPasteActionDialog;
public:
    explicit PasteWidget(QWidget *parent = nullptr);
    ~PasteWidget() = default;
    void buildWidget() override;
private slots:
    void sentenceIdentityReceived(QString id); // not const & because it's a slot, will make copy evenif
private:
    void changeContentInfo(const QString & content);
signals:
};

#endif // PASTEWIDGET_H
