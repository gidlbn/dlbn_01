#include "systemsource.h"
#include <QDebug>

SystemSource::SystemSource(QWidget *parent) :
    QWidget(parent)
{
    QByteArray tmp;
    QString qtmp;
    this->ConfigFile.setFileName("/root/faxtransfer.config");
    this->LogFile.setFileName("/root/faxtransfer.Log");
    this->MailSendSep=new QSemaphore(1);
    this->ConfigFileSep=new QSemaphore(1);
    this->MailRecordFolder.setPath("/root/MailRecord");
    this->faxDataFolderPath="/root/FaxData";
    this->faxInfoFolderPath="/root/FaxInfo";
    this->configFilePath="/root/config";
    if (!this->MailRecordFolder.exists())this->MailRecordFolder.mkdir(this->MailRecordFolder.absolutePath());
    QFile configFile(this->configFilePath);

    if (!configFile.open(QIODevice::ReadOnly|QIODevice::Text))qDebug()<<"error\n" ;
    while(!configFile.atEnd())
    {
        tmp=configFile.readLine();
        qtmp=QString(tmp);
        qDebug()<<qtmp<<"\n";

    }
    configFile.close();



}
