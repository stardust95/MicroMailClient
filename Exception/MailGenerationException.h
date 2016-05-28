#ifndef MAILGENERATIONEXCEPTION_H
#define MAILGENERATIONEXCEPTION_H
#include "MailClientException.h"
using namespace std;

class MailGenerationException: public MailClientException
{
public:
    MailGenerationException() {
//        qDebug() << "MailGenerationException default Constructor\n";
        message = "MailGenerationException: " + message;
    }
    MailGenerationException(const string& exc): MailClientException(exc.c_str()) {
//        qDebug() << "MailGenerationException string Constructor\n";
        message = "MailGenerationException: " + message;
    }
    MailGenerationException(const MailGenerationException& mce): MailClientException(mce) {
//        qDebug() << "MailGenerationException copy Constructor\n";
        message = "MailGenerationException: " + message;
    }
    ~MailGenerationException() override {}
    const char* what() override{
        return message.c_str();
    }
};

#endif // MAILGENERATIONEXCEPTION_H
