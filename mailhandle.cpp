#include "mailhandle.h"
#include <QFile>
#include <QDateTime>

MailHandle::MailHandle(SystemSource *SysS,QWidget *parent) :
    QWidget(parent)
{
    this->SysS=SysS;
    connect(&this->MailHandleTimer, SIGNAL(timeout()), this, SLOT(Process()));
    this->MailHandleTimer.setSingleShot(true);
    this->MailHandleTimer.start(2*1000);
    this->Pop3Server="pop.sogou.com";
    this->Pop3Password="so123456";
    this->Pop3User="fax_test";
}

void MailHandle::Process()
{
    int number,size,linenumber,ret;
    QString top,msgid,msg;
    QPair<QString,int> uIdList;
    QDateTime datetime;
    ret=this->pop3client.Connect(this->Pop3Server);
    ret=this->pop3client.Login(this->Pop3User,this->Pop3Password);
    ret=this->pop3client.GetMailboxStatus(number,size);
    qDebug() << "mail number="<<number<<"size="<<size;
    if (number>0)
    {
        QString top,msgid,msg;
        QPair<QString,int> uIdList;
        qint8 tmp;
        //this->pop3client.GetMsgList("1",uIdList);
        //qDebug() <<"hehe "<< uIdList.first<<" "<<uIdList.second;
        for (tmp=1;tmp<=number;tmp++)
        {
            this->pop3client.GetMessage(QString::number(tmp),msg);
            qDebug() <<"=====mesginfo====\n"<<msg<<"\n\n\n";
            QByteArray msgarray = msg.toLatin1();
            char *c_msg = msgarray.data();
            QFile MailFile;
            MailFile.setFileName(this->SysS->MailRecordFolder.absolutePath()+"/"+datetime.currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"));
            MailFile.open(QIODevice::ReadWrite|QIODevice::Text);
            MailFile.write(c_msg);
            MailFile.close();
            readMail(msg);
        }

        this->pop3client.Quit();
    }


    //this->MailHandleTimer.start(5*1000);
}

int MailHandle::readMail(QString srcS)
{
    int ret=0;
    QString testS,targetS;
    while(ret>=0)
    {
        testS="Content-Type: text";
        ret=srcS.indexOf(testS);
        qDebug()<<"Check Content-Type: text"<<ret;
        if (ret>=0)
        {
            srcS=srcS.right(srcS.count()-ret-testS.count());
            //qDebug()<<tmpS1;
            testS="\r\n\r\n";
            ret=srcS.indexOf(testS);
            qDebug()<<"Check \\r\\n\\r\\n text begin"<<ret;
            srcS=srcS.right(srcS.count()-ret-testS.count());
            ret=srcS.indexOf(testS);
            qDebug()<<"Check \\r\\n\\r\\n text end"<<ret;
            if (ret>=0)
            {
                targetS=srcS.left(ret);
                qDebug()<<targetS;
                QByteArray tmpB =targetS.toAscii();
                targetS = QString(QByteArray::fromBase64(tmpB));
                qDebug()<<targetS;
                srcS=srcS.right(srcS.count()-ret-testS.count());
            }

        }
    }
    return true;
}


