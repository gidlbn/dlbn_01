#ifndef CONFIGFILECLASS_H
#define CONFIGFILECLASS_H

#include <QWidget>
#include <QFile>
#include <QDomDocument>

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
    int openFile(QFile *configFilePath);
signals:

public slots:

};

#endif // CONFIGFILECLASS_H
