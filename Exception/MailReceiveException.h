#ifndef MAILRECEIVEEXCEPTION_H
#define MAILRECEIVEEXCEPTION_H
#include "MailClientException.h"

class MailReceiveException: public MailClientException
{
public:
    MailReceiveException() {}
    MailReceiveException(const string& exc): MailClientException(exc.c_str()) {}
    MailClientException(const MailClientException& mce): MailClientException(mce) {}
    ~MailReceiveException() override {}
    const char* what() const override{
        string tmp = MailClientException::what();
        tmp = "MailGenerationException: " + tmp;
        return tmp.c_str();
    }
};

#endif // MAILRECEIVEEXCEPTION_H
