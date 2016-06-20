#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include "Account.h"
#include "SendMailClient.h"
#include "MailBody.h"
#include "Attachment.h"

#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/FilePartSource.h>

#include <QDateTime>
#include <QString>
#include <QSharedPointer>

#include <string>
#include <vector>
#include <iostream>


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
    bool login(QString _user, QString _passwd,bool requireSSL) override{
        try{
            _session->login(Poco::Net::SMTPClientSession::AUTH_LOGIN,_user.toStdString (), _passwd.toStdString ());
            return true;
        }
        catch(Poco::Net::SMTPException & e){
            return false;
        }
    }

    bool sendMailBodies(const MAILBODY_PTR & MailBody) override{
//        MAILBODY_PTR newMail = MAILBODY_PTR::create();
        Poco::Net::MailMessage newMail;

        newMail.setSender(MailBody->getSender().toStdString());

        auto list = MailBody->getRecipients();

        for (auto i = list.begin(); i != list.end(); i++){
            std::cout << i->toStdString () << std::endl;
            Poco::Net::MailRecipient mailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT, i->toStdString());
            newMail.addRecipient(mailRecipient);
        }

        newMail.setSubject(MailBody->getSubject().toStdString());
        newMail.setContentType("text/html; charset=UTF-8");

        newMail.setContent(MailBody->getHTMLContent().toStdString());

        for(auto attach: MailBody->getAttachements ()){
            newMail.addAttachment (attach.getFileName ().toStdString (), new Poco::Net::FilePartSource(attach.getFilePath ().toStdString ()));
        }

//        newMail->setHTMLContent(MailBody->getHTMLContent());

        newMail.setDate(Poco::Timestamp::fromEpochTime( QDateTime::fromString(MailBody->getDateTime()).toTime_t()));

        try{
            _session->sendMessage(newMail);
            return true;
            qDebug() <<"Message successfully sent\n" ;
        }
        catch(Poco::Net::SMTPException &e){
            std::cout << e.what () << std::endl;
            return false;
        }
    }
    void setTimeout(int val)override{
        Poco::Timespan timeout = Poco::Timespan(0,0,0,0,1000 * val);  //该构造函数默认最小是微妙(第五个参数),前四个:day,hour,minute,second,这里的延时我们需要以毫秒计
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
