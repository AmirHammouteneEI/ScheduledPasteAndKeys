#ifndef SENTENCESTABLEWIDGET_H
#define SENTENCESTABLEWIDGET_H

#include <QTableWidget>

#include "ui/SentenceSelectedEditDialog.h"

enum class EditMode{
    Editable,
    SelectOnly
};

class SentencesTableWidget : public QTableWidget
{
    Q_OBJECT
    SentenceSelectedEditDialog *m_sentenceEditDialog;
public:
    explicit SentencesTableWidget(QWidget *parent = nullptr);
    EditMode m_editMode = EditMode::Editable;
    void refresh();
public slots:
    void createSentenceReceived();
    void removeSentenceReceived();
    void editFromDialogReceived();
private slots:
    void editSentenceSelected(int row, int);
signals:
};

#endif // SENTENCESTABLEWIDGET_H
