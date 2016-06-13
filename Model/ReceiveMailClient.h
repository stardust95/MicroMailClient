#ifndef RECEIVEMAILCLIENT_H
#define RECEIVEMAILCLIENT_H

#include "MailClient.h"
#include "MailBody.h"

#include <QSharedPointer>
#include <QMap>
#include <QString>
#include <QDateTime>

class ReceiveMailClient : public MailClient{

public:

        virtual int getFolders(QList<QString> & ) = 0;

        virtual int getMailBodies(QList<MAILBODY_PTR> & , int count) = 0;   // DO NOT CLEAR the list in this function

        virtual int DeleteMail(const QList<int> &) = 0;
		
        virtual int selectFolder(const QString & ) = 0;

        virtual int getAttachment(const Attachment & attach, std::string & outdata) = 0;

        virtual QString getSelectedFolder() final {           // final ==> cannot be override
            return _selectedFolder;
        }

protected:

    int _curListIndex;

    QString _selectedFolder;

};

typedef QSharedPointer<ReceiveMailClient> RECEIVEMAILCLIENT_PTR;

#endif // RECEIVEMAILCLIENT_H

