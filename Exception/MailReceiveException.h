#ifndef MAILRECEIVEEXCEPTION_H
#define MAILRECEIVEEXCEPTION_H
#include "MailClientException.h"

class MailReceiveException: public MailClientException
{
public:
    MailReceiveException() {}
    MailReceiveException(string exc): MailClientException(exc.c_str()) {}
    virtual ~MailReceiveException() {
        MailClientException::~MailClientException();
    }
    const char* what() const override{
        string tmp = MailClientException::what();
        tmp = "MailGenerationException: " + tmp;
        return tmp.c_str();
    }
};

#endif // MAILRECEIVEEXCEPTION_H
