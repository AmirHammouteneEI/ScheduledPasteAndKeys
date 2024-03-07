#include "SentenceSelectedEditDialog.h"
#include "ui_SentenceSelectedEditDialog.h"

#include <QPushButton>
#include <QRegularExpressionValidator>

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

void SentenceSelectedEditDialog::setEditable(bool id, bool content)
{
    ui->lineEdit->setReadOnly(!id);
    ui->plainTextEdit->setReadOnly(!content);
    if(!id && !content)
        ui->buttonBox->button(QDialogButtonBox::Save)->hide();
    else
        ui->buttonBox->button(QDialogButtonBox::Save)->show();
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
