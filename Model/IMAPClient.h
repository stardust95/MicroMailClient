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

    int getFolders(QList<QString> & _folders) override{

        FolderInfoVec folders;

        _session->listFolders("", folders);		// root is null

        for(auto f : folders){
            _folders.push_back (QString::fromUtf8 (f.name.c_str()));
        }

        return folders.size ();
    }

    int selectFolder(const QString & folder) override{

        _mailList.clear ();

        std::vector<std::string> uids;

        _selectedFolder = folder;

        _session->listMessages (folder.toStdString (), uids);

        _session->getMessages (folder.toStdString (), uids, _mailList);

        qDebug() << "In select Folder, folder = "<< folder << ", mailList.size = " << _mailList.size () << "uids.size = " << uids.size ()<< "\n";

        _curListIndex = 0;

        return uids.size ();

    }

    int getMailBodies(QList<MAILBODY_PTR> & result, int count ) override{

        std::string msg;

        int counter = 0;

        qDebug() << "listIndex = " <<_curListIndex << ", mailList.size=" << _mailList.size () << "\n";

        for( ; _curListIndex < _mailList.size (); _curListIndex++, counter++) {

            if( counter >= count )
                break;

            auto info = _mailList.at (_curListIndex);

            _session->loadMessage (_selectedFolder.toStdString (), info, msg);

            MAILBODY_PTR newmail = MAILBODY_PTR::create(QString::fromUtf8 (info.subject.c_str ()));

            newmail->setContent (QString::fromUtf8 (msg.c_str ()));
            newmail->setSender (QString::fromUtf8 (info.from.c_str()));
            newmail->addRecipient (QString::fromUtf8 (info.to.c_str()));
            newmail->setDateTime (QString::fromUtf8 (info.date.c_str()));
            newmail->setIsread (false);

            result.push_back (newmail);
        }

        return counter;

    }

    int DeleteMail (const QList<int> & ids) override{
        return 0;
    }

    void setTimeout (int val) override{
        _session->socket ().setReceiveTimeout (val);
        _session->socket ().setSendTimeout (val);
    }

    int getTimeout () override{
        return _session->socket ().getReceiveTimeout ().totalMilliseconds ();
    }

    ~IMAPClient(){

    }

private:


    Poco::Net::IMAPClientSession::MessageInfoVec _mailList;

     SESSION_PTR _session;
	
};

#endif // IMAPCLIENT_H
