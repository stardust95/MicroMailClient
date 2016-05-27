#ifndef MAILSENDEXCEPTION_H
#define MAILSENDEXCEPTION_H
#include "MailClientException.h"

class MailSendException: public MailClientException
{
public:
    MailSendException() {}
    MailSendException(string exc): MailClientException(exc.c_str()) {}
    virtual ~MailSendException() {
        MailClientException::~MailClientException();
    }
    const char* what() const override{
        string tmp = MailClientException::what();
        tmp = "MailGenerationException: " + tmp;
        return tmp.c_str();
    }
};

#endif // MAILSENDEXCEPTION_H
