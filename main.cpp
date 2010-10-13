#include <QtGui/QApplication>
#include <QSemaphore>
#include <QFile>
#include "mainwindow.h"
#include "faxtomailmodule.h"
#include "systemsource.h"
#include "mailhandle.h"
#include "configfileclass.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString test,test2;

    MainWindow w;
    SystemSource SysS;
    w.show();
    ConfigFileClass configFile;
    configFile.readFile(&SysS.ConfigFile,SysS.ConfigFileSep);
    //configFile.searchConfigInfo(QString("ip"),&test);
    //configFile.addConfigInfo(QString("ip"),QString("192.168.3.4"));
    //configFile.replaceConfigInfo(QString("ip"),QString("192.168.3.6"));
    //configFile.deleteConfigInfo(QString("ip"));
    //configFile.addMailInfo("002","dlbn@sina.com","123456");
    configFile.replaceMailInfo("002","dlbn@sina.com","121212");
    //configFile.searchMailInfo("001",&test,&test2);
    //configFile.deleteMailInfo("001");
    configFile.writeFile();
    //MailHandle MailHandleProcess(&SysS);
    //FaxToMailModule FaxToMailProcess(&SysS);
    return a.exec();
}
