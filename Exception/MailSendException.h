#ifndef MAILSENDEXCEPTION_H
#define MAILSENDEXCEPTION_H
#include "MailClientException.h"

class MailSendException: public MailClientException
{
public:
    MailSendException() {}
    MailSendException(const string& exc): MailClientException(exc.c_str()) {}
    MailSendException(const MailSendException& mce): MailClientException(mce) {
        message = "MailSendException: " + message;
    }
    ~MailSendException() override {}
    const char* what() override{
        message = exception::what();
        message = "MailSendException: " + message;
        return message.c_str();
    }
};

#endif // MAILSENDEXCEPTION_H
