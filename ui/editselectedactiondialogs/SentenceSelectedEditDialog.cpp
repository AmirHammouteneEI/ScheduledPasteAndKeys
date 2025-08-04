#include "SentenceSelectedEditDialog.h"
#include "ui_SentenceSelectedEditDialog.h"

#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QMessageBox>

SentenceSelectedEditDialog::SentenceSelectedEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SentenceSelectedEditDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\w+"), this));
}

SentenceSelectedEditDialog::~SentenceSelectedEditDialog()
{
    delete ui;
}

void SentenceSelectedEditDialog::setEditable(bool id)
{
    ui->lineEdit->setReadOnly(!id);
}

void SentenceSelectedEditDialog::setIdentity(const QString &id)
{
    ui->lineEdit->setText(id);
}

QString SentenceSelectedEditDialog::identity()
{
    return ui->lineEdit->text();
}

void SentenceSelectedEditDialog::setContent(const QString &content)
{
    ui->plainTextEdit->setPlainText(content);
}

QString SentenceSelectedEditDialog::content()
{
    return ui->plainTextEdit->toPlainText();
}

void SentenceSelectedEditDialog::accept()
{
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Sentence has no identity"),
                             tr("The sentence you tried to add has no identity, please define one."));
        return;
    }
    QDialog::accept();
}
