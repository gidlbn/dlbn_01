#include "comhandle.h"
#include <QDebug>

ComHandle::ComHandle(SystemSource *SysS,QWidget *parent) :
    QWidget(parent)
{
    this->serialPort= new Posix_QextSerialPort("/dev/ttyUSB0");
    //this->SysS=SysS;
    this->serialPort->setBaudRate(BAUD256000);
    connect(&this->ComHandleTimer, SIGNAL(timeout()), this, SLOT(Process()));
    this->ComHandleTimer.setSingleShot(true);
    this->ComHandleTimer.start(2*1000);

}

void ComHandle::Process()
{
    qDebug()<<"ComProcess!!";
    if(this->serialPort->bytesAvailable() >=8 )

    //如果可用数据大于或等于8字节再读取

        {
            qDebug()<<"read:"<<this->serialPort->bytesAvailable()<<"bytes";
            QByteArray temp = this->serialPort->readAll();


       }

    this->ComHandleTimer.start(2*1000);
}
