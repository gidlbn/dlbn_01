#include "comhandle.h"
#include <QDebug>

ComHandle::ComHandle(SystemSource *SysS,QWidget *parent) :
    QWidget(parent)
{
    this->serialPort= new Posix_QextSerialPort("/dev/ttyUSB0");
    //this->SysS=SysS;
    connect(&this->ComHandleTimer, SIGNAL(timeout()), this, SLOT(Process()));
    this->ComHandleTimer.setSingleShot(true);
    this->ComHandleTimer.start(2*1000);

}

void ComHandle::Process()
{

}
