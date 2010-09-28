#include "faxtomailmodule.h"
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
    FaxToMailTimer.setSingleShot(true);
    connect(&FaxToMailTimer, SIGNAL(timeout()), this, SLOT(Process()));
    FaxToMailTimer.start(5*1000);
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
                QString MailAddr;
                Re=FaxIdentify(FDFilePath,MailAddr);
                if (Re==true)
                {
                   qDebug()<<"begin to print\n";
                   QPrinter printer;
                   printer.setOutputFormat(QPrinter::PdfFormat);
                   printer.setOutputFileName("/tmp/nonwritable.pdf");
                   //QPrintPreviewDialog preview(&printer, this);
                   QPainter painter;


                    if (! painter.begin(&printer))
                    { // failed to open file
                        qWarning("failed to open file, is it writable?");
                        return 1;
                    }
                    painter.drawText(10, 10, "Test");
                    if (! printer.newPage())
                    {
                        qWarning("failed in flushing page to disk, disk full?");
                        return 1;
                    }
                    painter.drawText(10, 10, "Test 2");
                    painter.end();




                }
                else
                {

                }


                FIFileList.removeFirst();


            }
    //}
    //FaxToMailTimer.start(3*1000);
    qDebug()<<"FaxToMailProcess End\n";
}
