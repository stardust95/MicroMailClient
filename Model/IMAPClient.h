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

        for(int i=0; i<_mailList.size (); i++){
            for(int j=0; j<i; j++){
                if( _mailList.at (i).uid == _mailList.at(j).uid )
                    qDebug() << QString::fromUtf8( _mailList.at (i).subject.c_str()) << " and " << QString::fromUtf8 (_mailList.at (j).subject.c_str()) << " has the same uid, subject = " << QString::fromUtf8 (_mailList.at (i).subject.c_str()) << "\n";
            }
        }

        _curListIndex = 0;

        return uids.size ();

    }

    int getMailBodies(QList<MAILBODY_PTR> & result, int count ) override{

        std::string partsPaths;

        int counter = 0;

        for( ; _curListIndex < _mailList.size (); _curListIndex++, counter++) {

            if( counter >= count )
                break;

            auto info = _mailList.at ( _mailList.size () - _curListIndex - 1);

//            if ( info.uid != "1577" )
//                continue;

            _session->loadMessage (_selectedFolder.toStdString (), info);

            MAILBODY_PTR newmail = MAILBODY_PTR::create(QString::fromUtf8 (info.subject.c_str ()));

            newmail->setContent (QString::fromUtf8 (info.text.c_str ()));

            newmail->setHTMLContent (QString::fromUtf8 (info.htmlText.c_str ()));

//            qDebug() << QString::fromUtf8 ( info.uid.c_str() ) << " content: " << newmail->getHTMLContent ()<< "\n";

            std::string sender = info.from.find_last_of('<') != std::string::npos ?
                                info.from.substr(info.from.find_last_of('<')+1, info.from.find_last_of('>') - info.from.find_last_of('<')-1) :
                                info.from ;

            newmail->setSender (QString::fromUtf8 (sender.c_str()));

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

//    QMap<QString, Poco::Net::IMAPClientSession::MessageInfoVec> _mailList;

     SESSION_PTR _session;
	
};

#endif // IMAPCLIENT_H
