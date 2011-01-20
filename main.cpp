#include <QtGui/QApplication>
#include <QSemaphore>
#include <QFile>
#include "mainwindow.h"
#include "faxtomailmodule.h"
#include "systemsource.h"
#include "mailhandle.h"
#include "configfileclass.h"
#include "comhandle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString test,test2;

    MainWindow w;
    SystemSource SysS;
    w.show();
    //MailHandle MailHandleProcess(&SysS);
    //FaxToMailModule FaxToMailProcess(&SysS);
    //ComHandle comHandle(&SysS);

    return a.exec();
}
