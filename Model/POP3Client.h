#ifndef POP3CLIENT_H
#define POP3CLIENT_H
#include <iostream>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/POP3ClientSession.h>
#include <QDateTime>
#include <QString>
#include <QSharedPointer>
#include <string>
#include <vector>
#include "Account.h"
#include "ReceiveMailClient.h"
#include "MailBody.h"
#include "Attachment.h"

class POP3Client : public ReceiveMailClient{
private:
    typedef QSharedPointer<Poco::Net::POP3ClientSession> SESSION_PTR;
public:

    POP3Client(QString host,
               QString port = QString::number (Poco::Net::POP3ClientSession::POP3_PORT)) {
        this->_session = new Poco::Net::POP3ClientSession(host,port);
        this->_host = host;
        this->_port = port;
    }


    bool login(QString _user, QString _passwd,bool requireSSL) override{
        try{
            //SSL登陆
            if (requireSSL){
                initializeSSL();
                // always accept even if error occurred
                SharedPtr<InvalidCertificateHandler> ptrHandler = new AcceptCertificateHandler(false);
                Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
                SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);
                SocketAddress socketAddress(host, port);
                SecureStreamSocket socket(socketAddress, ptrContext);
                this->_session = new Poco::Net::POP3ClientSession(socket);
            }
            _session->login(_user.toStdString (), _passwd.toStdString ());
            return true;
        }
        catch(Poco::Net::POP3Exception & e){
            return false;
        }
    }

    int getFolders(QList<QString> & _folders) override{

        QString _name = "Inbox";
        int size = 1;
        _folders.push_back (_name);

        return size;
    }

    int selectFolder(const QString & folder) override{

        _mailList.clear ();

        std::vector<std::string> uids;

        _selectedFolder = folder;

        _session->listMessages(_mailList);

        qDebug() << "In select Folder, folder = "<< folder << ", mailList.size = " << _mailList.size () << "uids.size = " << uids.size ()<< "\n";

        for(int i=0; i<_mailList.size(); i++){
            for(int j=0; j<i; j++){
                Poco::Net::MailMessage message1, message2;
                _session->retrieveMessage((*(_mailList.begin()+j)).id,message1);
                _session->retrieveMessage((*(_mailList.begin()+i)).id,message2);
                if( (*(_mailList.begin()+i)).id == (*(_mailList.begin()+j)).id )
                    qDebug() << QString::fromUtf8(message1.getSubject().c_str()) << " and " << QString::fromUtf8 (message2.getSubject().c_str()) << " has the same uid, subject = " << QString::fromUtf8 (message1.getSubject().c_str()) << "\n";
            }
        }

        _curListIndex = 0;

        return _mailList.size();

    }

    int getMailBodies(QList<MAILBODY_PTR> & result,int count){
        _mailList.clear ();
        _session->listMessages(_mailList);
        std::string partsPaths;
        int counter = 0;
        for( ; _curListIndex < _mailList.size (); _curListIndex++, counter++) {

            if( counter >= count )
                break;

            auto i = _mailList.at ( _mailList.size () - _curListIndex - 1);
            Poco::Net::MailMessage message;
            _session->retrieveMessage(i.id, message);
            MAILBODY_PTR newMail = MAILBODY_PTR::create(QString::fromUtf8 (message.getSubject().c_str()));
            newMail->setContent(QString::fromUtf8 (message.getContent().c_str()));
            newMail->setSender(QString::fromUtf8 (message.getSender().c_str()));
            QString RecipientsList= "";
//            for (auto i=message.Recipients.begin();i!=message.Recipients.end();i++){
//                RecipientsList.append(QString::fromUtf8(*i));
//            }
//            newMail->addRecipient(message.Recipients[0]);
            newMail->setIsread (false);
            result.push_back (newMail);
        }
        return counter;

    }

    int DeleteMail (const QList<int> & ids)override{
        return 0;
    }

    void setTimeout (int val)override{
        Poco::Timespan timeout = Poco::Timespan(0,0,0,0,1000 * val);  //该构造函数默认最小是微妙(第五个参数),前四个:day,hour,minute,second,这里的延时我们需要以毫秒计
        _session->setTimeout(timeout);
    }

    int getTimeout ()override{
        return _session->getTimeout().milliseconds();
    }

    ~POP3Client(){

    }

private:
     Poco::Net::POP3ClientSession::MessageInfoVec _mailList;
     SESSION_PTR _session;
};

#endif // POP3CLIENT_H
