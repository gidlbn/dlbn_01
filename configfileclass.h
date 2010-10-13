#ifndef CONFIGFILECLASS_H
#define CONFIGFILECLASS_H

#include <QWidget>
#include <QFile>
#include <QDomDocument>
#include <QSemaphore>

class ConfigFileClass : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigFileClass(QWidget *parent = 0);
    QFile *configFile;
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument configDoc;
    QSemaphore *ConfigFileSep;
    int readFile(QFile *configFilePath,QSemaphore *ConfigFileSep);
    int writeFile();
    int searchConfigInfo(QString configName,QString *value);
    int addConfigInfo(QString configName,QString value);
    int replaceConfigInfo(QString configName,QString value);
    int deleteConfigInfo(QString configName);
    int addMailInfo(QString id,QString mailAddr,QString password);
    int searchMailInfo(QString id,QString *mailAddr,QString *password);
    int deleteMailInfo(QString id);
    int replaceMailInfo(QString id,QString mailAddr,QString password);


signals:

public slots:

};

#endif // CONFIGFILECLASS_H
