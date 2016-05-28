#ifndef MAILRECEIVEEXCEPTION_H
#define MAILRECEIVEEXCEPTION_H
#include "MailClientException.h"

class MailReceiveException: public MailClientException
{
public:
    MailReceiveException() {}
    MailReceiveException(const string& exc): MailClientException(exc.c_str()) {}
    MailReceiveException(const MailClientException& mce): MailClientException(mce) {}
    ~MailReceiveException() override {}
    const char* what() override{
        message = exception::what();
        message = "MailReceiveException: " + message;
        return message.c_str();
    }
};

#endif // MAILRECEIVEEXCEPTION_H
