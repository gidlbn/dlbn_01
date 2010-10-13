#include "systemsource.h"

SystemSource::SystemSource(QWidget *parent) :
    QWidget(parent)
{
    this->ConfigFile.setFileName("/root/faxtransfer.config");
    this->LogFile.setFileName("/root/faxtransfer.Log");
    this->MailSendSep=new QSemaphore(1);
    this->ConfigFileSep=new QSemaphore(1);
    this->MailRecordFolder.setPath("/root/MailRecord");
    if (!this->MailRecordFolder.exists())this->MailRecordFolder.mkdir(this->MailRecordFolder.absolutePath());
}
