#ifndef MAILRECEIVEEXCEPTION_H
#define MAILRECEIVEEXCEPTION_H
#include "MailClientException.h"

class MailReceiveException: public MailClientException
{
public:
    MailReceiveException() {}
    MailReceiveException(const string& exc): MailClientException(exc.c_str()) {}
    MailReceiveException(const MailReceiveException& mce): MailClientException(mce) {
        message = "MailReceiveException: " + message;
    }
    ~MailReceiveException() override {}
    const char* what() override{
        message = exception::what();
        message = "MailReceiveException: " + message;
        return message.c_str();
    }
};

#endif // MAILRECEIVEEXCEPTION_H
