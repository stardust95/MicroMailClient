#ifndef MAILCLIENTEXCEPTION_H
#define MAILCLIENTEXCEPTION_H
#include <string>
#include <qstring.h>
#include <exception>
#include <iostream>
using namespace std;

class MailClientException: public exception
{
protected:
    string message;
public:
    MailClientException() {}
    MailClientException(const string& exc): exception(exc.c_str()) {}
    MailClientException(const MailClientException& mce): exception(mce) {}
    virtual ~MailClientException() throw() = 0;
    virtual const char* what() = 0;
};

MailClientException::~MailClientException() {}

#endif // MAILCLIENTEXCEPTION_H
