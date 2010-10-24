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


signals:

public slots:

};

#endif // SYSTEMSOURCE_H
