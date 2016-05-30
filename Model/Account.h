#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <QList>
#include <QSharedPointer>
#include <qvector.h>
#include <iostream>
#include <QVariant>
#include <QSharedPointer>
#include <QDebug>

class Account: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString username READ getUserName WRITE setUserName)

    Q_PROPERTY(QString password READ getPassWord WRITE setPassWord)

    Q_PROPERTY(QString mailhost READ getMailHost WRITE setMailHost)

    Q_PROPERTY(QString smtphost READ getSMTPHost WRITE setSMTPHost)

    Q_PROPERTY(QString pop3host READ getPOP3Host WRITE setPOP3Host)

    Q_PROPERTY(QString imaphost READ getIMAPHost WRITE setIMAPHost)

    Q_PROPERTY(bool requiressl READ getRequireSSL WRITE setRequireSSL)

    QString _userName;
    QString _passWord;
    QString _mailHost;
    QString _smtpHost;
    QString _pop3Host;
    QString _imapHost;
    bool _requireSSL;
    QVector<QString> _contacts;

public:
    Account() {}

    Account(QString un, QString pw) {
        _userName = un;
        _passWord = pw;
    }

    void AddContact(QString c) {
        _contacts.push_back(c);
    }

    QString getUserName() const {
        return _userName;
    }

    void setUserName(QString s) {
        _userName = s;
    }

    QString getPassWord() const {
        return _passWord;
    }

    void setPassWord(QString s) {
        _passWord = s;
    }

    QString getMailHost() const {
        return _mailHost;
    }

    void setMailHost(QString s) {
        _mailHost = s;
    }

    QString getSMTPHost() const {
        return _smtpHost;
    }

    void setSMTPHost(QString s) {
        _smtpHost = s;
    }

    QString getPOP3Host() const {
        return _pop3Host;
    }

    void setPOP3Host(QString s) {
        _pop3Host = s;
    }

    QString getIMAPHost() const {
        return _imapHost;
    }

    void setIMAPHost(QString s) {
        _imapHost = s;
    }

    bool getRequireSSL() const {
        return _requireSSL;
    }

    void setRequireSSL(bool i) {
        _requireSSL = i;
    }


};


typedef QSharedPointer<Account> ACCOUNT_PTR;
typedef QList<ACCOUNT_PTR> ACCOUNT_PTR_QLIST;



#endif // ACCOUNT_H


