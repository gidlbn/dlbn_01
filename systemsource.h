#ifndef SYSTEMSOURCE_H
#define SYSTEMSOURCE_H

#include <QWidget>
#include <QSemaphore>
#include <QFile>
#include <QDir>

class SystemSource : public QWidget
{
    Q_OBJECT
public:
    explicit SystemSource(QWidget *parent = 0);
    QSemaphore *MailSendSep;
    QSemaphore *ConfigFileWriteableSep;
    QFile ConfigFile;
    QFile LogFile;
    QDir MailRecordFolder;


signals:

public slots:

};

#endif // SYSTEMSOURCE_H
