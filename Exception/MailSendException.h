#ifndef MAILSENDEXCEPTION_H
#define MAILSENDEXCEPTION_H
#include "MailClientException.h"

class MailSendException: public MailClientException
{
public:
    MailSendException() {
        message = "MailSendException: " + message;
    }
    MailSendException(const string& exc): MailClientException(exc.c_str()) {
        message = "MailSendException: " + message;
    }
    MailSendException(const MailSendException& mce): MailClientException(mce) {
        message = "MailSendException: " + message;
    }
    ~MailSendException() override {}
    const char* what() override{
        return message.c_str();
    }
};

#endif // MAILSENDEXCEPTION_H
