#ifndef SENTENCESELECTEDEDITDIALOG_H
#define SENTENCESELECTEDEDITDIALOG_H

#include <QDialog>

namespace Ui {
class SentenceSelectedEditDialog;
}

class SentenceSelectedEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SentenceSelectedEditDialog(QWidget *parent = nullptr);
    ~SentenceSelectedEditDialog();
    void setEditable(bool id);
    void setIdentity(const QString & id);
    QString identity();
    void setContent(const QString & content);
    QString content();

private:
    Ui::SentenceSelectedEditDialog *ui;
};

#endif // SENTENCESELECTEDEDITDIALOG_H
