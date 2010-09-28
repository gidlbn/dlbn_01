#include <QtGui/QApplication>
#include "mainwindow.h"
#include "faxtomailmodule.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    FaxToMailModule FaxToMailProcess;
    return a.exec();
}
