#ifndef GETQKEYSEQUENCEDIALOG_H
#define GETQKEYSEQUENCEDIALOG_H

#include <QDialog>
#include <QKeySequence>

namespace Ui {
class getQKeySequenceDialog;
}

class getQKeySequenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getQKeySequenceDialog(const QKeySequence &currentSeq = QKeySequence(), QWidget *parent = nullptr);
    ~getQKeySequenceDialog();

    QKeySequence keySequence() const;

private:
    Ui::getQKeySequenceDialog *ui;
};

#endif // GETQKEYSEQUENCEDIALOG_H
