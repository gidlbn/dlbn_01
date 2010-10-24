#ifndef COMHANDLE_H
#define COMHANDLE_H


#include <QWidget>
#include <QTimer>
#include "posix_qextserialport.h"
#include "systemsource.h"

class ComHandle : public QWidget
{
    Q_OBJECT
public:
    explicit ComHandle(SystemSource *SysS,QWidget *parent = 0);
    Posix_QextSerialPort *serialPort;
    QTimer ComHandleTimer;
    SystemSource *SysS;

signals:

private slots:
    qint8 Process();
    qint8 openSerialPort();

public slots:

};

#endif // COMHANDLE_H
