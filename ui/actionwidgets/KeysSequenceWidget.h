#ifndef KEYSSEQUENCEWIDGET_H
#define KEYSSEQUENCEWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"
#include "ui/createactiondialogs/CreateKeysSequenceActionDialog.h"
#include "ui/CustomPrimaryWidgets.h"
#include <QDateTime>

class KeysSequenceWidget : public AbstractActionWidget
{
    Q_OBJECT
    CreateKeysSequenceActionDialog *m_createKeysSeqActionDialog = nullptr;
    NoWheelFocusSpinBox *m_loopSpin = nullptr;
    void changedRunningState() override;
public:
    explicit KeysSequenceWidget(QWidget *parent = nullptr);
    ~KeysSequenceWidget() = default;
    void buildWidget() override;
public slots:
    void keysSeqIdentityReceived(QString id); // not const & because it's a slot, will make copy evenif
private slots:
    void timesToRunChanged(int times);
private:
    void refreshLoopsRemainingText(const QDateTime& departureDate);
signals:
};

#endif // KEYSSEQUENCEWIDGET_H
