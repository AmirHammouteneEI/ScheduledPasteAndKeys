#ifndef PASTEWIDGET_H
#define PASTEWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"
#include "ui/createactiondialogs/CreatePasteActionDialog.h"
#include "ui/CustomPrimaryWidgets.h"
#include <QDateTime>

class PasteWidget : public AbstractActionWidget
{
    Q_OBJECT
    CreatePasteActionDialog *m_createPasteActionDialog = nullptr;
    NoWheelFocusSpinBox *m_loopSpin = nullptr;
    void changedRunningState() override;
public:
    explicit PasteWidget(QWidget *parent = nullptr);
    ~PasteWidget() = default;
    void buildWidget() override;
private slots:
    void sentenceIdentityReceived(QString id); // not const & because it's a slot, will make copy evenif
    void timesToRunChanged(int times);
private:
    void refreshLoopsRemainingText(const QDateTime& departureDate);
signals:
};

#endif // PASTEWIDGET_H
