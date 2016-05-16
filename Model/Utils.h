#ifndef UTILS_H
#define UTILS_H

#include<Poco/Net/MediaType.h>

class Utils{
public:
    typedef Poco::Net::MediaType MediaType;

    enum ProtocolType{
        SMTP, POP3, IMAP
    };

};

#endif // UTILS_H
