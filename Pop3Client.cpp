#include "Pop3Client.h"
#include <iostream>
#include <QtDebug>

#define _ERROR(s) std::cerr << s << m_sock.errorString().toStdString() << std::endl
Pop3Client::Pop3Client(bool readOnly)
{
	this->readOnly = readOnly;
	state = NotConnected;
	m_sock.blockSignals(true);
}

void Pop3Client::SetReadOnly(bool readOnly)
{
	this->readOnly = readOnly;
}

bool Pop3Client::Connect(QString host,short unsigned int port)
{
	m_sock.connectToHost(host,port);
	if (!m_sock.waitForConnected(CONNECT_TIMEOUT))
	{
		_ERROR("Could not connect: ");
		return false;
	}
	QString response;
	ReadResponse(false,response);
	if (response.startsWith("+OK"))
    {
		state = Authorization;
        qDebug() << "connect response.startwhith ok";
    }
	else
		return false;
	return true;
}

//Sends a command to the server and returns the response
QString Pop3Client::doCommand(QString command,bool isMultiline)
{
    qDebug() << "sending command: " << command << "\n";
	QString response;
	qint64 writeResult = m_sock.write(command.toAscii());
	if (writeResult != command.size()) _ERROR("Could not write all bytes: ");
	if (writeResult > 0 && !m_sock.waitForBytesWritten(WRITE_TIMEOUT)) _ERROR("Could not write bytes from buffer: ");
	if (!ReadResponse(isMultiline,response))
    {
        qDebug() << "docommand readresponse";
		return "";
    }
	return response;
}

bool Pop3Client::ReadResponse(bool isMultiline,QString& response)
{
	char buff[1512+1]; // according to RFC1939 the response can only be 512 chars
	bool couldRead = m_sock.waitForReadyRead( READ_TIMEOUT ) ;
	if (!couldRead) _ERROR("could not receive data: ");
    bool complete = false;
    bool completeLine = false;
    unsigned int offset = 0;
	while (couldRead && !complete)
	{
        qDebug() << "had in while";
        qDebug() << "reading front\n";
		if (offset >= sizeof(buff))
		{
			qDebug() << "avoiding buffer overflow, server is not RFC1939 compliant\n";
			return false;
        }//end if
		qint64 bytesRead = m_sock.readLine(buff + offset,sizeof(buff)-offset);
		if (bytesRead == -1)
			return false;
		couldRead = bytesRead > 0;
		completeLine = buff[offset+bytesRead-1]=='\n';
        if (couldRead)
        {//if_1
            if (completeLine)
            {//if_2
                qDebug() << "Line Complete\n";
				offset = 0;
				if (response.size() == 0)
                {//if_3
                    //first line, check for error
                    qDebug() << "response.size() == 0";
					response.append(buff);
					if (!response.startsWith("+OK"))
						complete = true;
					else
                    {
						complete = !isMultiline;
                        qDebug() << "response.startsWith +OK";
                    }
                }//end if_3
				else
                {//else_2
                    //NOT first line, check for byte-stuffing
					//check if the response was complete
					complete = ( strcmp(buff,".\r\n") == 0 );
					if (!complete)
					{
						if (buff[0] == '.' && buff[1]== '.')
							response.append(buff+1);	//remove the stuffed byte and add to the response
						else
							response.append(buff);	//add to the response
					}
                }// end else_2
            }//end if_2
			else
            {//else_1
                qDebug() << "Line INComplete: " << buff <<"\n";
				offset += bytesRead;
            }//end else_1
        }//end_1
		if (couldRead && !complete)
		{
            qDebug() << "couldRead && !complete";
			if (m_sock.bytesAvailable() <= 0)
			{
                qDebug() << "waiting for data\n";
				couldRead = m_sock.waitForReadyRead( READ_TIMEOUT ) ;
                qDebug() << "waiting for data finished, couldread: " << couldRead << "\n";
			}
        }//end if
    }//end while
    qDebug() << "return couldRead && complete";
	return couldRead && complete;
}

bool Pop3Client::Login(QString user, QString pwd)
{
	if (!SendUser(user))
		return false;
	if (!SendPasswd(pwd))
		return false;
	state = Transaction;
	return true;
}
bool Pop3Client::SendUser(QString& user)
{
	QString res = doCommand("USER "+user+"\r\n",false);
	if (res.startsWith("+OK"))
		return true;
	else
		return false;
    qDebug() << "sent user is ok";
}
bool Pop3Client::SendPasswd(QString& pwd)
{
	QString res = doCommand("PASS "+pwd+"\r\n",false);
	if (res.startsWith("+OK"))
		return true;
	else
		return false;
    qDebug() << "sent password is ok";
}
bool Pop3Client::Quit()
{
	if (readOnly)
		if (!ResetDeleted())
			return false;
	QString res = doCommand("QUIT\r\n",false);
	if (res.startsWith("+OK"))
	{
		if (!m_sock.waitForDisconnected(DISCONNECT_TIMEOUT))
		{
			_ERROR("Connection was not closed by server: ");
			return false;
		}
		else
			state = NotConnected;
		return true;
	}
	else
	{
		return false;
	}
    qDebug() << "sent quit is ok";
}
bool Pop3Client::GetMailboxStatus(int& nbMessages, int& totalSize)
{
	QString res = doCommand("STAT\r\n",false);
	if (res.startsWith("+OK"))
	{
		QStringList sl = res.split(' ',QString::SkipEmptyParts);
		if (sl.count() < 3)
			return false;
		else
		{
			nbMessages = sl[1].toInt();
			totalSize = sl[2].toInt();
		}
		return true;
	}
	else
		return false;
}
bool Pop3Client::ResetDeleted()
{
	QString res = doCommand("RSET\r\n",false);
	if (res.startsWith("+OK"))
		return true;
	else
		return false;
}
bool Pop3Client::NoOperation()
{
	QString res = doCommand("NOOP\r\n",false);
	if (res.startsWith("+OK"))
		return true;
	else
		return false;
}
QPair<QString,QString> Pop3Client::parseUniqueIdListing(QString& line)
{
	QPair<QString,QString> p;
	QStringList sl = line.split(' ',QString::SkipEmptyParts);
	p.first = sl[0];
	p.second = sl[1];
	return p;
}
QPair<QString,int> Pop3Client::parseMsgIdListing(QString& line)
{
	QPair<QString,int> p;
	QStringList sl = line.split(' ',QString::SkipEmptyParts);
	p.first = sl[0];
	p.second = sl[1].toInt();
	return p;
}
bool Pop3Client::GetUniqueIdList(QVector< QPair<QString,QString> >& uIdList)
{
	QString res = doCommand("UIDL\r\n",true);
	if (res.startsWith("+OK"))
	{
		QStringList sl = res.split("\r\n",QString::SkipEmptyParts);
		int i;
		for (i=1;i<sl.count();i++)
			uIdList.append(parseUniqueIdListing(sl[i]));
		return true;
	}
	else
		return false;
}
bool Pop3Client::GetUniqueIdList(QString msgId, QPair<QString,QString>& uIdList)
{
	QString res = doCommand("UIDL "+msgId+"\r\n",false);
	if (res.startsWith("+OK"))
	{
		QStringList sl = res.split(' ',QString::SkipEmptyParts);
		uIdList.first = sl[1];
		uIdList.second = sl[2];
		return true;
	}
	else
		return false;
}
bool Pop3Client::Delete(QString msgId)
{
        qDebug() << "Pop3Client::Delete start";
	if (readOnly)
		return false;
	QString res = doCommand("DELE "+msgId+"\r\n",false);
	if (res.startsWith("+OK"))
		return true;
	else
		return false;
    qDebug() << "sent delete is ok";
}
bool Pop3Client::GetMessageTop(QString msgId, int nbLines, QString& msgTop)
{
	QString res = doCommand("TOP "+msgId+" "+QString::number(nbLines)+"\r\n",true);
	if (res.startsWith("+OK"))
	{
		msgTop = res.section("\r\n",1);
		return true;
	}
	else
		return false;
}
bool Pop3Client::GetMessage(QString msgId, QString& msg)
{
	QString res = doCommand("RETR "+msgId+"\r\n",true);
	if (res.size() == 0)
		return false;
	if (res.startsWith("+OK"))
	{
		msg = res.section("\r\n",1);
		return true;
	}
	else
	{
        qDebug() << "ErrResponse: " << res;
		return false;
	}
    qDebug() << "get message is ok";
}
bool Pop3Client::GetMsgList(QVector< QPair<QString,int> >& uIdList)
{
	QString res = doCommand("LIST\r\n",true);
	if (res.startsWith("+OK"))
	{
		QStringList sl = res.split("\r\n",QString::SkipEmptyParts);
		int i;
		for (i=1;i<sl.count();i++)
			uIdList.append(parseMsgIdListing(sl[i]));
		return true;
	}
	else
		return false;
}
bool Pop3Client::GetMsgList(QString msgId, QPair<QString,int>& uIdList)
{
	QString res = doCommand("LIST "+msgId+"\r\n",false);
	if (res.startsWith("+OK"))
	{
		QStringList sl = res.split(' ',QString::SkipEmptyParts);
		uIdList.first = sl[1];
		uIdList.second = sl[2].toInt();
		return true;
	}
	else
		return false;
}
bool Pop3Client::LoginWithDigest(QString user, QString digest)
{
	QString res = doCommand("APOP "+user+" "+digest+"\r\n",false);
	if (res.startsWith("+OK"))
		return true;
	else
		return false;
}


#undef _ERROR

