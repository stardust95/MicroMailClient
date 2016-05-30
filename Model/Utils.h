#ifndef UTILS_H
#define UTILS_H

#include<Poco/Net/MediaType.h>

class Utils{
public:
    typedef Poco::Net::MediaType MediaType;

    enum ProtocolType{
        SMTP, POP3, IMAP
    };


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
