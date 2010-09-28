#ifndef SMTP_H
#define SMTP_H

#include <QApplication>
#include <QMessageBox>
#include <QtDebug>
#include <QString>
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QLocale>
#include <QList>


class Smtp : public QObject
{
    Q_OBJECT
    
public:
    QString current_user_name;
    QString current_password;
    Smtp( const QString &server, const QString &from, const QString &to, const QList<QString> &bcc, const QString &subject, const QString &body, const bool html=false ,const QString &attachment="");
    ~Smtp();
    
    void send();
    
signals:
    void status( const int);

private slots:
		
		void stateChanged(QAbstractSocket::SocketState socketState);
		void errorReceived(QAbstractSocket::SocketError socketError);
		void disconnected();
		void connected();
		void readyRead();
private:
		QString message;
		QTextStream *t;
		QTcpSocket *socket;
		QString server;
		QString from;
		QList<QString> rcpt;
		int rcptIndex;
		QString response;
        enum states{Rcpt, Mail, Data, Init, Body, Quit, Close, User, Password, Ehlo, Login};
		int state;
        QString To;
};
#endif /* SMTP_H */
