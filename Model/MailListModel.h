#ifndef MAILLISTMODEL_H
#define MAILLISTMODEL_H

#include "MailBody.h"
#include "IMAPClient.h"
#include "ReceiveMailClient.h"
#include "SendMailClient.h"
#include "POP3Client.h"
#include "Utils.h"


#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QVariant>
#include <QList>
#include <QHash>
#include <QByteArray>
#include <QtAlgorithms>
#include <QQmlEngine>
#include <QDebug>
#include <QStringList>
#include <QtConcurrent/QtConcurrent>

#include <fstream>

//#include "SMTPClient.h"

namespace Models{

    template <typename FuncType>
    void postTo(QObject * obj, FuncType && fun) {
        if (obj->thread() != QThread::currentThread()) {
            QObject signalSource;
            QObject::connect(&signalSource, &QObject::destroyed, obj, std::move(fun));
        } else{
            fun();
        }
    }

    class MailListModel : public QAbstractListModel {
        Q_OBJECT

        Q_PROPERTY(double progress READ getProgress NOTIFY progressChanged)

        Q_PROPERTY(QStringList folders READ getFolders NOTIFY foldersChanged)

        Q_PROPERTY(int folderIndex READ getFolderIndex NOTIFY folderIndexChanged)

    public:

        explicit MailListModel(QObject * parent = 0) : QAbstractListModel(parent) {
            _receiveProtocol = Utils::ProtocolType::IMAP;
            _progress = false;
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
            mail_htmlcontent,
    //        mail_attachments
        };


        QHash<int, QByteArray> roleNames() const{
            QHash<int, QByteArray> roles;
            roles[mail_subject] = "mail_subject";
            roles[mail_content] = "mail_content";
            roles[mail_htmlcontent] = "mail_htmlcontent";
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
                case mail_htmlcontent:
                    return item->getHTMLContent ();
                case mail_datetime:
                    return item->getDateTime ().split (' ').first ();
                case mail_isread:
                    return item->getIsread();
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

        void appendRow(const MAILBODY_PTR & item){
            if( item != nullptr ){
                this->appendRows ( MAILBODY_PTR_QLIST() << item );
            }
        }

        void appendRows(const MAILBODY_PTR_QLIST & items){
            if( items.size () == 0 )
                return;

            postTo(this, [this, items]{
                this->beginInsertRows (QModelIndex(), this->rowCount (), this->rowCount () + items.size () - 1);

                for(auto item : items){

                    std::ofstream output(item->getSubject ().toStdString ());
                    output << item->getDateTime ().toStdString ()<< std::endl;
                    output << item->getContent ().toStdString ()<< std::endl;
                    this->_mails.append (item);
                }

                this->endInsertRows ();
            });


        }

        Q_INVOKABLE QStringList getFolders( ){
             return _foldersMap.keys ();
         }

        Q_INVOKABLE int getFolderIndex(){

            int res = 0;

            for(auto it = _foldersMap.keyBegin (); it != _foldersMap.keyEnd ()&& *it != _receiveClient->getSelectedFolder () ; it++, res++);

            if( res>= this->_foldersMap.size () ){
                // throw
            }

            return res;
        }

        MAILBODY_PTR_QLIST toList(const QString & folder) const{
            return this->_mails;
        }


        Q_INVOKABLE QVariant get(int index){
            if( index >= 0 && index < _mails.size () ){
//                qDebug() << "Model data index = " << index << "," << index << " : " << role << "\n";
                return QVariant::fromValue(_mails.at (index));
            }
            return QVariant();
        }

        Q_INVOKABLE QVariant getSubject(int index){
            if( index >= 0 && index < _mails.length () ){
                return QVariant::fromValue(_mails.at (index)->getSubject ());
            }
            return QVariant();
        }

        Q_INVOKABLE QVariant getContent(int index){
            if( index >= 0 && index < _mails.length () ){
                return QVariant::fromValue(_mails.at (index)->getContent ());
            }
            return QVariant();
        }


        Q_INVOKABLE QVariant getHTMLContent(int index){
            if( index >= 0 && index < _mails.length () ){
                return QVariant::fromValue(_mails.at (index)->getHTMLContent ());
            }
            return QVariant();
        }

        Q_INVOKABLE QVariant getSender(int index){
            if( index >= 0 && index < _mails.length () ){
                return QVariant::fromValue(_mails.at (index)->getSender ());
            }
            return QVariant();
        }

        Q_INVOKABLE QVariant getDateTime(int index){
            if( index >= 0 && index < _mails.length () ){
                return QVariant::fromValue(_mails.at (index)->getDateTime ());
            }
            return QVariant();
        }

        Q_INVOKABLE QVariant getRecipients(int index){
            if( index >= 0 && index < _mails.length () ){
                QString result;
                for(auto r : _mails.at(index)->getRecipients ()){
                    if( result.length () > 0 )
                        result += ", ";
                    result += r;
                }
                return QVariant::fromValue(result);
            }
            return QVariant();
        }

        Q_INVOKABLE double getProgress() {
            return _progress;
        }

        void setProgress(double val){
            _progress = val;
            emit(progressChanged ());
        }

        Q_INVOKABLE void buildMailList(int folderIndex){
            QtConcurrent::run(this, &MailListModel::doBuildMailList, folderIndex);
            return ;
        }

        bool doBuildMailList(int folderIndex){                // <= QtConcurrent::run()

            qDebug() << folderIndex << ", " << this->getFolders ().size() << "\n";

            if( folderIndex >= 0 && folderIndex < this->getFolders ().size ()
                    && this->getFolders ().at (folderIndex) != this->_receiveClient->getSelectedFolder() ){

                int cur_count = _mails.length ();

                auto folder = this->getFolders ().at (folderIndex);             // in login function has got folders

//                QString folder = "INBOX";

                int total = _receiveClient->selectFolder(folder);

                MAILBODY_PTR_QLIST tmplist;


                qDebug() << "do Build Mail List in folder " << folder << "\n";

                while( _receiveClient->getMailBodies ( tmplist, 1) > 0 ){      // if there exists mails not loaded

                    qDebug() << "_receiveClient->getMailBodies ( tmplist, 1) > 0" << "\n";

                    this->beginResetModel ();

                    this->appendRows (tmplist);

                    this->setProgress ( this->_progress + 1.0/total );

                    qDebug() << "progress set to " << this->getProgress () << "\n";

                    this->endResetModel ();

                    tmplist.clear ();
                }

                _foldersMap[folder] = tmplist;

                return true;
            }

            return false;
       }



        Q_INVOKABLE bool login(QString user, QString passwd, QString host, QString port){
/*
        Login and retrive all mails within folders.
*/
            try{

    //            if( _receiveProtocol == Utils::ProtocolType::IMAP )
                    _receiveClient = QSharedPointer<IMAPClient>::create(host, port);
                // _sendClient = MAILCLIENT_PTR::create(host, port);

                if( _receiveClient->login(user, passwd,true) ){          // if login success, retrive mails
    //            if( _sendClient->login (user, passwd) && _receiveClient->login (user,  passwd) ){

                    // initialize the user info
                    _user = ACCOUNT_PTR::create();

                    // get mail folders
                    QList<QString> folders;
                    _receiveClient->getFolders (folders);          // Assume that POP3 only has one folder "INBOX"

                    for(auto folder: folders){
                        MAILBODY_PTR_QLIST tmp;
                        this->_foldersMap.insert (folder, tmp);
                    }
                    emit(foldersChanged ());

                    // initially load the mails in first folder
    //                if( folders.size () > 0 )
    //                    QtConcurrent::run(this, &MailListModel::doBuildMailList, 0);

                    return true;
                }
            }
            catch( ... ){
                qDebug() << "MailListModel::login Exception " << "\n";
            }

            return false;
        }

    private:

        MAILBODY_PTR_QLIST _mails;

        ACCOUNT_PTR _user;

        QMap<QString, MAILBODY_PTR_QLIST> _foldersMap;

        SENDMAILCLIENT_PTR _sendClient;

        RECEIVEMAILCLIENT_PTR _receiveClient;

        Utils::ProtocolType _receiveProtocol;

        double  _progress;

    signals:

        void progressChanged();

        void foldersChanged();

        void folderIndexChanged();

    };

}

#endif // MAILLISTMODEL_H
