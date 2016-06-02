#ifndef MAILRECEIVEEXCEPTION_H
#define MAILRECEIVEEXCEPTION_H
#include "MailClientException.h"

class MailReceiveException: public MailClientException
{
public:
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
    }
};

#endif // MAILRECEIVEEXCEPTION_H
