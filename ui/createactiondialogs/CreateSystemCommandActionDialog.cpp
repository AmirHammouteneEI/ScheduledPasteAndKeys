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
    ui->killprocessButton->setProperty("type", "killprocess");
    ui->quitselfprogramButton->setProperty("type", "quitselfprogram");
    ui->createfolderButton->setProperty("type", "createfolder");
    ui->deletefolderButton->setProperty("type", "deletefolder");
    ui->createfileButton->setProperty("type", "createfile");
    ui->deletefileButton->setProperty("type", "deletefile");
    ui->openfileButton->setProperty("type", "openfile");
    ui->executeprogramButton->setProperty("type", "executeprogram");
    ui->openurlButton->setProperty("type", "openurl");
    ui->openfolderButton->setProperty("type", "openfolder");
    ui->copyfileButton->setProperty("type", "copyfile");
    ui->takescreenshotButton->setProperty("type", "screenshot");
    ui->changevolume->setProperty("type", "changevolume");

    m_option1Button = new QPushButton(this);
    m_option2Button = new QPushButton(this);

    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem());
    ui->tableWidget->setCellWidget(0,1,m_option1Button);
    ui->tableWidget->setCellWidget(0,2,m_option2Button);
    ui->tableWidget->item(0,0)->setTextAlignment(Qt::AlignCenter);

    m_programPathDialog = new getProgramPathDialog(this);
    m_filePathDialog = new getFilePathDialog(this);
    m_folderPathDialog = new getFolderPathDialog(this);
    m_savedFilePathDialog = new getFilePathDialog(this,true);
    m_imagePathDialog = new getImagePathDialog(this);
    m_autorenameDialog = new getAutoRenameOptionDialog(this);

    connect(ui->shutdownButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->restartButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->logoffButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->killprocessButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->quitselfprogramButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->createfolderButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->deletefolderButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->createfileButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->deletefileButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->openfileButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->executeprogramButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->openurlButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->openfolderButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->copyfileButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->takescreenshotButton,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
    connect(ui->changevolume,&QPushButton::released, this, &CreateSystemCommandActionDialog::commandButtonPushed);
}

CreateSystemCommandActionDialog::~CreateSystemCommandActionDialog()
{
    delete ui;
}

void CreateSystemCommandActionDialog::accept()
{
    if((m_option1Button->isEnabled() && m_option1Button->text().isEmpty())
        || ( m_option2Button->isEnabled() && m_option2Button->text().isEmpty()
            && (m_type != G_SystemCommands::ExecuteProgramType) ))
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
    m_option1Button->setToolTip("");
    m_option2Button->setToolTip("");
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
    if(m_type == G_SystemCommands::CreateFileType)
    {
        m_option1Button->setEnabled(true);
        m_option2Button->setEnabled(true);
        connect(m_option1Button,&QPushButton::released, m_folderPathDialog, &getFolderPathDialog::showDialog);
        connect(m_folderPathDialog, &getFolderPathDialog::sendDirectory,this,[&](const QString &dir)
                {
                    m_option1Button->setText(dir);
                    m_option1Button->setToolTip(dir);
                });

        connect(m_option2Button,&QPushButton::released, this, [&]()
                {
                    bool ok;
                    QString val = QInputDialog::getText(this,tr("Set the filename"),tr("Set the filename (existing or not) for the operation you would like to proceed :"),QLineEdit::Normal,m_option2Button->text(),&ok);
                    if(!ok)
                        return;
                    m_option2Button->setText(val);
                    m_option2Button->setToolTip(val);
                });
    }
    else if(m_type == G_SystemCommands::CreateFolderType || m_type == G_SystemCommands::DeleteFolderType || m_type == G_SystemCommands::OpenFolderType)
    {
        m_option1Button->setEnabled(true);
        connect(m_option1Button,&QPushButton::released, m_folderPathDialog, &getFolderPathDialog::showDialog);
        connect(m_folderPathDialog, &getFolderPathDialog::sendDirectory,this,[&](const QString & dir)
                {
                    m_option1Button->setText(dir);
                    m_option1Button->setToolTip(dir);
                });
    }
    else if(m_type == G_SystemCommands::KillProcessType)
    {
        m_option1Button->setEnabled(true);

        connect(m_option1Button,&QPushButton::released, this, [&]()
                {
                    bool ok;
                    QString val = QInputDialog::getText(this,tr("Set the process name"),tr("Set the process name (including .exe) you would like to schedule to kill :"),QLineEdit::Normal,m_option1Button->text(),&ok);
                    if(!ok)
                        return;
                    m_option1Button->setText(val);
                });
    }
    else if(m_type == G_SystemCommands::OpenFileType || m_type == G_SystemCommands::DeleteFileType)
    {
        m_option1Button->setEnabled(true);
        connect(m_option1Button,&QPushButton::released, m_filePathDialog, &getFilePathDialog::showDialog);
        connect(m_filePathDialog, &getFilePathDialog::sendFile,this,[&](const QString & dir)
                {
                    m_option1Button->setText(dir);
                    m_option1Button->setToolTip(dir);
                });
    }
    if(m_type == G_SystemCommands::ExecuteProgramType)
    {
        m_option1Button->setEnabled(true);
        m_option2Button->setEnabled(true);
        connect(m_option1Button,&QPushButton::released, m_programPathDialog, &getProgramPathDialog::showDialog);
        connect(m_programPathDialog, &getProgramPathDialog::sendProgram,this,[&](const QString & dir)
                {
                    m_option1Button->setText(dir);
                    m_option1Button->setToolTip(dir);
                });

        connect(m_option2Button,&QPushButton::released, this, [&]()
                {
                    bool ok;
                    QString val = QInputDialog::getText(this,tr("Set optional arguments"),tr("If needed, set arguments for the execution of the program you would like to run :"),QLineEdit::Normal,m_option2Button->text(),&ok);
                    if(!ok)
                        return;
                    m_option2Button->setText(val);
                    m_option2Button->setToolTip(val);
                });
    }
    else if(m_type == G_SystemCommands::OpenUrlType)
    {
        m_option1Button->setEnabled(true);

        connect(m_option1Button,&QPushButton::released, this, [&]()
                {
                    bool ok;
                    QString val = QInputDialog::getText(this,tr("Set the URL"),tr("Set the URL you would like to open :"),QLineEdit::Normal,m_option1Button->text(),&ok);
                    if(!ok)
                        return;
                    m_option1Button->setText(val);
                    m_option1Button->setToolTip(val);
                });
    }
    if(m_type == G_SystemCommands::CopyFileType)
    {
        m_option1Button->setEnabled(true);
        m_option2Button->setEnabled(true);
        connect(m_option1Button,&QPushButton::released, m_filePathDialog, &getFilePathDialog::showDialog);
        connect(m_filePathDialog, &getFilePathDialog::sendFile,this,[&](const QString &dir)
                {
                    m_option1Button->setText(dir);
                    m_option1Button->setToolTip(dir);
                });

        connect(m_option2Button,&QPushButton::released, m_savedFilePathDialog, &getFilePathDialog::showDialog);
        connect(m_savedFilePathDialog, &getFilePathDialog::sendFile,this,[&](const QString &dir)
                {
                    m_option2Button->setText(dir);
                    m_option2Button->setToolTip(dir);
                });
    }
    else if(m_type == G_SystemCommands::TakeScreenshotType)
    {
        m_option1Button->setEnabled(true);
        m_option2Button->setEnabled(true);
        connect(m_option1Button,&QPushButton::released, m_imagePathDialog, &getImagePathDialog::showDialog);
        connect(m_imagePathDialog, &getImagePathDialog::sendImage,this,[&](const QString & dir)
                {
                    m_option1Button->setText(dir);
                    m_option1Button->setToolTip(dir);
                });

        connect(m_option2Button,&QPushButton::released, m_autorenameDialog, &getAutoRenameOptionDialog::showDialog);
        connect(m_autorenameDialog, &getAutoRenameOptionDialog::sendAutorename,this,[&](bool autorename)
                {
                    if(autorename)
                        {
                            m_option2Button->setText("Auto-rename if exists");
                            m_option2Button->setToolTip(tr("If the file already exists, a new file will be created with another filename."));
                        }
                    else
                        {
                            m_option2Button->setText("Erase if exists");
                            m_option2Button->setToolTip(tr("If the file already exists, it will be erased to save the new file."));
                        }
                });
    }
    else if(m_type == G_SystemCommands::ChangeAudioVolumeType)
    {
        m_option1Button->setEnabled(true);
        connect(m_option1Button,&QPushButton::released, this, [&]()
                {
                    bool ok;
                    int val = QInputDialog::getInt(this,tr("Set the new volume"),tr("Set the new audio volume :"),m_option1Button->text() == "" ? 50 : m_option1Button->text().toInt(),0,100,1,&ok);
                    if(!ok)
                        return;
                    m_option1Button->setText(QString::number(val));
                });
    }
    // if QuitSelfProgramType,ShutDownType,RestartType,LogOffType no need of any option
}
