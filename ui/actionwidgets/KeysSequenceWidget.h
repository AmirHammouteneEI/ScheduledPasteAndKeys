#ifndef KEYSSEQUENCEWIDGET_H
#define KEYSSEQUENCEWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"
#include "ui/createactiondialog/CreateKeysSequenceActionDialog.h"

class KeysSequenceWidget : public AbstractActionWidget
{
    Q_OBJECT
    CreateKeysSequenceActionDialog *m_createKeysSeqActionDialog;
public:
    explicit KeysSequenceWidget(QWidget *parent = nullptr);
    ~KeysSequenceWidget() = default;
    void buildWidget() override;
private slots:
    void keysSeqIdentityReceived(QString id);
private:
    void refreshLoopsRemainingText();
signals:
};

#endif // KEYSSEQUENCEWIDGET_H
