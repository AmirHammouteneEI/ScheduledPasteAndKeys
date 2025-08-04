#ifndef SENTENCESTABLEWIDGET_H
#define SENTENCESTABLEWIDGET_H

#include "ui/CustomPrimaryWidgets.h"
#include "ui/editselectedactiondialogs/SentenceSelectedEditDialog.h"

class SentencesTableWidget : public NoFocusCellTableWidget
{
    Q_OBJECT
    SentenceSelectedEditDialog *m_sentenceEditDialog;
public:
    explicit SentencesTableWidget(QWidget *parent = nullptr);
    ~SentencesTableWidget() = default;
    void refresh();
    void selectSentenceFromIdentity(const QString & id);
    bool m_belongsToDataEditDialog = false;
public slots:
    void createSentenceReceived();
    void removeSentenceReceived();
    void editFromDialogReceived();
private slots:
    void editSentenceSelected(int row, int);
signals:
};

#endif // SENTENCESTABLEWIDGET_H
