#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globals.h"

#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QCursor>
#include <QSettings>
#include <QToolBar>
#include <QTabBar>
#include <QFile>
#include <QShortcut>
#include <QMessageBox>
#include <QDir>

bool G_Parameters::AutoScrollTask = true;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Scheduled PC Tasks v1.2.1");
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowContextHelpButtonHint
                   | Qt::WindowCloseButtonHint);

    // Will also be a system tray app
    m_sticon = new QSystemTrayIcon(QIcon(":/img/programIcon.png"),this);
    buildSystemTrayMenu();
    m_sticon->setToolTip("Scheduled PC Tasks");
    m_sticon->show();

    m_startupTasksDialog = new StartupTasksDialog(this);
    m_dataEditDialog = new DataEditDialog(this);
    buildToolBar();

    loadSettings();
    geometrySet();

    m_createLoadTaskDialog = new CreateLoadTaskDialog(this);

    ui->tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0,0);

    // Connect the "+" tab to load or create task dialog
    connect(ui->tabWidget, &QTabWidget::tabBarClicked, this, &MainWindow::taskTabPageClicked);

    ui->tabWidget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tabWidget->tabBar(), &QTabBar::customContextMenuRequested, this, &MainWindow::taskTabContextMenuRequest);

    setTheme();

    m_tasktabsManager = new TaskTabsManager(this);

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, m_tasktabsManager, &TaskTabsManager::onTabCloseRequest);
    connect(m_createLoadTaskDialog, &CreateLoadTaskDialog::requestOpenNewTab, m_tasktabsManager, &TaskTabsManager::onOpenNewTabRequest);
    connect(m_createLoadTaskDialog, &CreateLoadTaskDialog::requestRefreshTabs, m_tasktabsManager, &TaskTabsManager::onRefreshTabsRequest);
    connect(m_createLoadTaskDialog, &CreateLoadTaskDialog::taskfilePathChanged, m_tasktabsManager, &TaskTabsManager::onTaskfilePathChanged);
    connect(ui->pushButton, &QPushButton::released, m_createLoadTaskDialog, &CreateLoadTaskDialog::showDialog);

    // Global shortcut to stop all running tasks
    m_stopAllTasksShortcut = new QShortcut(QKeySequence("Ctrl+Alt+S"), this);
    connect(m_stopAllTasksShortcut, &QShortcut::activated, m_tasktabsManager, &TaskTabsManager::stopAllRunningTasksReceived);

    setWhatsThis(tr("This software allows you to automatically schedule the actions you would perform on your PC.\n\n"\
                    "Developed by Amir Hammoutene (contact@amirhammoutene.dev)\n"
                    "initial work on February 2024\n\n"
                    "version 1.2.1"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QTabWidget *MainWindow::getTabWidget()
{
    return ui->tabWidget;
}

void MainWindow::autoRun(const QString &filename, int delay)
{
    if(m_tasktabsManager == nullptr || delay<0)
        return;
    hide();
    auto taskid = m_tasktabsManager->onOpenNewTabRequest(QApplication::applicationDirPath()+"/"+G_Files::TasksFolder+filename+G_Files::TasksFileExtension);
    if(taskid > -1)
        m_tasktabsManager->scheduleTaskFromId(taskid, delay);
}

void MainWindow::buildSystemTrayMenu()
{
    m_stmenu = new QMenu(this);

    QAction* titleAction = new QAction(QIcon(":/img/programIcon.png"),"Scheduled PC Tasks",m_stmenu);
    QFont titleFont = titleAction->font();
    titleFont.setBold(true);
    titleAction->setFont(titleFont);
    connect(titleAction, SIGNAL(triggered()), this, SLOT(showWindow()));

    QAction* quitAction = new QAction(tr("Quit"),m_stmenu);
    connect(quitAction, &QAction::triggered, this, &MainWindow::quitApp);

    m_stmenu->addAction(titleAction);
    m_stmenu->addSeparator();
    m_stmenu->addAction(quitAction);
    m_sticon->setContextMenu(m_stmenu);

    connect(m_sticon, &QSystemTrayIcon::activated, this, &MainWindow::showWindow);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    // Shows a system tray message saying that app is still running only the first time we close window
    if(!m_stMessageShown)
    {
        m_sticon->showMessage(tr("Scheduled PC Tasks is still running..."),
                                 tr("You can open the window by a simple click on the system tray icon.\n"\
                                 "Right click on it to quit the application."),QIcon(":/img/programIcon.png"));
        m_stMessageShown = true;
    }
    event->ignore();
}

void MainWindow::geometrySet()
{
    auto scrsize = QGuiApplication::primaryScreen()->size();
    setGeometry(scrsize.width()-m_windowWidth-2,30,m_windowWidth,m_windowHeight);
}

void MainWindow::loadSettings()
{
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::SettingsFilePath, QSettings::IniFormat);
    m_windowWidth = settings.value("windowWidth", 510).toInt();
    m_windowHeight = settings.value("windowHeight", 800).toInt();

    m_windowWidth = m_windowWidth < 50 ? 510 : m_windowWidth;
    m_windowHeight = m_windowHeight < 50 ? 800 : m_windowHeight;

    m_currentThemeName = settings.value("style", "dark").toString();
    swapAutoscrollMode(settings.value("autoscroll", true).toBool());
}

void MainWindow::saveSettings()
{
    QSettings settings(QApplication::applicationDirPath()+"/"+G_Files::SettingsFilePath, QSettings::IniFormat);
    settings.setValue("windowWidth", width());
    settings.setValue("windowHeight", height());
    settings.setValue("style", m_currentThemeName);
    settings.setValue("autoscroll", G_Parameters::AutoScrollTask);
}

void MainWindow::buildToolBar()
{
    m_toolBar = new QToolBar("toolbar",this);
    m_toolBar->setMovable(false);
    m_toolBar->setFloatable(false);
    m_toolBar->setFixedHeight(30);

    QAction *startupTasksAction = new QAction(QIcon(":/img/startup.png"),"",this);
    startupTasksAction->setToolTip(tr("Edit tasks scheduled at system startup..."));
    connect(startupTasksAction, &QAction::triggered, m_startupTasksDialog, &StartupTasksDialog::showDialog);

    QWidget* spacer0 = new QWidget(m_toolBar);
    spacer0->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QAction *dataEditAction = new QAction("▤",this);
    QFont font = dataEditAction->font();
    font.setBold(true);
    font.setPointSize(21);
    dataEditAction->setFont(font);
    dataEditAction->setToolTip(tr("Edit data..."));
    connect(dataEditAction, &QAction::triggered, m_dataEditDialog, &DataEditDialog::showDialog);

    QWidget* spacer1 = new QWidget(m_toolBar);
    spacer1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_scrollAction = new QAction("",this);
    m_scrollAction->setCheckable(true);
    connect(m_scrollAction, &QAction::triggered, this, &MainWindow::swapAutoscrollMode);

    QWidget* spacer2 = new QWidget(m_toolBar);
    spacer2->setFixedWidth(20);
    spacer2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    m_lightThemeAction = new QAction(QIcon(":/img/lightTheme.png"),"",this);
    m_lightThemeAction->setObjectName("lightAction");
    m_lightThemeAction->setCheckable(true);
    m_lightThemeAction->setToolTip(tr("Click to switch to Light style"));
    connect(m_lightThemeAction, &QAction::triggered, this, &MainWindow::switchTheme);

    m_penombraThemeAction = new QAction(QIcon(":/img/penombraTheme.png"),"",this);
    m_penombraThemeAction->setObjectName("penombraAction");
    m_penombraThemeAction->setCheckable(true);
    m_penombraThemeAction->setToolTip(tr("Click to switch to Penombra style"));
    connect(m_penombraThemeAction, &QAction::triggered, this, &MainWindow::switchTheme);

    m_darkThemeAction = new QAction(QIcon(":/img/darkTheme.png"),"",this);
    m_darkThemeAction->setObjectName("darkAction");
    m_darkThemeAction->setCheckable(true);
    m_darkThemeAction->setToolTip(tr("Click to switch to Dark style"));
    connect(m_darkThemeAction, &QAction::triggered, this, &MainWindow::switchTheme);

    m_stayOnTopAction = new QAction(QIcon(":/img/unpinned.png"),"",this);
    m_stayOnTopAction->setCheckable(true);
    m_stayOnTopAction->setToolTip(tr("Click to pin the window so it will stay on top"));
    connect(m_stayOnTopAction, &QAction::triggered, this, &MainWindow::swapStayOnTop);

    m_toolBar->addAction(startupTasksAction);
    m_toolBar->addWidget(spacer0);
    m_toolBar->addAction(dataEditAction);
    m_toolBar->addWidget(spacer1);
    m_toolBar->addAction(m_scrollAction);
    m_toolBar->addWidget(spacer2);
    m_toolBar->addAction(m_lightThemeAction);
    m_toolBar->addAction(m_penombraThemeAction);
    m_toolBar->addAction(m_darkThemeAction);
    m_toolBar->addAction(m_stayOnTopAction);

    addToolBar(m_toolBar);
}

void MainWindow::setTheme()
{
    if(m_currentThemeName == "light")
        m_lightThemeAction->trigger();
    else if(m_currentThemeName == "penombra")
        m_penombraThemeAction->trigger();
    else
        m_darkThemeAction->trigger();
}

void MainWindow::quitApp()
{
    m_tasktabsManager->stopAllRunningTasksReceived();

    if(m_tasktabsManager->isAnyTaskModified())
    {
        QMessageBox::StandardButton response = QMessageBox::question(this,tr("Saving changes"),
          tr("Some Tasks have been modified since the last save of each, would you like to save all changes ?"),
          QMessageBox::StandardButtons(QMessageBox::YesToAll | QMessageBox::NoToAll | QMessageBox::Cancel), QMessageBox::StandardButton(QMessageBox::YesToAll));

        if(response == QMessageBox::Cancel)
            return;
        if(response == QMessageBox::YesToAll)
            m_tasktabsManager->saveAllTasks();
    }

    saveSettings();
    m_stMessageShown = true;
    QApplication::quit();
}

void MainWindow::swapStayOnTop(bool state)
{
    if(state)
    {
        setWindowFlag(Qt::WindowStaysOnTopHint);
        show();
        m_stayOnTopAction->setIcon(QIcon(":/img/pinned.png"));
        m_stayOnTopAction->setToolTip(tr("Click to unpin the window so it won't stay on top"));
        return;
    }
    setWindowFlag(Qt::WindowStaysOnTopHint, false);
    show();
    m_stayOnTopAction->setIcon(QIcon(":/img/unpinned.png"));
    m_stayOnTopAction->setToolTip(tr("Click to pin the window so it will stay on top"));
}

void MainWindow::swapAutoscrollMode(bool state)
{
    if(state)
    {
        G_Parameters::AutoScrollTask = true;
        m_scrollAction->setChecked(true);
        m_scrollAction->setIcon(QIcon(":/img/scroll.png"));
        m_scrollAction->setToolTip(tr("Uncheck to disable autoscrolling during task execution"));
        return;
    }

    G_Parameters::AutoScrollTask = false;
    m_scrollAction->setChecked(false);
    m_scrollAction->setIcon(QIcon(":/img/noscroll.png"));
    m_scrollAction->setToolTip(tr("Check to enable autoscrolling during task execution"));
}

void MainWindow::switchTheme()
{
    QObject* sender = QObject::sender();
    if(sender == nullptr)
        return;

    m_lightThemeAction->setChecked(false);
    m_penombraThemeAction->setChecked(false);
    m_darkThemeAction->setChecked(false);

    QString objName = sender->objectName();
    QString qssFileName;
    if(objName=="lightAction")
    {
        qssFileName = ":/style/light.qss";
        m_lightThemeAction->setChecked(true);
        m_currentThemeName = "light";
    }
    else if(objName=="penombraAction")
    {
        qssFileName = ":/style/penombra.qss";
        m_penombraThemeAction->setChecked(true);
        m_currentThemeName = "penombra";
    }
    else
    {
        qssFileName = ":/style/dark.qss";
        m_darkThemeAction->setChecked(true);
        m_currentThemeName = "dark";
    }

    QFile qss(qssFileName);
    if (qss.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
}

void MainWindow::taskTabPageClicked(int index)
{
    if(index == ui->tabWidget->count()-1) //the last tab should always be the "+" tab
        m_createLoadTaskDialog->showDialog();
}

void MainWindow::taskTabContextMenuRequest(QPoint point)
{
    int index = ui->tabWidget->tabBar()->tabAt(point);
    if(index < 0 || index == ui->tabWidget->count()-1)
        return;

    auto menu = new QMenu(ui->tabWidget->tabBar());
    auto renameAct = new QAction(tr("Rename..."),menu);
    auto closeAct = new QAction(tr("Close"), menu);
    menu->addAction(renameAct);
    menu->addAction(closeAct);
    connect(renameAct, &QAction::triggered, this, [=]()
        {
            QString taskName = ui->tabWidget->tabBar()->tabText(index);
            m_createLoadTaskDialog->renameFilename(taskName+".scht",m_tasktabsManager->m_taskFilePathsMap.value(m_tasktabsManager->getIdfromTaskName(taskName)));
        });
    connect(closeAct, &QAction::triggered, this, [=](){ m_tasktabsManager->onTabCloseRequest(index); });

    menu->popup(ui->tabWidget->tabBar()->mapToGlobal(point));
}

void MainWindow::showWindow(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Context)
    {
        m_sticon->contextMenu()->popup(QCursor::pos());
        return;
    }
    show();
    activateWindow();
}
