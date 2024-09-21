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
    QApplication a(argc, argv);
    qRegisterMetaType<ActionParameters>();
    qRegisterMetaType<ReleaseDelayKeysPair>();
    qRegisterMetaType<PressedReleaseDelaysKeysMap>();
    qRegisterMetaType<MovementList>();
    qRegisterMetaType<CursorMovementsList>();

    a.setWindowIcon(QIcon(":/img/programIcon.png"));

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("ScheduledPCTasks_") + locale);
    QTranslator baseTranslator;
    baseTranslator.load(QString("qtbase_") + locale);
    a.installTranslator(&translator);
    a.installTranslator(&baseTranslator);

    MainWindow w;
    w.showWindow();
    return a.exec();
}
