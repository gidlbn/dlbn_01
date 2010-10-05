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
    MailServer="smtp.sogou.com";
    MailPassword="so123456";
    MailUsername="fax_test@sogou.com";
    MailFrom="fax_test@sogou.com";
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

                   QPrinter printer;
                   printer.setOutputFormat(QPrinter::PdfFormat);
                   printer.setOrientation(QPrinter::Landscape);
                   printer.setPageSize(QPrinter::A2);
                   printer.setOutputFileName("/tmp/nonwritable.pdf");
                   QPainter painter;
                   QImage Image;
                   Image.load(FDFilePath);
                   qDebug()<<Image.size();
                   if (! painter.begin(&printer))
                   { // failed to open file
                        qWarning("failed to open file, is it writable?");
                        return 1;
                   }

                   //QPixmap pixmapToShow = QPixmap::fromImage( Image.scaled(size(), Qt::KeepAspectRatio) );
                   //painter.drawPixmap(0,0, pixmapToShow);
                   painter.drawImage(0,0,Image);
                   /*
                   QFont font("Times",50);
                   QString text = tr("Hello World");
                   painter.setPen(Qt::blue);
                   painter.setFont(font);
                   painter.drawText(QRect(0,0, 1000, 100), text);
                    */
                   painter.end();

                    QFile PDFFile("/tmp/nonwritable.pdf");
                    if(PDFFile.open(QIODevice::ReadOnly))
                    {


                        QByteArray ls;
                        ls=PDFFile.readAll();
                        PDFFile.close();
                        //qDebug()<<"File Size="<<ls.size();
                        this->MailAttachment=ls.toBase64();
                        //qDebug()<<ls.toBase64();
                        if (this->CheckMailServerInfo()<0)
                        {
                            qDebug()<<"Error:MailServerInfo="<<this->CheckMailServerInfo();
                        }
                        else
                        {
                            QList<QString> bcc;
                            bcc<<"dlbn@sina.com";

                            Smtp *mail=new Smtp(this->MailServer,this->MailFrom,this->MailTo,bcc,"test","test",true,this->MailAttachment);
                            mail->current_user_name=this->MailUsername;
                            mail->current_password=this->MailPassword;
                            mail->send();


                        }


                    }
                    else
                    {
                        qFatal("Can Not find the PDF file!!!");
                    }
                    FIFileList.removeFirst();
                }
                else
                {

                }




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
