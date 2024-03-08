/***
 * Amir Hammmoutene (contact@amirhammoutene.dev)
 * March 2024
 * This program, made for Windows systems, can be used to schedule some action on users computer by automation.
 * Code is in C++ language, using Qt framework (version 6.4.1)
 * Open-source, under GNU general public license version 3
***/

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<ActionParameters>();
    a.setWindowIcon(QIcon(":/img/programIcon.png"));
    MainWindow w;
    w.showWindow();
    return a.exec();
}
