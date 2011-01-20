#ifndef FAXTOMAILMODULE_H
#define FAXTOMAILMODULE_H
#include <QObject>
#include <QDir>
#include <QPrinter>
#include <QTimer>
#include <QSemaphore>
//#include <QTest>

#include <QWidget>
#include "systemsource.h"

class FaxToMailModule : public QWidget
{
    Q_OBJECT
public:
    explicit FaxToMailModule(SystemSource *SysS,QWidget *parent = 0);
    QTimer FaxToMailTimer;
    uint FileCount,Re;
    QStringList FileFilter;
    QString FIFilePath,FDFilePath;//FaxInfoFilePath,FaxDateFilePath
    QDir FIFolder;//FaxInfoFolder
    QDir FDFolder;//FaxDateFolder
    QString MailServer,MailFrom,MailTo,MailSubject,MailBody,MailAttachment;
    QString MailUsername,MailPassword;
    SystemSource *SysS;
    qint8 CheckMailServerInfo();
    qint8 SetMailServerInfo();

signals:

private slots:
    qint8 Process();


private:
    bool FaxIdentify(QString qcode,QString &Addr);

};

#endif // FAXTOMAILMODULE_H
