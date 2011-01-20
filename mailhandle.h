#ifndef MAILHANDLE_H
#define MAILHANDLE_H

#include <QWidget>
#include <QSemaphore>
#include <QTimer>
#include "Pop3Client.h"
#include "systemsource.h"

class MailHandle : public QWidget
{
    Q_OBJECT
public:
    explicit MailHandle(SystemSource *SysS,QWidget *parent = 0);
    QTimer MailHandleTimer;
    Pop3Client pop3client;
    SystemSource *SysS;
    QString Pop3Server,Pop3User,Pop3Password;

private:
    int readMail(QString srcS);
    QFile MailFile;



signals:

private slots:
    void Process();

public slots:


};



#endif // MAILHANDLE_H
