#ifndef SELECTSENTENCEDIALOG_H
#define SELECTSENTENCEDIALOG_H

#include <QDialog>

namespace Ui {
class SelectSentenceDialog;
}

class SelectSentenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectSentenceDialog(QWidget *parent = nullptr);
    ~SelectSentenceDialog();

private slots:
    void accept() override;
public slots:
    void showDialog();
signals:
    void sendSentenceIdentity(QString id);
private:
    Ui::SelectSentenceDialog *ui;
};

#endif // SELECTSENTENCEDIALOG_H
