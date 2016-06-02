#ifndef MAILRECEIVEEXCEPTION_H
#define MAILRECEIVEEXCEPTION_H
#include "MailClientException.h"

class MailReceiveException: public MailClientException
{
public:
<<<<<<< HEAD
    MailReceiveException() {}
    MailReceiveException(const string& exc): MailClientException(exc.c_str()) {}
    MailClientException(const MailClientException& mce): MailClientException(mce) {}
    ~MailReceiveException() override {}
    const char* what() const override{
        string tmp = MailClientException::what();
        tmp = "MailGenerationException: " + tmp;
        return tmp.c_str();
=======
    MailReceiveException() {
        message = "MailReceiveException: " + message;
    }
    MailReceiveException(const string& exc): MailClientException(exc.c_str()) {
        message = "MailReceiveException: " + message;
    }
    MailReceiveException(const MailReceiveException& mce): MailClientException(mce) {
        message = "MailReceiveException: " + message;
    }
    ~MailReceiveException() override {}
    const char* what() override{
        return message.c_str();
>>>>>>> repairing_pop3_decode
    }
};

#endif // MAILRECEIVEEXCEPTION_H
