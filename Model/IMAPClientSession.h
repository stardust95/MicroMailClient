#ifndef IMAPCLIENTSESSION_H
#define IMAPCLIENTSESSION_H

#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/NetSSL.h"
#include "Poco/Net/X509Certificate.h"
#include "Poco/Net/Context.h"
#include "Poco/NumberFormatter.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/MailMessage.h"


#include <istream>
#include <iostream>
#include <string>
#include <vector>
#include <ostream>
#include <streambuf>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>

namespace Poco {
    namespace Net {

        POCO_DECLARE_EXCEPTION (Net_API, IMAPException, NetException)

        class IMAPClientSession {
        public:
            enum {
                IMAP_PORT = 143
            };

            struct FolderInfo
                /// Information returned by listFolders().
            {
                std::string name;
                std::string flags;
            };

            struct PartInfo {			// 只存放一个Mail每个部分的信息
                std::vector<std::string> attributes;
                std::vector<PartInfo> childs;
            };

            struct MessageInfo		// 只存放每个Mail的信息
                /// Information returned by listMessages().
            {
                std::string uid;
                std::string from;
                std::string to;
                std::string subject;
                std::string date;
                std::string flags;
                PartInfo    parts;

                std::string htmlText;
                std::string text;

                bool        seen;
                bool        forwarded;
                int         size;

            };

            typedef std::vector<FolderInfo> FolderInfoVec;
            typedef std::vector<MessageInfo> MessageInfoVec;
            typedef std::vector<PartInfo>		PartInfoVec;

            explicit IMAPClientSession (const StreamSocket& socket);
            IMAPClientSession (const std::string& host, Poco::UInt16 port = IMAP_PORT);

            virtual bool checkCapability (const std::string& cap);

            ~IMAPClientSession ( );

            void setTimeout (const Poco::Timespan& timeout);

            Poco::Timespan getTimeout ( ) const;

            virtual void login (const std::string& username, const std::string& password);

            void close ( );
            void noop ( );

            virtual void capability ( );

            std::vector<std::string>& getCapability ( ) { return _capability; }

            virtual void copyMessage (const std::string& uid, const std::string& from_folder, const std::string& to_folder);
            virtual void moveMessage (const std::string& uid, const std::string& from_folder, const std::string& to_folder);
            virtual void deleteMessage (const std::string& uid, const std::string& folder, bool expunge = true);
            virtual void loadMessage (const std::string & folder, MessageInfo & info, std::map<std::string, std::string> & paths);
            virtual void loadText (const std::string & uid, const PartInfo & info, const std::string & index, const std::string & type, std::string & text);

            virtual void loadParts (const std::string & uid, const PartInfo & info, const std::string & index, const std::string & type, std::map<std::string, std::string> &);
            virtual void loadPartData(const std::string & uid, const PartInfo & info, const std::string &, std::string & data);

            virtual void createFolder (const std::string& folder);
            virtual void deleteFolder (const std::string& folder);
            virtual void listFolders (const std::string& root, FolderInfoVec& folders);

            virtual void listMessages (const std::string& folder, std::vector<std::string>& uids);
            virtual void getMessages (const std::string& folder, std::vector<std::string>& uids, MessageInfoVec& messages);

            virtual bool sendCommand (const std::string& command, std::string& response, std::vector<std::string>& data);

            DialogSocket&  socket ( ) { return _socket; }

            std::string host ( ) { return _host; }


        private:
            void moveMessage_without_MOVE (const std::string& uid, const std::string& from_folder, const std::string& to_folder);
            PartInfo parseBodyStructure (std::istringstream& src);

            DialogSocket              _socket;
            bool                      _isOpen;
            int                       _tag;
            std::string               _curTag, _host;
//            std::string               _folder_separator;
            std::vector<std::string>  _capability;
        };

    }
}




#endif IMAPCLIENTSESSION_H
