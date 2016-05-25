#ifndef SENDMAILCLIENT_H
#define SENDMAILCLIENT_H

#include "MailClient.h"
#include "MailBody.h"

#include <QSharedPointer>
#include <QString>
#include <QDateTime>

class SendMailClient : MailClient{

public:

		virtual void sendMailBodies(const MAILBODY_PTR_QLIST &, const QList<QString> &) = 0;

protected:

};

typedef QSharedPointer<SendMailClient> SENDMAILCLIENT_PTR;

#endif // SENDMAILCLIENT_H

