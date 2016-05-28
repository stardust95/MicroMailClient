#ifndef MAILGENERATIONEXCEPTION_H
#define MAILGENERATIONEXCEPTION_H
#include "MailClientException.h"
using namespace std;

class MailGenerationException: public MailClientException
{
public:
    MailGenerationException() {}
    MailGenerationException(const string& exc): MailClientException(exc.c_str()) {}
    MailGenerationException(const MailClientException& mce): MailClientException(mce) {}
    ~MailGenerationException() override {}
    const char* what() override{
        message = exception::what();
        message = "MailGenerationException: " + message;
        return message.c_str();
    }
};

#endif // MAILGENERATIONEXCEPTION_H
