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
    ConfigFileClass configFile;
    configFile.readFile(&SysS.ConfigFile,SysS.ConfigFileSep);
    //configFile.searchConfigInfo(QString("ip"),&test);
    //configFile.addConfigInfo(QString("ip"),QString("192.168.3.4"));
    //configFile.replaceConfigInfo(QString("ip"),QString("192.168.3.6"));
    //configFile.deleteConfigInfo(QString("ip"));
    //configFile.addMailInfo("002","dlbn@sina.com","123456");
    //configFile.replaceMailInfo("002","dlbn@sina.com","121212");
    //configFile.searchMailInfo("001",&test,&test2);
    //configFile.deleteMailInfo("001");
    configFile.writeFile();
    //MailHandle MailHandleProcess(&SysS);
    //FaxToMailModule FaxToMailProcess(&SysS);
    /*
    int tmpI1=0;
    QString srcS,testS,targetS;
    srcS="123451Content-Type: text\r\nsdfasdfdf\r\n\r\nMTIzNDU=\r\n\r\n123451Content-Type: text\r\nsdfasdfdf\r\n\r\nY2hhbmdlaWQ6MTY1Mw==\r\n\r\n";
    while(tmpI1>=0)
    {
        testS="Content-Type: text";
        tmpI1=srcS.indexOf(testS);
        //qDebug()<<tmpI1;
        if (tmpI1>0)
        {
            srcS=srcS.right(srcS.count()-tmpI1-testS.count());
            //qDebug()<<tmpS1;
            testS="\r\n\r\n";
            tmpI1=srcS.indexOf(testS);
            srcS=srcS.right(srcS.count()-tmpI1-testS.count());
            tmpI1=srcS.indexOf(testS);
            if (tmpI1>0)
            {
                targetS=srcS.left(tmpI1);
                qDebug()<<targetS;
                QByteArray tmpB =targetS.toAscii();
                targetS = QString(QByteArray::fromBase64(tmpB));
                qDebug()<<targetS;
                srcS=srcS.right(srcS.count()-tmpI1-testS.count());
            }

        }
    }
    */

    ComHandle comHandle(&SysS);

    return a.exec();
}
