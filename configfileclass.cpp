#include "configfileclass.h"
//#include <QFile>
ConfigFileClass::ConfigFileClass(QWidget *parent) :
    QWidget(parent)
{
}

int ConfigFileClass::openFile(QFile *configFilePath)
{
    int ret=true;
    this->configFile=configFilePath;
    if(!this->configFile->open(QIODevice::ReadWrite|QIODevice::Text))return false;
    if(!this->configDoc.setContent(this->configFile, false, &errorStr, &errorLine, &errorColumn))
    {
        this->configFile->close();
        return false;
    }


    return ret;

}
