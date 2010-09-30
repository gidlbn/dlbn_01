#include "faxtomailmodule.h"
#include "smtp.h"
#include "Base64.h"
#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QPrinter>
#include <QTimer>
//#include <QTest>
#include <QPainter>
#include <QPrintPreviewDialog>

FaxToMailModule::FaxToMailModule(QWidget *parent) :
    QWidget(parent)
{
    MailServer="smtp.sina.com";
    MailPassword="si123456";
    MailUsername="fax_test@sina.com";
    MailFrom="fax_test@sina.com";
    FaxToMailTimer.setSingleShot(true);
    connect(&FaxToMailTimer, SIGNAL(timeout()), this, SLOT(Process()));
    FaxToMailTimer.start(1*1000);
}


bool FaxToMailModule::FaxIdentify(QString FilePath,QString &Addr)
{
     Addr="dlbn126@126.com";
     return true;
}


qint8 FaxToMailModule::Process()
{
        qDebug()<<"FaxToMailProcess Start\n";
        this->FIFolder.setPath("/root/chuanzhen/faxtransfer/faxinfo");//FaxInfoFolder
        this->FDFolder.setPath("/root/chuanzhen/faxtransfer/faxdate");//FaxDateFolder
        this->FileFilter.clear();
        this->FileFilter.append("*.FI");
        this->FIFolder.setNameFilters(this->FileFilter);
        qint32 FileCount=this->FIFolder.count();
        qDebug()<<"FileCount="<<FileCount<<"\n";
        QStringList FIFileList = this->FIFolder.entryList(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);

        while (!FIFileList.isEmpty())
            {
                QString FIFilePath;
                //qDebug()<<FIFileList.first();
                FIFilePath.clear();
                FIFilePath.append(this->FIFolder.absolutePath()+"/"+FIFileList.first());
                qDebug()<<FIFilePath;

                QFile FIfile(FIFilePath);
                if (!FIfile.open(QIODevice::ReadWrite|QIODevice::Text)) return -1;
                QByteArray tmp;
                tmp=FIfile.readAll();
                QStringList FIfileinfo;
                FIfileinfo=QString(tmp).split("::");
                qDebug()<<FIfileinfo;
                FIfile.close();
                FDFilePath.clear();
                FDFilePath.append(FDFolder.absolutePath()+"/"+FIfileinfo.first());
                qDebug()<<FDFilePath;
                Re=FaxIdentify(FDFilePath,MailTo);
                if (Re==true)
                {
                   qDebug()<<"begin to print\n";
                   /*
                   QPrinter printer;
                   printer.setOutputFormat(QPrinter::PdfFormat);
                   printer.setOrientation(QPrinter::Landscape);
                   printer.setPageSize(QPrinter::A2);
                   printer.setOutputFileName("/tmp/nonwritable.pdf");
                   QPainter painter;
                   QImage Image(FDFilePath);
                   if (! painter.begin(&printer))
                   { // failed to open file
                        qWarning("failed to open file, is it writable?");
                        return 1;
                   }
                    painter.drawImage(0,0,Image);
                    painter.end();
                    */
                    QFile PDFFile("/tmp/nonwritable.pdf");
                    if(PDFFile.open(QIODevice::ReadOnly|QIODevice::Text))
                    {
                        /*
                        QByteArray ls;
                        ls=PDFFile.readAll();
                        QString ls2;

                        //qDebug()<<ls.toBase64();
                        */
                        char ReadData[3];
                        char Base64Data[4];
                        Base64 base64;
                        qint8 ReadSize=4;
                        while (ReadSize>0)
                        {
                            ReadSize=PDFFile.read(ReadData,3);
                            int Base64Size=4;
                            base64.Base64Encode(ReadData,int(ReadSize),Base64Data,&Base64Size);
                            QString BData;
                            while (Base64Size>0)
                            {

                            }

                        }


                        if (this->CheckMailServerInfo()<0)
                        {
                            //qDebug()<<"Error:MailServerInfo="<<this->CheckMailServerInfo();
                        }
                        else
                        {
                            QList<QString> bcc;
                            bcc<<"dlbn@sina.com";
                            /*
                            Smtp mail(this->MailServer,this->MailFrom,this->MailTo,bcc,"test","test",true,QString(tmp.toBase64()));
                            mail.current_user_name=this->MailUsername;
                            mail.current_password=this->MailPassword;
                            mail.send();
                            */
                            /*
                            Smtp *smtp = new Smtp("smtp.sina.com","dlbn@sina.com","dlbn126@126.com",bcc,"Testmail","Testmail",true,tmp.toBase64());
                            smtp->current_user_name="dlbn@sina.com";
                            smtp->current_password="xl123456";
                            smtp->send();
                            */

                        }
                        PDFFile.close();

                    }
                    else
                    {

                    }





                }
                else
                {

                }


                FIFileList.removeFirst();


            }
    //}
    //FaxToMailTimer.start(3*1000);
    qDebug()<<"FaxToMailProcess End\n";
    return true;
}

qint8 FaxToMailModule::CheckMailServerInfo()
{
    if (this->MailServer.isNull())     return -1;
    if (this->MailPassword.isNull())   return -2;
    if (this->MailUsername.isNull())   return -3;
    return true;
}

qint8 FaxToMailModule::SetMailServerInfo()
{
    return true;
}
