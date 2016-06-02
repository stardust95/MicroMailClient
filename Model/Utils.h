#ifndef UTILS_H
#define UTILS_H

#include "Poco/Net/MediaType.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Net/QuotedPrintableDecoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/String.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/TextEncoding.h"
#include "Poco/TextConverter.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/NetSSL.h"
#include "Poco/Net/X509Certificate.h"
#include "Poco/Net/Context.h"
#include "Poco/NumberFormatter.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/MailMessage.h"
#include "Poco/StringTokenizer.h"

#include <istream>
#include <iostream>
#include <string>
#include <vector>
#include <ostream>
#include <streambuf>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>

using Poco::StringTokenizer;
using Poco::NumberFormatter;
using Poco::DateTimeFormatter;
using Poco::StreamCopier;
using Poco::Base64Decoder;
using Poco::Net::MessageHeader;
using Poco::Net::QuotedPrintableDecoder;
using Poco::TextEncoding;
using Poco::TextConverter;


using Poco::trim;
using Poco::toUpper;


class Utils{
private:   //used for fuzzy search

    const static int level = 2;  //0~4

    struct check {
        bool _satisfy;
        int _degree;
        QString _string;
    };

    static check _satisfyFuzzy(const QString& key, const QString& src) {
        int satisfy = 0;
        int total = 0;
        QString subString;
        for(int i = 0; i < key.length() - 1; i++)
            for(int j = i; j < key.length(); j++)
            {
                total++;
                subString = key.mid(i, j-i+1);
                if(src.contains(subString))
                    satisfy++;
            }

        bool flag = false;

        switch(level) {  //等级可以自己修改
            case(0):
                if(satisfy >= 1) flag = true;
                break;
            case(1):
                if(satisfy >= key.length()/2) flag = true;
                break;
            case(2):
                if((float)satisfy >= key.length()*0.7) flag = true;
                break;
            case(3):
                if((float)satisfy >= total*0.6) flag = true;
                break;
            case(4):
                if(satisfy == total) flag = true;
                break;
        }
        if(flag)
            return {true, satisfy, src};
        else
            return {false, 0, NULL};

    }

    static bool myCompare(const check & p, const check & q) {  //check if a < b
        if(q._degree > p._degree)
            return true;
        else if(q._degree == p._degree)
            if(q._string.length() < p._string.length())
                return true;
            else
                return false;
        return false;
    }


public:

    static QStringList ListContacts(const QString& key, const QStringList& _contacts) {
        QVector<check> tmp;
        QStringList result;
        check temp;
        foreach(QString src, _contacts) {
            if((temp = _satisfyFuzzy(key, src))._satisfy) {
                int i;
                for(i = 0; i < tmp.size(); i++)
                    if(myCompare(tmp[i], temp)) break;
                tmp.insert(i, temp);
            }
        }
        foreach (check i, tmp) {
            result.push_back(i._string);
        }
        return result;
    }


public:
    typedef Poco::Net::MediaType MediaType;

    enum ProtocolType{
        SMTP, POP3, IMAP
    };

    // GB2312转UTF8
    static std::string GB2312ToUTF8(const char* lpszGb32Text)
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
    static S trimchar(const S& str, const char ch)
    {
        int first = 0;
        int last = int(str.size()) - 1;

        while (first <= last && str[first] == ch) ++first;
        while (last >= first && str[last]  == ch) --last;

        return S(str, first, last - first + 1);
    }

    static void decodeRFC2047 (const std::string & ins, std::string & outs, const std::string & charset_to = "UTF-8" ) {

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

    static std::string decoder (const std::string & _text, const std::string& charset = "UTF-8"){
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
            Utils::decodeRFC2047 (text.substr (0, last), tmpout , charset);
            outs += tmpout;

            text = text.substr (last + 2);
        }
        return outs;
    }

    template < class T = std::string>
    static void tokenize (const T& str, std::vector<T>& tokens,
                          const T& delimiters, const T& parenthesis, bool trimParenthesis = false) {
        T token;
        T opened_par;

        for ( std::string::value_type c : str ) {
            for ( int i = 0; i < parenthesis.size ( ); i += 2 ) {
                // Open a new one.
                if ( c == parenthesis[i] ) {
                    // special case, when the open char = close char, a second ocurrence must close, not open a new one.
                    if ( !( opened_par.size ( )>0 &&
                           opened_par[opened_par.size ( ) - 1] == c &&
                           parenthesis[i] == parenthesis[i + 1] ) ) {
                        opened_par.push_back (parenthesis[i]);
                        continue;
                    }
                }

                // if close, check that if is already open.
                if ( c == parenthesis[i + 1] && opened_par.size ( ) ) {
                    // Check if is the last open
                    if ( parenthesis[i] == opened_par[opened_par.size ( ) - 1] ) {
                        // close it
                        opened_par.pop_back ( );
                    }
                }
            }

            if ( opened_par.size ( ) > 0 || ( delimiters.find (c) == std::string::npos ) ) {
                token += c;
            } else {
                // remove parenthesis if apply
                if ( trimParenthesis ) {
                    for ( int i = 0; i < parenthesis.size ( ); i++ ) {
                        if ( token.size ( ) < 2 ) break;
                        if ( token[0] == parenthesis[i] && token[token.size ( ) - 1] == parenthesis[i + 1] )
                            token = token.substr (1, token.length ( ) - 2);
                    }
                }
                tokens.push_back (token);
                token.clear ( );
            }
        }

        if ( !token.empty ( ) ) {
            if ( trimParenthesis ) {
                for ( int i = 0; i < parenthesis.size ( ); i++ ) {
                    if ( token.size ( ) < 2 ) break;
                    if ( token[0] == parenthesis[i] && token[token.size ( ) - 1] == parenthesis[i + 1] )
                        token = token.substr (1, token.length ( ) - 2);
                }
            }
            tokens.push_back (token);
        }
    }


};

#endif // UTILS_H
