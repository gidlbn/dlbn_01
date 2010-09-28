#ifndef FAXTOMAILMODULE_H
#define FAXTOMAILMODULE_H
#include <QObject>
#include <QDir>
#include <QPrinter>
#include <QTimer>
//#include <QTest>

#include <QWidget>

class FaxToMailModule : public QWidget
{
    Q_OBJECT
public:
    explicit FaxToMailModule(QWidget *parent = 0);
    QTimer FaxToMailTimer;
    uint FileCount,Re;
    QStringList FileFilter;
    QString FIFilePath,FDFilePath;//FaxInfoFilePath,FaxDateFilePath
    QDir FIFolder;//FaxInfoFolder
    QDir FDFolder;//FaxDateFolder
    QString MailServer



signals:

private slots:
    qint8 Process();

private:
    bool FaxIdentify(QString FilePath,QString &Addr);

};

#endif // FAXTOMAILMODULE_H
