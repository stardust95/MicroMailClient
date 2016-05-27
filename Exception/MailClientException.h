#ifndef MAILCLIENTEXCEPTION_H
#define MAILCLIENTEXCEPTION_H
#include <qstring.h>
#include <exception>
#include <iostream>
using namespace std;

class MailClientException: public exception
{
public:
    MailClientException() {}
    MailClientException(const string& exc): exception(exc.c_str()) {}
    MailClientException(const MailClientException& mce): exception(mce) {}
    virtual ~MailClientException() throw() = 0;
    virtual const char* what() const = 0;
};

MailClientException::~exception() {}

#endif // MAILCLIENTEXCEPTION_H
