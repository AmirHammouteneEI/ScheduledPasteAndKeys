#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QCursor>
#include <QSettings>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Schedule Paste & Keys v1.0");
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowContextHelpButtonHint
                   | Qt::WindowCloseButtonHint);
    // Will also be a system tray app
    m_sticon = new QSystemTrayIcon(QIcon(":/img/programIcon.png"),this);
    buildSystemTrayMenu();
    m_sticon->show();

    loadSettings();
    geometrySet();

    buildToolBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildSystemTrayMenu()
{
    QMenu *stmenu = new QMenu(this);

    QAction* titleAction = new QAction(QIcon(":/img/programIcon.png"),"Scheduled Paste & Keys",stmenu);
    QFont titleFont = titleAction->font();
    titleFont.setBold(true);
    titleAction->setFont(titleFont);
    connect(titleAction, SIGNAL(triggered()), this, SLOT(showWindow()));

    QAction* quitAction = new QAction(tr("Quit"),stmenu);
    connect(quitAction, &QAction::triggered, this, &MainWindow::quitApp);

    stmenu->addAction(titleAction);
    stmenu->addSeparator();
    stmenu->addAction(quitAction);
    m_sticon->setContextMenu(stmenu);

    connect(m_sticon, &QSystemTrayIcon::activated, this, &MainWindow::showWindow);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void MainWindow::geometrySet()
{
    auto scrsize = QGuiApplication::primaryScreen()->size();
    setGeometry(scrsize.width()-m_windowWidth-2,30,m_windowWidth,m_windowHeight);
}

void MainWindow::loadSettings()
{
    QSettings settings("programSettings.ini", QSettings::IniFormat);
    m_windowWidth = settings.value("windowWidth", 300).toInt();
    m_windowHeight = settings.value("windowHeight", 900).toInt();

    m_windowWidth = m_windowWidth < 50 ? 300 : m_windowWidth;
    m_windowHeight = m_windowHeight < 50 ? 900 : m_windowHeight;
}

void MainWindow::saveSettings()
{
    QSettings settings("programSettings.ini", QSettings::IniFormat);
    settings.setValue("windowWidth", width());
    settings.setValue("windowHeight", height());
}

void MainWindow::buildToolBar()
{
    QToolBar* toolBar = new QToolBar("toolbar",this);
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setFixedHeight(30);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    m_stayOnTopAction = new QAction(QIcon(":/img/unpinned.png"),"",this);
    m_stayOnTopAction->setCheckable(true);
    m_stayOnTopAction->setToolTip(tr("Click to pin the window so it will stay on top"));
    connect(m_stayOnTopAction, &QAction::triggered, this, &MainWindow::swapStayOnTop);

    toolBar->addAction(m_stayOnTopAction);

    addToolBar(toolBar);
}

void MainWindow::quitApp()
{
    saveSettings();
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
