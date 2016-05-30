#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H
#include <iostream>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/SMTPClientSession.h>
#include <QDateTime>
#include <QString>
#include <QSharedPointer>
#include <string>
#include <vector>
#include "Account.h"
#include "SendMailClient.h"
#include "MailBody.h"
#include "Attachment.h"
class SMTPClient:public SendMailClient{
private:
    typedef QSharedPointer<Poco::Net::SMTPClientSession> SESSION_PTR;
public:
    SMTPClient(QString host,
               QString port = QString::number (Poco::Net::SMTPClientSession::SMTP_PORT)){
        this->_session = SESSION_PTR::create(host.toStdString ());
        this->_host = host;
        this->_port = port;
    }
    bool login(QString _user, QString _passwd) override{
        try{
            _session->login(_user.toStdString (), _passwd.toStdString ());
            return true;
        }
        catch(Poco::Net::SMTPException & e){
            return false;
        }
    }
    void sendMailBodies(QString to, QString from, QString subject, QString content)override{
        MailMessage message;
        message.setSender(from);
        message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, to));
        message.setSubject(subject);
        message.setContentType("text/plain; charset=UTF-8");
        message.setContent(content, MailMessage::ENCODING_8BIT);
        try{
            _session->sendMessage(message);
            return true;
//            cout<<"Message successfully sent" << endl;
        }
        catch(SMTPException &e){
            return false;
        }
    }
    void setTimeout(int val)override{
        timeout = Poco::Timespan(0,0,0,0,1000 * val);  //该构造函数默认最小是微妙(第五个参数),前四个:day,hour,minute,second,这里的延时我们需要以毫秒计
        _session->setTimeout(timeout);
    }
    int getTimeout ()override{
        return _session->getTimeout().milliseconds();
    }
    ~SMTPClient(){

    }

private:
     SESSION_PTR _session;
};

#endif // SMTPCLIENT_H
