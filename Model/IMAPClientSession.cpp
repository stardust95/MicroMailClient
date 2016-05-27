#include "IMAPClientSession.h"

#include "Poco/DateTimeFormatter.h"
#include "Poco/Net/QuotedPrintableDecoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/StringTokenizer.h"
#include "Poco/String.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/TextEncoding.h"
#include "Poco/TextConverter.h"
#include "Poco/Net/MessageHeader.h"

#include <Windows.h>

#include <iostream>
#include <codecvt>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <fstream>

using Poco::NumberFormatter;
using Poco::DateTimeFormatter;
using Poco::StreamCopier;
using Poco::Base64Decoder;
using Poco::StringTokenizer;
using Poco::Net::MessageHeader;

using Poco::trim;
using Poco::toUpper;

namespace Poco {
    namespace Net {

        //POCO_IMPLEMENT_EXCEPTION(IMAPException, NetException, "IMAP Exception")

        // GB2312转UTF8
        std::string GB2312ToUTF8(const char* lpszGb32Text)
        {
            int nUnicodeBufLen = MultiByteToWideChar(CP_ACP, 0, lpszGb32Text, -1, 0, 0);
                if (nUnicodeBufLen == 0)
                    return "";

                WCHAR* pUnicodeBuf = new WCHAR[nUnicodeBufLen];
                if (pUnicodeBuf == 0)
                    return "";

                MultiByteToWideChar(CP_ACP, 0, lpszGb32Text, -1, pUnicodeBuf, nUnicodeBufLen);

                int nUtf8BufLen = WideCharToMultiByte(CP_UTF8, 0, pUnicodeBuf, -1, 0, 0, NULL, NULL);
                if (nUtf8BufLen == 0)
                {
                    delete[] pUnicodeBuf;
                    return "";
                }

                char* pUft8Buf = new char[nUtf8BufLen];
                if (pUft8Buf == 0)
                {
                    delete[] pUnicodeBuf;
                    return "";
                }

                WideCharToMultiByte(CP_UTF8, 0, pUnicodeBuf, -1, pUft8Buf, nUtf8BufLen, NULL, NULL);

                std::string strUtf8 = pUft8Buf;

                delete[] pUnicodeBuf;
                delete[] pUft8Buf;

                return strUtf8;
        }

        template <class S=std::string>
        S trimchar(const S& str, const char ch)
        {
            int first = 0;
            int last = int(str.size()) - 1;

            while (first <= last && str[first] == ch) ++first;
            while (last >= first && str[last]  == ch) --last;

            return S(str, first, last - first + 1);
        }

        IMAPClientSession::IMAPClientSession(const StreamSocket& socket) :
            _socket(socket),
            _isOpen(true),
            _tag(1)
        {

        }

        IMAPClientSession::IMAPClientSession(const std::string& host, Poco::UInt16 port) :
        _socket(SocketAddress(host, port)),
        _isOpen(true),
        _tag(1),
        _host(host)
        {

        }

        IMAPClientSession::~IMAPClientSession()
        {
            try
            {
                close();
            }
            catch (...)
            {
            }
        }

        bool IMAPClientSession::checkCapability(const std::string& cap)
        {
            for (auto c : _capability) {
                if (c == cap) {
                    return true;
                }
            }
            return false;
        }

        void IMAPClientSession::setTimeout(const Poco::Timespan& timeout)
        {
            _socket.setReceiveTimeout(timeout);
        }

        Poco::Timespan IMAPClientSession::getTimeout() const
        {
            return _socket.getReceiveTimeout();
        }

        void IMAPClientSession::capability()
        {
            std::string response, raw;
            std::vector<std::string> data, tokens;

            _capability.clear();

            if (!sendCommand("CAPABILITY", response, data)) throw NetException("can't obtain capability", response);

            for (auto s: data) {
                std::vector<std::string> tokens1;
                tokenize(s, tokens1, std::string(" "), std::string("\"\""));
                if (tokens1[0] == "*" && tokens1[1] == "CAPABILITY") {
                    raw = s;
                    break;
                }
            }

            if (raw == "") throw NetException("No capability response", response);

            // Parse capability.
            tokenize(raw, tokens, std::string(" "), std::string("\"\""));
            if (tokens.size() < 3)  throw NetException("No capability response", response);
            for (auto c : tokens) {
                if (c == "CAPABILITY" || c=="*") continue;
                _capability.push_back(c);
            }
        }


        void IMAPClientSession::login(const std::string& username, const std::string& password)
        {
            std::string response;
            std::vector<std::string> data;

            if (!sendCommand("LOGIN " + username + " " + password, response, data)) throw NetException("Login rejected for login", response);

            capability();
        }

        void IMAPClientSession::close()
        {
            if (_isOpen)
            {
                std::string response;
                std::vector<std::string> data;
                sendCommand("LOGOUT", response, data);
                _socket.close();
                _isOpen = false;
            }
        }

        void IMAPClientSession::noop()
        {
            std::string response;
            std::vector<std::string> data;
            if (!sendCommand("NOOP", response, data)) throw NetException("The IMAP conection has been desconected", response);
        }

        void IMAPClientSession::unsubscribe(const std::string& folder)
        {
            std::string response;
            std::vector<std::string> data;
            std::ostringstream oss;
            oss << "UNSUBSCRIBE \"" << folder << "\"";
            if (!sendCommand(oss.str(), response, data)) throw NetException("Subscribe error", response);
        }

        void IMAPClientSession::subscribe(const std::string& folder)
        {
            std::string response;
            std::vector<std::string> data;
            std::ostringstream oss;
            oss << "SUBSCRIBE \"" << folder << "\"";
            if (!sendCommand(oss.str(), response, data)) throw NetException("Unsubscribe error", response);
        }

        bool IMAPClientSession::sendCommand(const std::string& command, std::string& response, std::vector<std::string>& data)
        {
            std::string tag;
            NumberFormatter::append0(tag, _tag++, 10);

            _socket.sendMessage(tag + " " + command);
#ifdef _DEBUG
            std::cout << tag + " " + command << std::endl;
#endif
            while (true) {
                _socket.receiveMessage(response);
                if (response.substr(0, tag.length()+1) != (tag+" ")) {
                    data.push_back(response);
                    response.clear();
                }
                else break;
            }
            return response.substr(tag.length()+1, 2) == "OK";
        }


        void IMAPClientSession::listFolders(const std::string& root, FolderInfoVec& folders)
        {
            folders.clear();
            std::string response;
            std::vector<std::string> data;

            // FIXME: escape query.
            if (!sendCommand("LIST \"" + root + "\" *", response, data)) throw NetException("Cannot get folders list", response);

            for (auto r : data)
            {
                std::vector<std::string> tokens, tokens2;
                FolderInfo  f;
                tokenize(r, tokens, std::string(" "), std::string("\"\""));
                tokenize(r.substr(r.find(")")), tokens2, std::string(" "), std::string("\"\""));

                f.name = trimchar(tokens2[2],'"');
                f.flags = tokens[2];

                folders.push_back(f);
            }
        }

        void IMAPClientSession::listMessages(const std::string& folder, std::vector<std::string>& uids)
        {
            std::string tag;
            std::string response, response1, response2;
            std::vector<std::string> data, data1, data2;

            if (!sendCommand("SELECT \"" + folder + "\"", response1, data1)) throw NetException("Cannot select folder", response1);

            std::ostringstream oss;

            //oss << "tag " << "UID SORT (" << order << ") UTF-8 " << filter ;
            oss << "UID SEARCH ALL";

            if (!sendCommand(oss.str(), response, data))  throw NetException("Cannot fetch messages", response);

//            sendCommand("CLOSE", response2, data2);

            for (int j = 0; j < data.size(); j++)
            {
                auto r = data[j];
                std::vector<std::string> tokens;
                tokenize(r, tokens, std::string(" "), std::string("\"\""));
                if (tokens[1] != "SEARCH") continue;
                for (int i = 2; i < tokens.size(); i++) {
                    uids.push_back(tokens[i]);
                }
            }
        }


        void IMAPClientSession::copyMessage(const std::string& uid, const std::string& from_folder, const std::string& to_folder)
        {
            std::string response;
            std::vector<std::string> data;

            if (!sendCommand("SELECT \"" + from_folder + "\" *", response, data)) throw NetException("Can't select from folder", response);
            if (!sendCommand("UID COPY " + uid + "  \"" + to_folder + "\" *", response, data)) throw NetException("failed copy the message", response);
            sendCommand("CLOSE", response, data);
        }


        void IMAPClientSession::moveMessage(const std::string& uid, const std::string& from_folder, const std::string& to_folder)
        {
            std::string response;
            std::vector<std::string> data;

            if (from_folder == to_folder) return;

            if (!checkCapability("MOVE")) {
                // ALTERNATIVE.
                // If the server has no support for MOVE, we can still move messages by copy and delete.
                moveMessage_without_MOVE(uid, from_folder, to_folder);
                return;
            }
            if (!sendCommand("SELECT \"" + from_folder + "\" *", response, data)) throw NetException("Can't select from folder", response);
            if (!sendCommand("UID MOVE " +  uid + "  \"" + to_folder + "\" *", response, data)) throw NetException("failed to move the message", response);
            sendCommand("CLOSE", response, data);
        }


        // move message version when the backend has no MOVE EXTENSION.
        void IMAPClientSession::moveMessage_without_MOVE(const std::string& uid, const std::string& from_folder, const std::string& to_folder)
        {
            std::string response;
            std::vector<std::string> data;

            if (!sendCommand("SELECT \""  + from_folder + "\" *", response, data)) throw NetException("Can't select from folder", response);
            if (!sendCommand("UID COPY "  + uid + "  \"" + to_folder + "\" *", response, data)) throw NetException("failed to copy the message", response);
            if (!sendCommand("UID STORE " + uid + " +FLAGS.SILENT (\\Deleted)", response, data)) throw NetException("failed to delete the message", response);
            if (!sendCommand("EXPUNGE", response, data)) throw NetException("failed to expunge", response);
            sendCommand("CLOSE", response, data);
        }


        void IMAPClientSession::deleteMessage(const std::string& uid, const std::string& folder, bool expunge)
        {
            std::string response;
            std::vector<std::string> data;

            if (!sendCommand("SELECT \"" + folder + "\" *", response, data)) throw NetException("Can't select from folder", response);
            if (!sendCommand("UID STORE " + uid + " +FLAGS.SILENT (\\Deleted)", response, data)) throw NetException("failed to delete the message", response);
            if (expunge)
               if (!sendCommand("EXPUNGE", response, data)) throw NetException("failed to expunge", response);
            sendCommand("CLOSE", response, data);
        }


        void IMAPClientSession::createFolder(const std::string& folder)
        {
            std::string response;
            std::vector<std::string> data;

            if (!sendCommand("CREATE \"" + folder + "\" *", response, data)) throw NetException("Can't create folder", response);
        }


        void IMAPClientSession::deleteFolder(const std::string& folder)
        {
            std::string response;
            std::vector<std::string> data;

            if (!sendCommand("DELETE \"" + folder + "\" *", response, data)) throw NetException("Can't delete folder", response);
        }

        void IMAPClientSession::loadMessage (const std::string & folder, MessageInfo & info) {
            std::string response, tmpdata;
            std::vector<std::string> data, data1;

            if ( !sendCommand ("SELECT \"" + folder + "\"", response, data) ) throw NetException ("Can't select from folder", response);

            loadText (info.uid, info.parts, "", "HTML" , info.htmlText);
            loadText (info.uid, info.parts, "", "PLAIN" , info.text);

//            sendCommand ("CLOSE", response, data1);

//            if ( data.size ( ) <= 2 ) return;

//            for ( int i = 1; i < data.size ( ) - 2; i++ ) {
//                if( data[i].length() > 0 && data[i][0] != '*' ){
//                    message += data[i];
//                    message += "\r\n";
//                }
//            }

        }

        void IMAPClientSession::loadText (const std::string & uid, const PartInfo & info, const std::string & index, const std::string & _type, std::string & text) {

            auto & attrs = info.attributes;

            std::string type = _type;

            std::transform(type.begin (), type.end (), type.begin (), ::toupper );

            std::string response;
            std::vector<std::string> result;
            std::stringstream ss;

            if ( std::find (attrs.begin ( ), attrs.end ( ), type) != info.attributes.end ( ) ) {      // 如果是正文类型

                if ( !sendCommand ("UID FETCH " + uid + " BODY.PEEK[" + ( index.length() == 0 ? "1" : index ) + "]", response, result) )
                    throw NetException ("Cannot Load Mail Text", response);

                // 解码

                for (auto line : result) {
                    if( line.length() > 0 && line.at(0) != '*' )
                        ss << line << "\n";
                }

                if ( std::find (attrs.begin ( ), attrs.end ( ), "BASE64") != attrs.end() ) {	// BASE64编码的正文
                    Base64Decoder decode (ss);
                    char c;
                    while ( !decode.eof ( ) && ( c = decode.get ( ) ) ) {
                        text += c;
                    }
                } else if( std::find (attrs.begin ( ), attrs.end ( ), "QUOTED-PRINTABLE") != attrs.end ( ) ){
                    QuotedPrintableDecoder qpd (ss);
                    char c;
                    while ( !qpd.eof ( ) && ( c = qpd.get ( ) ) ) {
                        text += c;
                    }

                } else {
                    std::cout << "Unknown Content Transfer Type: " ;
                    for(auto s : attrs){
                        std::cout << s << ";";
                    }
//                    std::cout << "Content: " << ss.str ()<< std::endl;
                    text += ss.str ( );
                    //throw NetException ("UNKNOWN CONTENT TRANSFER TYPE");
                }

            }

            for ( int i = 0; i < info.childs.size ( ); i++ ) {
                std::string nextindex = index.length () == 0 ? std::to_string(i+1) : index + "." + std::to_string(i+1);
                loadText (uid, info.childs.at(i), nextindex, type,  text);
            }

//            text += "\n";

            return;

        }

        void IMAPClientSession::loadParts (const std::string & uid, const PartInfo & info, const std::string & index, const std::string & _type, std::vector<std::string> & paths){

            auto & attrs = info.attributes;

            std::string type = _type;

            std::transform(type.begin (), type.end (), type.begin (), ::toupper );

            std::string response;
            std::vector<std::string> result;

            if ( std::find (attrs.begin ( ), attrs.end ( ), type) != info.attributes.end ( ) ) {

                paths.push_back (index);

            }

            for ( int i = 0; i < info.childs.size ( ); i++ ) {
                std::string nextindex = index.length () == 0 ? std::to_string(i+1) : index + "." + std::to_string(i+1);
                loadParts(uid, info.childs.at(i), nextindex, type,  paths);
            }


        }


        void IMAPClientSession::getMessages(const std::string& folder, std::vector<std::string>& uids, MessageInfoVec& messages)
        {
            std::string response, response1;
            std::vector<std::string> data, data1;

            if (!sendCommand("SELECT \"" + folder + "\"", response1, data1)) throw NetException("Cannot select folder", response1);

            std::ostringstream oss;
            oss << "UID FETCH ";

            for (int i = 0; i < uids.size(); i++) {
                if (i > 0) oss << ",";
                oss << uids[i];
            }

            oss << " (RFC822.SIZE UID FLAGS INTERNALDATE BODYSTRUCTURE body.PEEK[header.fields (SUBJECT FROM CONTENT-TYPE TEXT X-PRIORITY TO CC)])";

            if (!sendCommand(oss.str(), response, data)) throw NetException("Cannot fetch messages", response);

            response1.clear(); data1.clear();
//            if (!sendCommand("CLOSE", response1, data1)) throw NetException("Cannot close folder", response);

            for (int j = 0; j < data.size(); j++)
            {
                auto r = data[j];
                std::vector<std::string> tokens, tokens2, tokens3;
                tokenize(r + ")", tokens, std::string(" "), std::string("()"), true);
                MessageInfo  m;
                if (tokens[0]!="*") throw NetException("Cannot fetch messages", r);

                m.uid = tokens[1];
                tokenize(tokens[3], tokens2, std::string(" "), std::string("\"\"()[]"), true);
                for (int i = 0; i < tokens2.size();i++)
                {
                    std::string cmd = toUpper(tokens2[i]);

                    if (cmd == "RFC822.SIZE")
                         m.size = atoi(tokens2[++i].c_str());
                    else if (cmd == "BODYSTRUCTURE")
                        m.parts = parseBodyStructure(std::istringstream(tokens2[i + 1]));
                    else if (cmd == "INTERNALDATE")
                        m.date = tokens2[++i];
                    else if (cmd == "UID")
                        m.uid = tokens2[++i];
                    else if (cmd == "FLAGS") {
                        m.flags = tokens2[++i];
                        m.seen      = m.flags.find("Seen") != std::string::npos;
                        m.forwarded = m.flags.find("Forwarded") != std::string::npos;
                    }
                }

                while (true) {
                    r = data[++j];
                    if (r == "") {
                        j++;
                        break;
                    }
//                    RFC2047 say:
//                        An 'encoded-word' may not be more than 75 characters long, including
//                        'charset', 'encoding', 'encoded-text', and delimiters.  If it is
//                        desirable to encode more text than will fit in an 'encoded-word' of
//                        75 characters, multiple 'encoded-word's (separated by CRLF SPACE) may be used

                    if (j < data.size() - 2) {
                        // check if next lines begins with a SPACE, if so, merge with main line.
                        while (true && j<data.size()) {
                            if (data[j + 1].substr(0, 1) == " ") {
                                r += data[++j];
                                continue;
                            }
                            break;
                        }
                    }

                    std::vector<std::string> tokens4;
                    tokenize(r, tokens4, std::string(":"), std::string("()"));
                    std::string cmd = toUpper(tokens4[0]);

                    if (tokens4.size() < 2) continue;

                    if ( cmd == "SUBJECT" )
                        m.subject = IMAPClientSession::decoder(trim (r.substr (8)));
                    //m.subject = MessageHeader::decodeWord(trim(r.substr(8)));
                    else if ( cmd == "FROM" )
                        m.from = IMAPClientSession::decoder(trim (tokens4[1]));
                    //m.from = MessageHeader::decodeWord(trim(tokens4[1]));
                    else if ( cmd == "TO" )
                        m.to = IMAPClientSession::decoder (trim (tokens4[1]));
                        //m.to = MessageHeader::decodeWord(trim(tokens4[1]));
                }
                messages.push_back(m);
            }
        }

        IMAPClientSession::PartInfo  IMAPClientSession::parseBodyStructure(std::istringstream& src)
        {
            char c;
            PartInfo mi;
            bool inside = false;
            std::string token;

            while ( !src.eof ( ) ) {
                src.get (c);

                if ( c == '"' ) {
                    inside = !inside;
                    continue;
                }
                if ( c == ')' ) {
                    if ( token != "" ) mi.attributes.push_back (token);
                    break;
                }

                if ( c == ' ' ) {
                    if ( token != "" ) mi.attributes.push_back (trimchar (token, '"'));
                    token.clear ( );
                    continue;
                }

                if ( c == '(' ) {
                    mi.childs.push_back (parseBodyStructure (src));
                    continue;
                }

                token += c;
            }

            return mi;
        }

        void IMAPClientSession::decodeRFC2047
            (const std::string & ins, std::string & outs, const std::string & charset_to ) {

            outs.clear ( );

            StringTokenizer tokens (ins, "?");				//

            char c;
            std::string tmp;
            std::string charset = toUpper (tokens[0]);
            std::string encoding = toUpper (tokens[1]);
            std::string text = tokens[2];

            std::istringstream iss (text);

            if ( encoding[0] == 'B' ) {	// Base64编码
                Base64Decoder decode (iss);
                while ( ( c = decode.get ( ) ) != -1 ) {
                    tmp += c;
                }
            } else if ( encoding[0] == 'Q' ) {		// Quote-Printable编码
                QuotedPrintableDecoder qpd (iss);
                while ( ( c = qpd.get ( ) ) != -1  ) {
                    tmp += c;
                }

            } else {				// 编码未知, 直接返回原字符串
                outs = ins;
                return;
            }

            // 标题转换字符集
            if ( charset != charset_to ) {
                std::transform(charset.begin (), charset.end (), charset.begin (), ::toupper);
                if( charset == "GB2312" ){
                    std::cout << "charset == gb2312" << std::endl;
                    try{
                        outs = GB2312ToUTF8 (tmp.c_str ());
                    }catch( std::exception & e ){
                        std::cout << e.what () << std::endl;
                    }
                }else{
                    try {
                        TextEncoding & enc = TextEncoding::byName (charset);
                        TextEncoding & dec = TextEncoding::byName (charset_to);
                        TextConverter converter (enc, dec);
                        converter.convert (tmp, outs);
                    } catch ( ... ) {			// 无法转换的未知字符集(包括GB2312)
                        std::cout << "Unknown charset " << charset << std::endl;
                        std::cout << "Content: " << tmp << std::endl;
                        outs = tmp;
                    }
                }
            } else {			// 不需要转换字符集
                outs = tmp;
            }

        }

        std::string IMAPClientSession::decoder (const std::string & _text, const std::string& charset ){
            std::string tmpout, outs, text = _text;
            for ( ; ; ) {
                auto start = text.find ("=?");	// 找不到则返回npos
                if ( start == std::string::npos ) {		// 找不到=?, 不是MIME编码类型, 直接返回
                    outs += text;
                    break;
                }
                if ( start > 0 ) {
                    outs += text.substr (0, start);
                }

                text = text.substr (start+2);		// 从=?之后开始查找其余信息

                auto second = text.find ("?");	// 第二部分是字符集编码
                if ( second == std::string::npos ) {
                    outs += text;
                    break;
                }
                auto third = text.find ("?", second+1);		// 第三部分是正文MIME编码
                if ( third == std::string::npos ) {
                    outs += text;
                    break;
                }
                auto last = text.find ("?=", third+1);		// MIME编码结束标志
                if ( last == std::string::npos ) {
                    outs += text;
                    break;
                }
                IMAPClientSession::decodeRFC2047 (text.substr (0, last), tmpout , charset);
                outs += tmpout;

                text = text.substr (last + 2);
            }
            return outs;
        }


    }
}
