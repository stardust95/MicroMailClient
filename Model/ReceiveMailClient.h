#ifndef RECEIVEMAILCLIENT_H
#define RECEIVEMAILCLIENT_H

#include "MailClient.h"
#include "MailBody.h"

#include <QSharedPointer>
#include <QString>
#include <QDateTime>

class ReceiveMailClient : MailClient{

public:

        virtual void getFolders(QList<QString> & ) = 0;

        virtual void getMailBodies(const QString & , QList<MAILBODY_PTR> & ) = 0;   // DO NOT CLEAR the list in this function

		virtual void DeleteMail(const QList<int> &) = 0;
		
protected:

};

typedef QSharedPointer<ReceiveMailClient> RECEIVEMAILCLIENT_PTR;

#endif // RECEIVEMAILCLIENT_H

