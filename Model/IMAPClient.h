#ifndef IMAPCLIENT_H
#define IMAPCLIENT_H

#include "Account.h"
#include "ReceiveMailClient.h"
#include "IMAPClientSession.h"
#include "MailBody.h"
#include "Attachment.h"


#include <QDateTime>
#include <QString>
#include <QSharedPointer>

#include <string>
#include <vector>
#include <sstream>


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
	
    bool login(QString _user, QString _passwd, bool p) override{
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
            _folders.push_back (QString::fromStdString (f.name.c_str()));
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
                    qDebug() << QString::fromStdString( _mailList.at (i).subject.c_str()) << " and " << QString::fromStdString (_mailList.at (j).subject.c_str()) << " has the same uid, subject = " << QString::fromStdString (_mailList.at (i).subject.c_str()) << "\n";
            }
        }

        _curListIndex = 0;

        return uids.size ();

    }

    int getAttachment(const Attachment & attach, std::string & outdata) override{
        std::string data;

        try{
            QStringList tokens = attach.getAccessCommand ().split (';');
//            std::string uid = tokens[0].toStdString();
            qDebug() << attach.getAccessCommand () << "\n";
            auto info = _mailList.at ( tokens[0].toInt() );
            _session->loadPartData (info.uid, info.parts, tokens[1].toStdString(), data);
            outdata = data;
            return 1;
        }

        catch( const std::exception & e ){
            qDebug() << "Unable to get attachment, exception:" << e.what () << "\n";
            return 0;
        }

//        std::cout << "write file " << paths.begin ()->first << ", " << data.length () << std::endl;
//        std::ofstream output;
//        output.open ("blink.pptx", ofstream::out | ofstream::binary);
//        if( output.is_open () ){

//            output.write (data.c_str (), data.length ());
//            output.close ();
//        }else{
//            std::cout << "Unable to open file" << std::endl;
//        }

    }

    int getMailBodies(QList<MAILBODY_PTR> & result, int count ) override{

        std::map<std::string, std::string> paths;

        int counter = 0;

        for( ; _curListIndex < _mailList.size (); _curListIndex++, counter++) {

            if( counter >= count )
                break;

            int actualIndex = _mailList.size () - _curListIndex - 1;

            auto info = _mailList.at ( actualIndex );

            _session->loadMessage (_selectedFolder.toStdString (), info, paths);

            MAILBODY_PTR newmail = MAILBODY_PTR::create(QString::fromStdString (info.subject.c_str ()));

            for( auto it = paths.begin (); it != paths.end (); it++){

                auto partInfo = info.parts.childs[std::atoi(it->second.c_str())];

                if( partInfo.attributes[0] == "BOUNDARY" )
                    break;

                Attachment attach(partInfo.attributes[0],partInfo.attributes[1]);

                attach.setFileName (QString::fromStdString (it->first));

                attach.setFileSize ( std::atoi (partInfo.attributes[5].c_str()));

                std::stringstream ss;
                ss << actualIndex << ";" << it->second;

                attach.setAccessCommand ( QString::fromStdString ( ss.str () ) );


//                qDebug() << actualIndex << " || " << QString::fromStdString(it->second ) << " ||\n";
                qDebug() << QString::fromStdString ( ss.str () ) << "\n";

                newmail->addAttachment (attach);
            }


            newmail->setContent (QString::fromStdString (info.text.c_str ()));

            newmail->setHTMLContent (QString::fromStdString (info.htmlText.c_str ()));

//            qDebug() << QString::fromStdString ( info.uid.c_str() ) << " content: " << newmail->getHTMLContent ()<< "\n";

            std::string sender = info.from.find_last_of('<') != std::string::npos ?
                                info.from.substr(info.from.find_last_of('<')+1, info.from.find_last_of('>') - info.from.find_last_of('<')-1) :
                                info.from ;

            newmail->setSender (QString::fromStdString (sender.c_str()));

            newmail->addRecipient (QString::fromStdString (info.to.c_str()));

            newmail->setDateTime (QString::fromStdString (info.date.c_str()));

            newmail->setIsread (false);



            result.push_back (newmail);
        }

        return counter;

    }

    int getAttachment(){

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
