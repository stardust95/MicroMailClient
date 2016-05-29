#ifndef MAILCLIENT_H
#define MAILCLIENT_H

#include <QSharedPointer>
#include <QString>
#include <QDateTime>

class MailClient{

public:

        MailClient(){
            _requireSSL = true;
            _loginTime = QDateTime::currentDateTime ();
        }

        enum ConnectionType{
           TCPConnectoin, SSLConnection, TLSConnection
        };

        virtual bool login(QString, QString, bool ) = 0;

		virtual int getTimeout() = 0;
		
        virtual void setTimeout(int)     = 0;
		
//       virtual ~MailClient() = 0;

protected:

        QString _host;
        QString _port;
        QDateTime _loginTime;
        bool _requireSSL;

};

typedef QSharedPointer<MailClient> MAILCLIENT_PTR;

#endif // MAILCLIENT_H

