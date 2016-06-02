#ifndef MAILSENDEXCEPTION_H
#define MAILSENDEXCEPTION_H
#include "MailClientException.h"

class MailSendException: public MailClientException
{
public:
<<<<<<< HEAD
<<<<<<< HEAD
    MailSendException() {}
    MailSendException(const string& exc): MailClientException(exc.c_str()) {}
    MailClientException(const MailClientException& mce): MailClientException(mce) {}
    ~MailSendException() override {}
    const char* what() const override{
        string tmp = MailClientException::what();
        tmp = "MailGenerationException: " + tmp;
        return tmp.c_str();
=======
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
>>>>>>> repairing_pop3_decode
=======
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
>>>>>>> mxydls-master
    }
};

#endif // MAILSENDEXCEPTION_H
