#ifndef KEYSSEQUENCEWIDGET_H
#define KEYSSEQUENCEWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"
#include "ui/createactiondialog/CreateKeysSequenceActionDialog.h"
#include <QDateTime>

class KeysSequenceWidget : public AbstractActionWidget
{
    Q_OBJECT
    CreateKeysSequenceActionDialog *m_createKeysSeqActionDialog;
    NoWheelFocusSpinBox *m_loopSpin;
    void changedRunningState() override;
public:
    explicit KeysSequenceWidget(QWidget *parent = nullptr);
    ~KeysSequenceWidget() = default;
    void buildWidget() override;
private slots:
    void keysSeqIdentityReceived(QString id); // not const & because it's a slot, will make copy evenif
    void timesToRunChanged(int times);
private:
    void refreshLoopsRemainingText(const QDateTime& departureDate);
signals:
};

#endif // KEYSSEQUENCEWIDGET_H
