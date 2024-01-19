#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/img/programIcon.png"));
    MainWindow w;
    w.showWindow();
    return a.exec();
}
