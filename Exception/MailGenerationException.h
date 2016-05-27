#ifndef MAILGENERATIONEXCEPTION_H
#define MAILGENERATIONEXCEPTION_H
#include "MailClientException.h"

class MailGenerationException: public MailClientException
{
public:
    MailGenerationException() {}
    MailGenerationException(string exc): MailClientException(exc.c_str()) {}
    virtual ~MailGenerationException() {
        MailClientException::~MailClientException();
    }
    const char* what() const override{
        string tmp = MailClientException::what();
        tmp = "MailGenerationException: " + tmp;
        return tmp.c_str();
    }
};

#endif // MAILGENERATIONEXCEPTION_H
