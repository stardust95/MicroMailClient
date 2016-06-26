#ifndef MAILLISTMODEL_H
#define MAILLISTMODEL_H

#include "MailBody.h"
#include "IMAPClient.h"
#include "POP3Client.h"
#include "SMTPClient.h"
#include "Utils.h"
#include "Exception/MailGenerationException.h"
#include "Exception/MailSendException.h"
#include "Exception/MailReceiveException.h"


#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QFile>
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
            _sendClient.clear ();
            _receiveClient.clear ();
            _progress = false;
            _user.clear ();

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
                    qDebug() << "this->_mails.append (item)\n";
                    this->_mails.append (item);
                }

                this->endInsertRows ();
            });
        }

        void clearRows(){

            if( _mails.size () == 0 )
                return;

            postTo(this, [this]{
                this->beginResetModel ();

                this->_mails.clear ();

                this->endResetModel ();
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

        Q_INVOKABLE QStringList getAttachments(int index){

            QStringList result;
            if( index >= 0 && index < _mails.length ()){
                for(auto attach : _mails.at (index)->getAttachements ()){
                    result.append (attach.getFileName ());
                }
            }
            return result;
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

            this->clearRows ();

            this->setProgress (0);

            if( folderIndex >= 0 && folderIndex < this->getFolders ().size ()
                    && this->getFolders ().at (folderIndex) != this->_receiveClient->getSelectedFolder() ){

                auto folder = this->getFolders ().at (folderIndex);             // in login function has got folders

                int total = _receiveClient->selectFolder(folder);

                MAILBODY_PTR_QLIST tmplist;

                qDebug() << "do Build Mail List in folder " << folder << "\n";

                while( _receiveClient->getMailBodies ( tmplist, 1) > 0 ){      // if there exists mails not loaded

                    qDebug() << "_receiveClient->getMailBodies ( tmplist, 1) > 0" << "\n";

//                    this->beginResetModel ();

                    this->appendRows (tmplist);

                    this->setProgress ( this->_progress + 1.0/total );

                    qDebug() << "progress set to " << this->getProgress () << "\n";

//                    this->endResetModel ();

                    tmplist.clear ();

                }

                _foldersMap[folder] = _mails;

                return true;
            }

            return false;
       }

	Q_INVOKABLE void setProtocol(QString p){
	    if( p == "IMAP" ){
		_receiveProtocol = Utils::ProtocolType::IMAP;
	    }else if ( p == "POP3" ){
		_receiveProtocol = Utils::ProtocolType::POP3;
	    }else{
		throw MailGenerationException("Unknown Receive Protocol");
	    }
	}

    Q_INVOKABLE bool downloadAttach(int mailIndex, int attachIndex, QString path){
        std::string data;

        try{

            if( mailIndex >= _mails.size () )
                throw MailReceiveException("mailIndex >= _mails.size ()");

            Attachment attach = _mails.at (mailIndex)->getAttachment (attachIndex);

            qDebug() << "in downloadAttach, name = " << attach.getFileName () << "\n";

            if( !_receiveClient->getAttachment ( attach, data ) )
                return false;

            std::ofstream output;
            output.open (attach.getFileName ().toStdString (), std::ofstream::out | std::ofstream::binary);

            if( output.is_open () ){
                output.write (data.c_str (), data.length ());
                output.close ();
            }else{
                throw MailReceiveException("downloadAttach: Unable to open file " + attach.getFileName ().toStdString ());
            }

            system (attach.getFileName ().toStdString ().c_str ());

            return true;
        }

        catch( const std::exception & e ){
            qDebug() << "MailListModel::downloadAttach Exception " << QString(e.what ()) << "\n";
        }

        return false;

    }

	Q_INVOKABLE bool login(QString user, QString passwd, QString sendHost, QString receiveHost, bool requireSSL){
/*
        Login and retrive all mails within folders.
*/

    try{

        // initialize the user info
        _user = ACCOUNT_PTR::create();

		_sendClient = QSharedPointer<SMTPClient>::create(sendHost, requireSSL ? Utils::Port_SMTP_SSL() : Utils::Port_SMTP() );
        _user->setSMTPHost (sendHost);

        if( _receiveProtocol == Utils::ProtocolType::IMAP ){
		    _receiveClient = QSharedPointer<IMAPClient>::create(receiveHost, requireSSL? Utils::Port_IMAP_SSL() : Utils::Port_IMAP() );
            _user->setIMAPHost (receiveHost);
        }
        else if( _receiveProtocol == Utils::ProtocolType::POP3 ){
		    _receiveClient = QSharedPointer<POP3Client>::create(receiveHost, requireSSL? Utils::Port_POP3_SSL() : Utils::Port_POP3() );
            _user->setPOP3Host (receiveHost);
        }
        _user->setPassWord (passwd);
        _user->setRequireSSL (requireSSL);
        _user->setUserName (user);

		if( _receiveClient->login(user, passwd,requireSSL) && _sendClient->login (user, passwd, requireSSL) ) {          // if login success, retrive mails

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
	    catch( std::exception & e ){
                qDebug() << "MailListModel::login Exception " << QString(e.what ()) << "\n";
        }

            return false;
        }

        bool doSendMail(QString recipient, QString subject, QString content, QStringList attachments={}){

            MAILBODY_PTR mail = MAILBODY_PTR::create();

            mail->setContent (content);

            mail->setSubject (subject);

            mail->addRecipient (recipient);

            mail->setSender (_user->getUserName ());

            for(auto filepath : attachments ){
                QString path = filepath.startsWith ("file:///") ? filepath.split ("///").at (1): filepath;
                Attachment attach("application", "octet-stream");
                attach.setFilePath (path);
                mail->addAttachment (attach);
                std::cout << path.toStdString () << std::endl;
            }

            try{
                _sendClient->sendMailBodies (mail);
            }
            catch(const std::exception & e){
                qDebug() << "sendMail: " << QString::fromStdString (e.what ()) << "\n";
            }

            return true;
        }

        Q_INVOKABLE void sendMail(QString recipient, QString subject, QString content, QStringList attachments = {}){

            if( !_user.isNull () )
                QtConcurrent::run(this, &MailListModel::doSendMail, recipient, subject, content, attachments);

            return ;
        }

    private:

        MAILBODY_PTR_QLIST _mails;

        ACCOUNT_PTR _user;

        QMap<QString, MAILBODY_PTR_QLIST> _foldersMap;      // current selected folder stored in _receiveClient

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
