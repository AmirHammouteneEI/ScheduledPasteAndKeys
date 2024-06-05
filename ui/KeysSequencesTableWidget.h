#ifndef KEYSSEQUENCESTABLEWIDGET_H
#define KEYSSEQUENCESTABLEWIDGET_H

#include <QTableWidget>

#include "ui/KeysSequenceSelectedEditDialog.h"

class KeysSequencesTableWidget : public QTableWidget
{
    Q_OBJECT
    KeysSequenceSelectedEditDialog *m_keysSequenceEditDialog;
public:
    explicit KeysSequencesTableWidget(QWidget *parent = nullptr);
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
