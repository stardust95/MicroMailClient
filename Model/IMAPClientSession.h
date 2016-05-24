#ifndef IMAPCLIENTSESSION_H
#define IMAPCLIENTSESSION_H


// ...
#include <WinSock2.h>

#include "Poco/Net/Net.h"
#include "Poco/Net/SocketDefs.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/NetSSL.h"
#include "Poco/Net/X509Certificate.h"
#include "Poco/Net/Context.h"
#include "Poco/NumberFormatter.h"
#include "Poco/StreamCopier.h"



#include <istream>
#include <iostream>
#include <string>
#include <sstream>



namespace Poco {
	namespace Net {

		class IMAPClientSession
		{
		public:
			enum
			{
				IMAP_PORT = 143
			};

			struct FolderInfo
				/// Information returned by listFolders().
			{
				std::string name;
				std::string flags;
			};

			struct PartInfo
			{
				std::vector<std::string> attributes;	
				std::vector<PartInfo> childs;				
			};

			struct MessageInfo
				/// Information returned by listMessages().
			{
				std::string uid;
				std::string from;
				std::string to;
				std::string subject;
				std::string date;
				std::string flags;
				PartInfo    parts;

				bool        seen;
				bool        forwarded;
				int         size;

				//std::vector<PartInfo> parts;
			};			

			typedef std::vector<FolderInfo> FolderInfoVec;
			typedef std::vector<MessageInfo> MessageInfoVec;

			explicit IMAPClientSession(const StreamSocket& socket);
			IMAPClientSession(const std::string& host, Poco::UInt16 port = IMAP_PORT);

			
			virtual bool checkCapability(const std::string& cap);

			~IMAPClientSession();

			void setTimeout(const Poco::Timespan& timeout);			

			Poco::Timespan getTimeout() const;			

			virtual void login(const std::string& username, const std::string& password);
			
			void close();
			void noop();

			virtual void capability();

            std::vector<std::string>& getCapability() { return _capability; }

			virtual void copyMessage(const std::string& uid, const std::string& from_folder, const std::string& to_folder);
			virtual void moveMessage(const std::string& uid, const std::string& from_folder, const std::string& to_folder);
			virtual void deleteMessage(const std::string& uid, const std::string& folder, bool expunge = true);
			virtual void loadMessage(const std::string& folder, const std::string& uid, std::string& message);

			virtual void createFolder(const std::string& folder);
			virtual void deleteFolder(const std::string& folder);
			virtual void listFolders(const std::string& root, FolderInfoVec& folders);

			virtual void listMessages(const std::string& folder, const std::string& filter, const std::string& order, std::vector<std::string>& uids);
			virtual void getMessages(const std::string& folder, std::vector<std::string>& uids, MessageInfoVec& messages);

			virtual void subscribe(const std::string& folder);
			virtual void unsubscribe(const std::string& folder);

			virtual bool sendCommand(const std::string& command, std::string& response, std::vector<std::string>& data);

            DialogSocket&  socket() { return _socket; }

            std::string&  folderSeparator() { return _folder_separator; }

			template < class T = std::string>
			static void tokenize(const T& str, std::vector<T>& tokens,
				const T& delimiters, const T& parenthesis, bool trimParenthesis = false)
			{
				T token;
				T opened_par;

                for (std::string::value_type c : str)
				{
					for (int i = 0; i < parenthesis.size(); i += 2)
					{
						// Open a new one.
						if (c == parenthesis[i]) {
							// special case, when the open char = close char, a second ocurrence must close, not open a new one.
							if (!(opened_par.size()>0 &&
								opened_par[opened_par.size() - 1] == c &&
								parenthesis[i] == parenthesis[i + 1])) {
								opened_par.push_back(parenthesis[i]);
								continue;
							}
						}

						// if close, check that if is already open.
						if (c == parenthesis[i + 1] && opened_par.size()) {
							// Check if is the last open
							if (parenthesis[i] == opened_par[opened_par.size() - 1]) {
								// close it
								opened_par.pop_back();
							}
						}
					}

					if (opened_par.size()>0 || (delimiters.find(c) == std::string::npos)) {
						token += c;
					}
					else {
						// remove parenthesis if apply
						if (trimParenthesis) {
							for (int i = 0; i < parenthesis.size(); i++) {
								if (token.size() < 2) break;
								if (token[0] == parenthesis[i] && token[token.size() - 1] == parenthesis[i + 1])
									token = token.substr(1, token.length() - 2);
							}
						}
						tokens.push_back(token);
						token.clear();
					}
				}

				if (!token.empty()) {
					if (trimParenthesis) {
						for (int i = 0; i < parenthesis.size(); i++) {
							if (token.size() < 2) break;
							if (token[0] == parenthesis[i] && token[token.size() - 1] == parenthesis[i + 1])
								token = token.substr(1, token.length() - 2);
						}
					}
					tokens.push_back(token);
				}
            }

            std::string host() { return _host;  }

		private:
			void moveMessage_without_MOVE(const std::string& uid, const std::string& from_folder, const std::string& to_folder);
            PartInfo parseBodyStructure(std::istringstream& src);

			DialogSocket              _socket;
			bool                      _isOpen;
			int                       _tag;
			std::string               _curTag, _host;			
			std::string               _folder_separator;			
			std::vector<std::string>  _capability;
		};

	}
}



#endif IMAPCLIENTSESSION_H
