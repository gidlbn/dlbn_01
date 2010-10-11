#include <QtGui/QApplication>
#include <QSemaphore>
#include <QFile>
#include "mainwindow.h"
#include "faxtomailmodule.h"
#include "systemsource.h"
#include "mailhandle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    SystemSource SysS;
    w.show();
    MailHandle MailHandleProcess(&SysS);
    //FaxToMailModule FaxToMailProcess(&SysS);
    return a.exec();
}
