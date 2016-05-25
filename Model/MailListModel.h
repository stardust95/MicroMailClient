#ifndef MAILLISTMODEL_H
#define MAILLISTMODEL_H

#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QVariant>
#include <QList>
#include <QHash>
#include <QByteArray>
#include <QtAlgorithms>
#include <QQmlEngine>
#include <QDebug>

#include "MailBody.h"
#include "IMAPClient.h"
#include "ReceiveMailClient.h"
#include "SendMailClient.h"
#include "Utils.h"

namespace Models{

    class MailListModel : public QAbstractListModel {
        Q_OBJECT
    public:

        explicit MailListModel(QObject * parent = 0) : QAbstractListModel(parent) {
            _receiveProtocol = Utils::ProtocolType::IMAP;
        }

        ~MailListModel(){

        }


        enum MailBodyRoles{
            mail_subject = Qt::UserRole + 1,
            mail_content,
            mail_datetime,
            mail_isread,
            mail_sender,
            mail_recipients,
    //        mail_attachments
        };


        QHash<int, QByteArray> roleNames() const{
            QHash<int, QByteArray> roles;
            roles[mail_subject] = "mail_subject";
            roles[mail_content] = "mail_content";
            roles[mail_datetime] = "mail_datetime";
            roles[mail_isread] = "mail_isread";
            roles[mail_sender] = "mail_sender";
            roles[mail_recipients] =  "mail_recipients";
    //        roles[mail_attachments] = "mail_attachments";
            return roles;
        }


        int rowCount(const QModelIndex & parent = QModelIndex()) const{
            return _mails.size ();
        }

        int columnCount(const QModelIndex & parent = QModelIndex()) const{
            return 1;
        }

        QVariant data(const QModelIndex &index, int role) const{

            if( !index.isValid () )
                return QVariant();
//            qDebug() << "Model data index = " << index.row () << "," << index.column () << " : " << role << "\n";
            auto item = _mails.at(index.row());
            switch( role ){
                case mail_content:
                    return item->getContent ();
                case mail_datetime:
                    return item->getDateTime ();
                case mail_isread:
                    return item->getIsread();
//                case mail_recipients:
//                    return item->recipients.at (0);
                case mail_sender:
                    return item->getSender ();
                case mail_subject:
                    return item->getSubject ();
                default:
                    return QVariant();
                }
        }


        bool setData(const QModelIndex &index, const QVariant &value, int role) {

            if (index.row() >= 0 && index.row() < this->_mails.size()){
                qDebug() << "set data at " << index << " : " << role << "\n";

                switch (role) {
                case mail_isread:
                    this->_mails.at (index.row ())->setIsread (value.toBool ());
                    break;
                default:
                    break;
                }

                emit(dataChanged (index, index));           // 刷新qml的数据关键函数
                return true;
            }

            return false;
        }

//        QVariant headerData(int section, Qt::Orientation orientation, int role) const{

//        }

         void buildMailList(const MAILBODY_PTR_QLIST & items){          // only for debug
             if ( items.size () == 0 )
                 return;
             this->beginInsertRows (QModelIndex(), this->rowCount (), this->rowCount () + items.size () - 1);

             foreach (auto item, items) {
                 this->_mails.append (item);
             }

             this->endInsertRows ();

         }

         QList<QString> getFolders( ){
             return _foldersMap.keys ();
         }

        MAILBODY_PTR_QLIST toList(QString folder) const{
            return this->_mails;
        }

//        Q_INVOKABLE QString getMailCo

        Q_INVOKABLE QVariant get(int index){
            if( index >= 0 && index < _mails.size () ){
//                qDebug() << "Model data index = " << index << "," << index << " : " << role << "\n";
                return QVariant::fromValue(_mails.at (index));
            }

            return QVariant();
        }
        Q_INVOKABLE bool login(QString user, QString passwd, QString host, QString port){
/*
        Login and retrive all mails within folders.
*/
            if( _receiveProtocol == Utils::ProtocolType::IMAP )
                _receiveClient = QSharedPointer<IMAPClient>::create(host, port);
            // _sendClient = MAILCLIENT_PTR::create(host, port);

            if( _receiveClient->login(user, passwd) ){          // if login success, retrive mails
//            if( _sendClient->login (user, passwd) && _receiveClient->login (user,  passwd) ){

                // First get mails
                QList<QString> folders;
                _receiveClient->getFolders (folders);          // Assume that POP3 only has one folder "INBOX"
                for(auto folder : folders){
                    int cur_count = _mails.length ();
                    QList<int> indexs;
                    _receiveClient->getMailBodies (folder, _mails);

                    while( ++cur_count < _mails.length () ){
                        indexs.push_back (cur_count);
                    }

                    _foldersMap[folder] = indexs;
                }

                // Then initialize the user info
                _user = ACCOUNT_PTR::create();
                return true;
            }
            return false;
        }

    private:

        MAILBODY_PTR_QLIST _mails;

        ACCOUNT_PTR _user;

        QMap<QString, QList<int>> _foldersMap;

        SENDMAILCLIENT_PTR _sendClient;

        RECEIVEMAILCLIENT_PTR _receiveClient;

        Utils::ProtocolType _receiveProtocol;

    };

}

#endif // MAILLISTMODEL_H
