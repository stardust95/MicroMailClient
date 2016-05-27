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
    MailClientException(string exc): exception(exc.c_str()) {}
    virtual ~MailClientException() {
        exception::~exception();
    }
    const char* what() const override{
        string tmp = exception::what();
        tmp = "MailClientException: " + tmp;
        return tmp.c_str();
    }
};


#endif // MAILCLIENTEXCEPTION_H
