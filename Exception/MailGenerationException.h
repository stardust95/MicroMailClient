#ifndef MAILGENERATIONEXCEPTION_H
#define MAILGENERATIONEXCEPTION_H
#include "MailClientException.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
using namespace std;
>>>>>>> repairing_pop3_decode
=======
using namespace std;
>>>>>>> mxydls-master

class MailGenerationException: public MailClientException
{
public:
<<<<<<< HEAD
<<<<<<< HEAD
    MailGenerationException() {}
    MailGenerationException(const string& exc): MailClientException(exc.c_str()) {}
    MailClientException(const MailClientException& mce): MailClientException(mce) {}
    ~MailGenerationException() override {}
    const char* what() const override{
        string tmp = MailClientException::what();
        tmp = "MailGenerationException: " + tmp;
        return tmp.c_str();
=======
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
>>>>>>> repairing_pop3_decode
=======
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
>>>>>>> mxydls-master
    }
};

#endif // MAILGENERATIONEXCEPTION_H
