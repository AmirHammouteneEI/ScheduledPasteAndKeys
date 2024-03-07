#ifndef PASTEWIDGET_H
#define PASTEWIDGET_H

#include "ui/actionwidgets/AbstractActionWidget.h"
#include "ui/SelectSentenceDialog.h"

class PasteWidget : public AbstractActionWidget
{
    Q_OBJECT
    SelectSentenceDialog *m_selectSentenceDialog;
    QPushButton *m_mainButton;
public:
    explicit PasteWidget(QWidget *parent = nullptr);
    ~PasteWidget() = default;
    void buildWidget() override;
private slots:
    void sentenceIdentityReceived(QString id);
signals:
};

#endif // PASTEWIDGET_H
