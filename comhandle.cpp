#include "comhandle.h"
#include <QDebug>
#include <QProcess>
#include <QDateTime>

ComHandle::ComHandle(SystemSource *SysS,QWidget *parent) :
    QWidget(parent)
{
    this->SysS=SysS;
    this->serialPort= new Posix_QextSerialPort("/dev/ttyUSB0");



    connect(&this->ComHandleTimer, SIGNAL(timeout()), this, SLOT(Process()));
    this->ComHandleTimer.setSingleShot(true);
    this->ComHandleTimer.start(2*1000);

}

qint8 ComHandle::openSerialPort()
{

    serialPort ->open(QIODevice::ReadWrite);

    //以读写方式打开串口

    serialPort->setBaudRate(BAUD9600);

    //波特率设置，我们设置为9600

    serialPort->setDataBits(DATA_8);

    //数据位设置，我们设置为8位数据位

    serialPort->setParity(PAR_NONE);

    //奇偶校验设置，我们设置为无校验

    serialPort->setStopBits(STOP_1);

    //停止位设置，我们设置为1位停止位

    serialPort->setFlowControl(FLOW_OFF);

    //数据流控制设置，我们设置为无数据流控制

    return true;
}

qint8 ComHandle::Process()
{
    qDebug()<<"ComProcess!!";

    if (!this->serialPort->isOpen())this->openSerialPort();
    if(this->serialPort->bytesAvailable() >=8 )

    //如果可用数据大于或等于8字节再读取

        {
            QDateTime datetime;
            QString shellCmd="fax r -v ";
            QString dateStamp;

            qDebug()<<"read:"<<this->serialPort->bytesAvailable()<<"bytes";
            QByteArray temp = this->serialPort->readAll();
            qDebug()<<"info:"<<QString(temp)<<"\n";
            dateStamp.append(datetime.currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"));
            shellCmd.append(this->SysS->faxDataFolderPath+"/"+dateStamp+".FD >/tmp/"+dateStamp+".log");
            qDebug()<<"shellcmd:"<<shellCmd<<"\n";
            serialPort->close();
            QProcess::execute(shellCmd);
            shellCmd.clear();
            shellCmd.append("ls "+this->SysS->faxDataFolderPath+"/"+dateStamp+".FD* ">this->SysS->faxInfoFolderPath+"/"+dateStamp+".FI");
            /*
            QFile faxInfoFile(this->SysS->faxInfoFolderPath+"/"+dateStamp+".FI");
            if (!faxInfoFile.open(QIODevice::ReadWrite|QIODevice::Text)) return -1;
            QTextStream tmp(&faxInfoFile);
            tmp<<QString(dateStamp+".FD");
            tmp<<"::68486004";
            */






        }

    this->ComHandleTimer.start(2*1000);
}
