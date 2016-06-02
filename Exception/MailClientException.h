#ifndef MAILCLIENTEXCEPTION_H
#define MAILCLIENTEXCEPTION_H
#include <QString>
#include <QtDebug>

#include <string>
#include <exception>
#include <iostream>
using namespace std;

class MailClientException: public exception
{
public:
    string message;
public:
    MailClientException() {
//        qDebug() << "MailClientException default Constructor\n";
        message = exception::what();
    }
    MailClientException(const string& exc): exception(exc.c_str()) {
//        qDebug() << "MailClientException string Constructor\n";
        message = exception::what();
    }
    MailClientException(const MailClientException& mce): exception(mce){
//        qDebug() << "MailClientException copy Constructor\n";
        message = exception::what();
    }
    virtual ~MailClientException() noexcept = 0 {}
    virtual const char* what() = 0;
};



#endif // MAILCLIENTEXCEPTION_H
