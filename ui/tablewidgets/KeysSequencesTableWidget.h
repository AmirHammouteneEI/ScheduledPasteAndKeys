#ifndef KEYSSEQUENCESTABLEWIDGET_H
#define KEYSSEQUENCESTABLEWIDGET_H

#include "ui/CustomPrimaryWidgets.h"
#include "ui/editselectedactiondialogs/KeysSequenceSelectedEditDialog.h"

class KeysSequencesTableWidget : public NoFocusCellTableWidget
{
    Q_OBJECT
    KeysSequenceSelectedEditDialog *m_keysSequenceEditDialog = nullptr;
public:
    explicit KeysSequencesTableWidget(QWidget *parent = nullptr);
    ~KeysSequencesTableWidget() = default;
    void refresh();
    void selectKeysSequenceFromIdentity(const QString & id);
    bool m_belongsToDataEditDialog = false;
public slots:
    void createKeysSequenceReceived();
    void removeKeysSequenceReceived();
    void editFromDialogReceived();
private slots:
    void editKeysSequenceSelected(int row, int);
signals:
};

#endif // KEYSSEQUENCESTABLEWIDGET_H
