#include "CreateSystemCommandActionDialog.h"
#include "ui_CreateSystemCommandActionDialog.h"
#include "globals.h"
#include <QMessageBox>
#include <QInputDialog>

CreateSystemCommandActionDialog::CreateSystemCommandActionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateSystemCommandActionDialog)
{
    ui->setupUi(this);
    ui->shutdownButton->setProperty("type", "shutdown");
    ui->restartButton->setProperty("type", "restart");
    ui->logoffButton->setProperty("type", "logoff");
    ui->sleepmodeButton->setProperty("type", "sleepmode");
    ui->changevolumeButton->setProperty("type", "changevolume");
    ui->changeaudiodeviceButton->setProperty("type", "changeaudiodevice");
    ui->killprocessButton->setProperty("type", "killprocess");
    ui->focuswindowButton->setProperty("type", "focuswindow");
    ui->createfolderButton->setProperty("type", "createfolder");
    ui->deletefolderButton->setProperty("type", "deletefolder");
    ui->createfileButton->setProperty("type", "createfile");
    ui->deletefileButton->setProperty("type", "deletefile");
    ui->screenshotButton->setProperty("type", "screenshot");
    ui->printscreenButton->setProperty("type", "printscreen");

    m_option1Button = new QPushButton(this);
    m_option2Button = new QPushButton(this);

    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem());
    ui->tableWidget->setCellWidget(0,1,m_option1Button);
    ui->tableWidget->setCellWidget(0,2,m_option2Button);
    ui->tableWidget->item(0,0)->setTextAlignment(Qt::AlignCenter);

    m_folderPathDialog = new getFolderPathDialog(this);

    connect(ui->shutdownButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->restartButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->logoffButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->sleepmodeButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->changevolumeButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->changeaudiodeviceButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->killprocessButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->focuswindowButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->createfolderButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->deletefolderButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->createfileButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->deletefileButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->screenshotButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->printscreenButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
}

CreateSystemCommandActionDialog::~CreateSystemCommandActionDialog()
{
    delete ui;
}

void CreateSystemCommandActionDialog::accept()
{
    if((m_option1Button->isEnabled() && m_option1Button->text().isEmpty())
        || (m_option2Button->isEnabled() && m_option2Button->text().isEmpty()))
    {
        QMessageBox::warning(this, tr("Empty options"), tr("Option 1 or Option 2 is empty but they are required.\nPlease provide all necessary information."));
        return;
    }

    emit sendSystemCommand(m_type,m_option1Button->text(),m_option2Button->text());
    QDialog::accept();
}

void CreateSystemCommandActionDialog::commandButtonPushed()
{
    auto commandButtonSender = qobject_cast<QPushButton*>(sender());
    if(commandButtonSender == nullptr)
        return;
    if(!commandButtonSender->property("type").isValid())
        return;

    m_type = commandButtonSender->property("type").toString();
    ui->tableWidget->item(0,0)->setText(commandButtonSender->text());
    m_option1Button->setEnabled(false);
    m_option2Button->setEnabled(false);
    m_option1Button->setText("");
    m_option2Button->setText("");
    m_option1Button->disconnect();
    m_option2Button->disconnect();

    activateButtons();
}

void CreateSystemCommandActionDialog::showDialog()
{
    show();
    ui->tableWidget->item(0,0)->setText("");
    m_option1Button->setText("");
    m_option1Button->setEnabled(false);
    m_option1Button->disconnect();
    m_option2Button->setText("");
    m_option2Button->setEnabled(false);
    m_option2Button->disconnect();

    // if shows from an existing System Command Widget
    auto mainButtonSender = qobject_cast<QPushButton*>(sender());
    if(mainButtonSender == nullptr)
        return;

    if(!mainButtonSender->property("type").isValid() || !mainButtonSender->property("param1").isValid() ||
        !mainButtonSender->property("param2").isValid() || !mainButtonSender->property("displayedStr").isValid() ||
        mainButtonSender->property("displayedStr").toString() == tr("ERROR"))
        return;

    m_type = mainButtonSender->property("type").toString();
    ui->tableWidget->item(0,0)->setText(mainButtonSender->property("displayedStr").toString());
    QString opt1str = mainButtonSender->property("param1").toString();
    QString opt2str = mainButtonSender->property("param2").toString();
    if(!opt1str.isEmpty())
    {
        m_option1Button->setEnabled(true);
        m_option1Button->setText(opt1str);
        m_option1Button->setToolTip(opt1str);
    }
    if(!opt2str.isEmpty())
    {
        m_option2Button->setEnabled(true);
        m_option2Button->setText(opt2str);
        m_option2Button->setToolTip(opt2str);
    }
    activateButtons();
}


void CreateSystemCommandActionDialog::activateButtons()
{
    //TODO : unlock options buttons for all implemented sysyem commands
    if(m_type == G_SystemCommands::CreateFolderType)
    {
        m_option1Button->setEnabled(true);
        m_option2Button->setEnabled(true);
        connect(m_option1Button,&QPushButton::released, m_folderPathDialog, &getFolderPathDialog::showDialog);
        connect(m_folderPathDialog, &getFolderPathDialog::sendDirectory,this,[=](QString dir)
                {
                    m_option1Button->setText(dir);
                    m_option1Button->setToolTip(dir);
                });

        connect(m_option2Button,&QPushButton::released, this, [=]()
                {
                    bool ok;
                    QString val = QInputDialog::getText(this,tr("Set the option text"),tr("Set the folder/file name for the operation you would like to proceed :"),QLineEdit::Normal,m_option2Button->text(),&ok);
                    if(!ok)
                        return;
                    m_option2Button->setText(val);
                    m_option2Button->setToolTip(val);
                });
    }
}
