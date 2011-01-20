#ifndef SYSTEMSOURCE_H
#define SYSTEMSOURCE_H

#include <QWidget>
#include <QSemaphore>
#include <QFile>
#include <QDir>
#include "configfileclass.h"

class SystemSource : public QWidget
{
    Q_OBJECT
public:
    explicit SystemSource(QWidget *parent = 0);
    QSemaphore *MailSendSep;
    QSemaphore *ConfigFileSep;
    QFile ConfigFile;
    QFile LogFile;
    QDir MailRecordFolder;
    ConfigFileClass configFile;
    QString faxDataFolderPath;
    QString faxInfoFolderPath;
    QString configFilePath;
    QString MAIL_USERNAME;
    QString MAIL_ADDR;
    QString MAIL_PASSWORD;
    QString MAIL_POP3SERVER;
    QString MAIL_SMTPSERVER;
    QString MAIL_ADMINMAIL;
    QString MAIL_ADMINPASSWORD;


signals:

public slots:

};

#endif // SYSTEMSOURCE_H
