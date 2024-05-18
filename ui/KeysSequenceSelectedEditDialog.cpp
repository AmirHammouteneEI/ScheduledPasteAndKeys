#include "KeysSequenceSelectedEditDialog.h"
#include "ui_KeysSequenceSelectedEditDialog.h"

KeysSequenceSelectedEditDialog::KeysSequenceSelectedEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::KeysSequenceSelectedEditDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\w+"), this));
}

KeysSequenceSelectedEditDialog::~KeysSequenceSelectedEditDialog()
{
    delete ui;
}

void KeysSequenceSelectedEditDialog::setEditable(bool id)
{
    ui->lineEdit->setReadOnly(!id);
}

void KeysSequenceSelectedEditDialog::setIdentity(const QString &id)
{
    ui->lineEdit->setText(id);
}

QString KeysSequenceSelectedEditDialog::identity()
{
    return ui->lineEdit->text();
}

void KeysSequenceSelectedEditDialog::setTableKeysSequence(const QMap<double, ReleaseDelayKeysPair> &tableContent)
{
    //TODO next step
}

QMap<double, ReleaseDelayKeysPair> KeysSequenceSelectedEditDialog::tableKeysSequence()
{
    //TODO next step
    return QMap<double, ReleaseDelayKeysPair>();
}
