#ifndef MAILCLIENTEXCEPTION_H
#define MAILCLIENTEXCEPTION_H
<<<<<<< HEAD
<<<<<<< HEAD
#include <qstring.h>
=======
=======
>>>>>>> mxydls-master
#include <QString>
#include <QtDebug>

#include <string>
<<<<<<< HEAD
>>>>>>> repairing_pop3_decode
=======
>>>>>>> mxydls-master
#include <exception>
#include <iostream>
using namespace std;

class MailClientException: public exception
{
public:
<<<<<<< HEAD
<<<<<<< HEAD
    MailClientException() {}
    MailClientException(const string& exc): exception(exc.c_str()) {}
    MailClientException(const MailClientException& mce): exception(mce) {}
    virtual ~MailClientException() throw() = 0;
    virtual const char* what() const = 0;
};

MailClientException::~exception() {}
=======
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


>>>>>>> repairing_pop3_decode
=======
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


>>>>>>> mxydls-master

#endif // MAILCLIENTEXCEPTION_H
