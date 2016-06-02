#ifndef MAILBODY_H
#define MAILBODY_H

#include "Attachment.h"
#include "Account.h"

#include <QDateTime>
#include <QVariant>
#include<QSharedPointer>
#include<QDebug>

#include <iostream>
#include <string>


using namespace std;

class MailBody : public QObject {
private:
    Q_OBJECT

    Q_PROPERTY(QString subject READ getSubject CONSTANT)

    Q_PROPERTY(QString content READ getContent CONSTANT)

    Q_PROPERTY(QString sender READ getSender CONSTANT)

    Q_PROPERTY(QString datetime READ getDateTime CONSTANT)

    Q_PROPERTY(bool isread READ getIsread WRITE setIsread)

//    Q_PROPERTY(QList<QString> recipients READ getRecipient CONSTANT)


public:

    MailBody(QString _s = "subject", QString _c="content", bool _r = false  ){
        _subject = _s;
        _content = _c;
        _isread = _r;
    }

    QString getSubject(){
        return _subject;
    }

    void setSubject(QString s){
        _subject = s;
    }

    QString getContent(){
        return _content;
    }

    QString getHTMLContent(){
        return _htmlContent;
    }

    void setHTMLContent (QString c){
        _htmlContent = c;
    }

    void setContent(QString c){
        _content = c;
    }

    QString getSender(){
        return _sender;
    }

    void setSender(QString s){
        _sender = s;
    }

    QString getDateTime(){
        return _datetime;
    }

    void setDateTime(QString val){
        _datetime = val;
//        _datetime = QDateTime::fromString (val, Qt::RFC2822Date);
    }

    bool getIsread(){
        return _isread;
    }

    void setIsread(bool val){
        _isread = val;
    }

    QList<QString> getRecipients(){
        return _recipients;
    }

    void addRecipient(QString r) {
        _recipients.push_back (r);
    }

//    QVector<Attachment> GetAttachements(){
//        return attachements;
//    }

//    void addAttachment()

private:

    QString _subject;

    QString _content;

    QString _htmlContent;

    QString _sender;

    QString _datetime;

    QList<QString> _recipients;

    bool _isread;

//    QVector<Attachment> _attachements;

};

typedef QSharedPointer<MailBody> MAILBODY_PTR;
typedef QList<MAILBODY_PTR> MAILBODY_PTR_QLIST;



#endif // MAILBODY_H
