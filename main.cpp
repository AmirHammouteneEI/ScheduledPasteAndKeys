/***
 * Amir Hammmoutene (contact@amirhammoutene.dev)
 * March 2024
 * This program, made for Windows systems, can be used to schedule some action on users computer automatically.
 * Code is in C++ language, using Qt framework (version 6.7.1)
 * Open-source, under GNU general public license version 3
***/

#include "mainwindow.h"
#include "actions/ActionParameters.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Application as a Qt ui-framework object
    // Registering metatype to be used in signals/slots of Qt mecanism
    qRegisterMetaType<ActionParameters>();
    qRegisterMetaType<ReleaseDelayKeysPair>();
    qRegisterMetaType<PressedReleaseDelaysKeysMap>();
    qRegisterMetaType<MovementList>();
    qRegisterMetaType<CursorMovementsList>();

    a.setWindowIcon(QIcon(":/img/programIcon.png"));

    // Get local system language to apply translations is exists
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QApplication::applicationDirPath()+"/"+QString("ScheduledPCTasks_") + locale);
    QTranslator baseTranslator;
    baseTranslator.load(QApplication::applicationDirPath()+"/"+QString("qtbase_") + locale);
    a.installTranslator(&translator);
    a.installTranslator(&baseTranslator);

    // Main ui window
    MainWindow *w = MainWindow::getInstance();
    w->buildTaskTabsManager();

    // In case of program used with correct arguments, automatically run the task (usage : ScheduledPCTasks.exe "TaskFileName" <DelayInSeconds> <NumberOfLoops>)
    if(argc == 3 || argc == 4)
    {
        int delay = atoi(argv[2]);
        if(delay > 0)
        {
            if(argc == 4)
            {
                int timesToRun = atoi(argv[3]);
                if(timesToRun != 0)
                    w->autoRun(QString(argv[1]), delay,timesToRun);
                else
                    w->showWindow();
            }
            else
                w->autoRun(QString(argv[1]), delay);
        }
        else
            w->showWindow();
    }
    else
        w->showWindow();

    int execCode = a.exec();
    delete w;
    return execCode;
}
