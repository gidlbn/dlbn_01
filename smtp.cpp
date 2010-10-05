#include <QtNetwork/QTcpSocket>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QLocale>
#include <QList>
#include "smtp.h"


Smtp::Smtp( const QString &server, const QString &from, const QString &to, const QList<QString> &bcc, const QString &subject, const QString &body, const bool html ,const QString &attachment)
{
    socket = new QTcpSocket( this );

    connect( socket, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
    connect( socket, SIGNAL( connected() ), this, SLOT( connected() ) );
    connect( socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(errorReceived(QAbstractSocket::SocketError)));
    connect( socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this,
    SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(disconnected()), this,
    SLOT(disconnected()));;

    //Generate date
    QTime myTime = QDateTime::currentDateTime().toUTC().time();
    QLocale myLocal = QLocale(QLocale::C);
    QString date = myLocal.toString(QDate::currentDate(), "ddd, dd MMM yyyy ");
    date += myLocal.toString(myTime, "hh:mm:ss");
    date += " +0000 (UTC)";

    message = "To: " + to + "\n";
    message.append("From: " + from + "\n");
    message.append("Subject: " + subject + "\n");
    message.append("Date: " + date + "\n");
    message.append("MIME-Version: 1.0 \n");

    message.append("Content-Type: multipart/mixed;\n");
    message.append("    boundary=\"=-mail_mix\"\n\n");
    //test
    message.append("--=-mail_mix\n");
    message.append("Content-Type: text/plain;\n");
    message.append("charset=\"GBK\"\n");
    message.append("Content-Transfer-Encoding: base64 \n");
    message.append("Content-Disposition: inline\n\n");
    QByteArray ls;
    ls.append(body);
    message.append(ls.toBase64()+"\n\n\n");
    message.append("--=-mail_mix\n");
    //attachment
    message.append("Content-Type: application/octet-stream; name=\"Fax.pdf\" \n");
    message.append("Content-Description: Fax.pdf \n");
    message.append("Content-Disposition: attachment; filename=\"Fax.pdf\"; size=1955;creation-date=\"Thu, 05 Aug 2010 13:09:54 GMT\";modification-date=\"Thu, 05 Aug 2010 13:14:51 GMT\" \n");
    message.append("Content-Transfer-Encoding: base64 \n\n");
    message.append(attachment+"\n");
    message.append("--=-mail_mix--\n");
    message.replace( QString::fromLatin1( "\n" ), QString::fromLatin1( "\r\n" ) );
    message.replace( QString::fromLatin1( "\r\n.\r\n" ),QString::fromLatin1( "\r\n..\r\n" ) );
    this->from = from;

    //Add to adress
    rcpt = bcc;
    rcpt.insert(0, to);
    rcptIndex = 0;
    this->server = server;
    this->To = to;
    //qDebug()<<"Smtp::Smtp finished!!";
}

void Smtp::send()
{
		state = Init;
		emit status( 1 ); //Connecting
		socket->connectToHost( server, 25);
		if(socket->waitForConnected ( 100000 )) 	{qDebug("constructor"); 	}
		
		t = new QTextStream( socket );
		emit status( 2 ); //Connected
}
Smtp::~Smtp()
{
    delete t;
    delete socket;
}
void Smtp::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() <<"stateChanged " << socketState;
}

void Smtp::errorReceived(QAbstractSocket::SocketError socketError)
{
    qDebug() << "error " <<socketError;
    qDebug() << socketError;
}

void Smtp::disconnected()
{
		qDebug() <<"disconneted";
        if(socket->errorString() == "The remote host closed the connection")
        qDebug()<< socket->errorString();
        else qDebug() << "error " ;
}

void Smtp::connected()
{
		qDebug() << "Connected ";
}

void Smtp::readyRead()
{
     qDebug() <<"readyRead";

    QString UserName = current_user_name;
    QString PassWord = current_password;
    while(!UserName.endsWith("@"))
    {
        UserName.chop(1);
    }
    UserName.chop(1);

    QByteArray user;
    user.append(UserName);
    QByteArray password;
    password.append(PassWord);

    QString responseLine;
    do
    {
        responseLine = socket->readLine();
        response += responseLine;
    }
    while ( socket->canReadLine() && responseLine[3] != ' ' );

	responseLine.truncate( 3 );


    if ( state == Init && responseLine[0] == '2' )
    {
         qDebug()<<"responseLine is"<<responseLine<<"\n";
        // banner was okay, let's go on
		*t << "HELO there\r\n";
		t->flush();
		
//        state = Mail;
        state = Login;
        qDebug()<<"HELO there";        
        emit status( 3 ); //Sending
    }
    //----------------------------------------------------------------//
/*    else if ( state == Ehlo && responseLine[0] == '2' )
    {
        // banner was okay, let's go on
        *t << "EHLO 163.com\r\n";
        t->flush();
        state = Login;
        emit status( 4 ); //Sending
    }
*/
    else if ( state == Login && responseLine[0] == '2' )
    {
//        qDebug()<<"responseLine is"<<responseLine<<"\n";
        // banner was okay, let's go on
        *t << "AUTH LOGIN\r\n";      
        t->flush();
        state = User;
        emit status( 4 ); //Sending
//        qDebug()<<"AUTH LOGIN";

    }
    else if ( state == User && responseLine[0] == '3' )
    {
//        qDebug()<<"responseLine is"<<responseLine<<"\n";
        *t << user.toBase64() << "\r\n";        
        t->flush();
        state = Password;
//        qDebug()<<"user:"<<user.toBase64();
        emit status( 5 ); //Sending
    }
    else if ( state == Password && responseLine[0] == '3' )
    {
//        qDebug()<<"responseLine is"<<responseLine<<"\n";
        *t << password.toBase64() << "\r\n";
        t->flush();
        state = Mail;
        emit status( 6 ); //Sending
//        qDebug()<<"password:"<<password.toBase64();

    }
    //----------------------------------------------------------------//
    else if ( state == Mail && responseLine[0] == '2' )
    {
//        qDebug()<<"responseLine is"<<responseLine<<"\n";
        // HELO response was okay (well, it has to be)
        *t << "MAIL FROM:<" << from << ">"<<"\r\n";
		t->flush();
//        state = Rcpt;
        state = Rcpt;
        emit status( 7); //Sending
//        qDebug()<<"MAIL FROM:<"<< from <<">";

    }
    else if ( state == Mail && responseLine[0] == '5' )
    {
        QMessageBox::warning( 0, tr( "Error" ), tr( "User and PassWord are wrong!" ) );
        this->socket->close();
    }
    else if ( state == Rcpt && responseLine[0] == '2' )
    {
//        qDebug()<<"responseLine is"<<responseLine<<"\n";
        for (rcptIndex=0;rcptIndex<=rcpt.size();rcptIndex++)
        {
            QString adress = rcpt[rcptIndex];
            *t << "RCPT TO:<" << adress << ">\r\n"; //
        }
//        *t << "RCPT TO:<" << To << ">\r\n"; //
//        rcptIndex++;
		t->flush();
        emit status( 8 ); //Sending 5
//        qDebug()<<"RCPT TO:<>";
//        if (rcptIndex >= rcpt.size())
            state = Data;
    }

    else if ( state == Data && responseLine[0] == '2' )
    {
//        qDebug()<<"responseLine is"<<responseLine<<"\n";
        *t << "DATA\r\n";
		t->flush();
        state = Body;
        emit status( 9 ); //Sending 6
//        qDebug()<<"DATA";
    }
    else if ( state == Body && responseLine[0] == '3' )
    {
//        qDebug()<<"responseLine is"<<responseLine<<"\n";
        *t << message << "\r\n.\r\n";
		t->flush();
        state = Quit;
        emit status( 10 );
        qDebug()<<"sent message ...";        
    }
    else if ( state == Quit && responseLine[0] == '2' )
    {
        qDebug()<<"responseLine is"<<responseLine<<"\n";
        *t << "QUIT\r\n";
		t->flush();
        // here, we just close.
        state = Close;
//        emit status( 11 ); //Sending7
        qDebug()<<"QUIT";
    }
    else if ( state == Close )
    {
        qDebug()<<"responseLine is"<<responseLine<<"\n";
        deleteLater();
        QMessageBox::warning( 0, tr( "Qt Mail Example" ), tr( "e_mail had sent\n" ) + response );
        socket->close();
        return;
    }
    else
    {
        // something broke.
        qDebug()<<"responseLine is"<<responseLine<<"\n";
        //qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        QMessageBox::warning( 0, tr( "Qt Mail Example" ), tr( "Unexpected reply from SMTP server:\n\n" ) + response );
        emit status(-1);
        socket->close();
    }
    response = "";
}
