#include "configfileclass.h"
#include <QDebug>
#include <QDomDocument>
//#include <QFile>
ConfigFileClass::ConfigFileClass(QWidget *parent) :
    QWidget(parent)
{
}

int ConfigFileClass::readFile(QFile *configFile,QSemaphore *ConfigFileSep)
{
    int ret=true;
    this->configFile=configFile;
    this->ConfigFileSep=ConfigFileSep;
    this->ConfigFileSep->acquire();
    if(!this->configFile->open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Unbuffered))
    {
        this->ConfigFileSep->release();
        return false;
    }
    if(!this->configDoc.setContent(this->configFile, false, &errorStr, &errorLine, &errorColumn))
    {
        this->configFile->close();
        qDebug()<<"setContent file error!!";
        this->ConfigFileSep->release();
        return false;
    }

    this->configFile->close();
    return ret;

}

int ConfigFileClass::writeFile()
{
    if(!this->configFile->open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Unbuffered|QIODevice::Truncate))return false;
    QTextStream out(this->configFile);
    out.setCodec("UTF-8");
    this->configDoc.save(out,4,QDomNode::EncodingFromTextStream);
    this->configFile->close();
    this->ConfigFileSep->release();
    return true;
}

int ConfigFileClass::searchConfigInfo(QString configName,QString *value)
{
    QDomElement rootE=this->configDoc.firstChildElement(QString("Root"));
    QDomElement configE=rootE.firstChildElement(QString("Config"));
    QDomElement NodeE=configE.firstChildElement(configName);
    if (NodeE.isNull())return false;
    *value=NodeE.text();
    return true;
}

int ConfigFileClass::addConfigInfo(QString configName,QString value)
{
    QString ls;
    if(searchConfigInfo(configName,&ls))return false;
    QDomElement rootE=this->configDoc.firstChildElement(QString("Root"));
    QDomElement configE=rootE.firstChildElement(QString("Config"));
    QDomElement newE=this->configDoc.createElement(configName);
    configE.appendChild(newE);
    QDomText newT=this->configDoc.createTextNode(value);
    newE.appendChild(newT);
    return true;
}

int ConfigFileClass::replaceConfigInfo(QString configName,QString value)
{
    if(!deleteConfigInfo(configName))return false;
    if(!addConfigInfo(configName,value))return false;
    return true;
}

int ConfigFileClass::deleteConfigInfo(QString configName)
{
    QString ls;
    if(!searchConfigInfo(configName,&ls))return false;
    qDebug()<<"delete!!";
    QDomElement rootE=this->configDoc.firstChildElement(QString("Root"));
    QDomElement configE=rootE.firstChildElement(QString("Config"));
    QDomElement deleteE=configE.firstChildElement(configName);

    configE.removeChild(deleteE);

    return true;
}


int ConfigFileClass::searchMailInfo(QString id,QString *mailAddr,QString *password)
{
    QDomElement rootE=this->configDoc.firstChildElement(QString("Root"));
    QDomElement mailListE=rootE.firstChildElement(QString("MailList"));
    QDomNodeList list = this->configDoc.elementsByTagName("MailInfo");
    //以标签名进行查找
    for(int i=0; i<list.count(); i++)
    {
        QDomElement e = list.at(i).toElement();
        if(e.attribute("id") == id)
        {  //如果元素的“编号”属性值与我们所查的相同
            *mailAddr=e.firstChildElement("MailAddr").text();
            *password=e.firstChildElement("PassWord").text();
            return true;
        }
    }
    return false;
}



int ConfigFileClass::addMailInfo(QString id,QString mailAddr,QString password)
{
    QString ls,ls2;
    if(searchMailInfo(id,&ls,&ls2))return false;
    QDomElement rootE=this->configDoc.firstChildElement(QString("Root"));
    QDomElement mailListE=rootE.firstChildElement(QString("MailList"));
    QDomElement newMailInfoE=this->configDoc.createElement("MailInfo");
    QDomAttr newId = this->configDoc.createAttribute("id");
    QDomElement newMailAddrE = this->configDoc.createElement("MailAddr");
    QDomElement newPasswordE = this->configDoc.createElement(tr("PassWord"));
    QDomText text;
    newId.setValue(id);
    newMailInfoE.setAttributeNode(newId);
    text = this->configDoc.createTextNode(mailAddr);
    newMailAddrE.appendChild(text);
    text = this->configDoc.createTextNode(password);
    newPasswordE.appendChild(text);
    newMailInfoE.appendChild(newMailAddrE);
    newMailInfoE.appendChild(newPasswordE);
    mailListE.appendChild(newMailInfoE);
    return true;
}


int ConfigFileClass::replaceMailInfo(QString id,QString mailAddr,QString password)
{
    if(!deleteMailInfo(id))return false;
    if(!addMailInfo(id,mailAddr,password))return false;
    return true;
}
int ConfigFileClass::deleteMailInfo(QString id)
{
    QString ls,ls2;
    if(!searchMailInfo(id,&ls,&ls2))return false;
    QDomElement rootE=this->configDoc.firstChildElement(QString("Root"));
    QDomElement mailListE=rootE.firstChildElement(QString("MailList"));
    QDomNodeList list = this->configDoc.elementsByTagName("MailInfo");
    //以标签名进行查找
    for(int i=0; i<list.count(); i++)
    {
        QDomElement e = list.at(i).toElement();
        if(e.attribute("id") == id)
        {  //如果元素的“编号”属性值与我们所查的相同
            e.parentNode().toElement().removeChild(e);
            return true;
        }
    }
    return false;
}


