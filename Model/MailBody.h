#ifndef MAILBODY_H
#define MAILBODY_H

#include "Attachment.h"
#include "Account.h"

#include <QDateTime>
#include <QVariant>
#include<QSharedPointer>
#include<QDebug>

#include <string>


using namespace std;

class MailBody : public QObject {
private:
    Q_OBJECT

    Q_PROPERTY(QString subject READ getSubject CONSTANT)

    Q_PROPERTY(QString content READ getContent CONSTANT)

    Q_PROPERTY(QString sender READ getSender CONSTANT)

    Q_PROPERTY(QDateTime datetime READ getDateTime CONSTANT)

    Q_PROPERTY(bool isread READ getIsread WRITE setIsread CONSTANT)

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

    void setContent(QString c){
        _content = c;
    }

    QString getSender(){
        return _sender;
    }

    void setSender(QString s){
        _sender = s;
    }

    QDateTime getDateTime(){
        return _datetime;
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

    void addRecipient(QString r)
{
        _recipients.push_back (r);
    }

//    QVector<Attachment> GetAttachements(){
//        return attachements;
//    }

private:

    QString _subject;

    QString _content;

    QString _sender;

    QDateTime _datetime;

    QList<QString> _recipients;

    bool _isread;

//    QVector<Attachment> _attachements;

};

typedef QSharedPointer<MailBody> MAILBODY_PTR;
typedef QList<MAILBODY_PTR> MAILBODY_PTR_QLIST;



#endif // MAILBODY_H
