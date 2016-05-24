#ifndef IMAPCLIENT_H
#define IMAPCLIENT_H

#include <QDateTime>

#include <string>
#include <vector>

#include "Account.h"
#include "IMAPClientSession.h"

typedef std::vector<Poco::Net::IMAPClientSession::FolderInfo> FolderInfoVec;

class IMAPClient : public ReceiveMailClient {
public:

    IMAPClient(string host, int port)
		: _session(host, port) {
		
    }
	
	bool login(string _user, string _passwd){
		
		try{
			_session.login(_user, _passwd);
		
			FolderInfoVec folders;
			
			_session.listFolders("", folders);		// root is null
			
			for ( auto folder : folders ){
				
			}
		}
		catch(Poco::Net::IMAPException & e){
			


		}
		
		
		
	}

private:

    QDateTime _loginTime;

    bool _requireSSL;
	
	IMAPClientSession _session;
	
};

#endif // IMAPCLIENT_H
