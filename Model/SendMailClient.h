#ifndef SENDMAILCLIENT_H
#define SENDMAILCLIENT_H

#include "MailClient.h"
#include "MailBody.h"

#include <QSharedPointer>
#include <QString>
#include <QDateTime>

class SendMailClient : public MailClient{

public:

        virtual bool sendMailBodies(const MAILBODY_PTR &) = 0;

protected:

};

typedef QSharedPointer<SendMailClient> SENDMAILCLIENT_PTR;

#endif // SENDMAILCLIENT_H

