#ifndef IMAPCLIENT_H
#define IMAPCLIENT_H

#include <QDateTime>
#include <QString>
#include <QSharedPointer>

#include <string>
#include <vector>

#include "Account.h"
#include "ReceiveMailClient.h"
#include "IMAPClientSession.h"
#include "MailBody.h"
#include "Attachment.h"


typedef std::vector<Poco::Net::IMAPClientSession::FolderInfo> FolderInfoVec;

class IMAPClient : public ReceiveMailClient
{
private:
    typedef QSharedPointer<Poco::Net::IMAPClientSession> SESSION_PTR;

public:

    IMAPClient(QString host,
               QString port = QString::number (Poco::Net::IMAPClientSession::IMAP_PORT)) {
        this->_session = SESSION_PTR::create(host.toStdString ());
        this->_host = host;
        this->_port = port;
    }
	
    bool login(QString _user, QString _passwd) override{
        try{
            _session->login(_user.toStdString (), _passwd.toStdString ());

            return true;
		}
		catch(Poco::Net::IMAPException & e){
            return false;
        }
		
    }

    void getFolders(QList<QString> & _folders) override{

        FolderInfoVec folders;

        _session->listFolders("", folders);		// root is null

        for(auto f : folders){
            _folders.push_back (QString::fromUtf8 (f.name.c_str()));
        }

    }

    void getMailBodies(const QString & folder, QList<MAILBODY_PTR> & result) override{

        Poco::Net::IMAPClientSession::MessageInfoVec msgs;

        std::vector<std::string> uids;

        _session->listMessages (folder.toStdString (), uids);

        _session->getMessages (folder.toStdString (), uids, msgs);

        std::string msg;

        for ( auto info : msgs) {

            _session->loadMessage (folder.toStdString (), info, msg);

            MAILBODY_PTR newmail = MAILBODY_PTR::create(QString::fromUtf8 (info.subject.c_str ()));

            newmail->setContent (QString::fromUtf8 (msg.c_str ()));
            newmail->setSender (QString::fromUtf8 (info.from.c_str()));
            newmail->addRecipient (QString::fromUtf8 (info.to.c_str()));
            newmail->setIsread (false);

            result.push_back (newmail);

        }

    }

    void DeleteMail (const QList<int> & ids){

    }

    void setTimeout (int val){

    }

    int getTimeout (){
        return _session->socket ().getReceiveTimeout ().totalMilliseconds ();
    }

    ~IMAPClient(){

    }

private:

     SESSION_PTR _session;
	
};

#endif // IMAPCLIENT_H
